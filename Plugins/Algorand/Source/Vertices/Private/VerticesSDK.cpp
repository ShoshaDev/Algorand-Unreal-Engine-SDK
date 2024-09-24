// Copyright 2022, Wisdom Labs. All Rights Reserved

#include "VerticesSDK.h"
#include "Misc/Paths.h"
#include "Misc/MessageDialog.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Internationalization/Text.h"
#include "Interfaces/IPluginManager.h"
#include "VerticesApiOperations.h"
#include "ResponseBuilers.h"
#include "HAL/UnrealMemory.h"
#include "AES.h"
#include <cstring>

#include "FileHelper.h"
#include "SDKException.h"
#include "include/vertices/vertices_http.h"
#include "Windows/AllowWindowsPlatformTypes.h"

using namespace std;

DECLARE_LOG_CATEGORY_EXTERN(LogMyAwesomeGame, Log, All);

#define PUBLIC_ADDRESS_LENGTH 58
#define LOCTEXT_NAMESPACE "FVerticesModule"

#define CHECK_DLL_LOAD(loaded) \
    do { \
    if (!(loaded)) { \
        UE_LOG(LogTemp, Warning, TEXT("💥 Failed loading of dll libraries.")); \
        ret_code_t error_code = VTC_ERROR_ASSERT_FAILS; /* Define or ensure this is defined elsewhere */ \
        checkVTCSuccess((char*)"Failed loading of dll libraries", error_code); \
    } \
} while(0)

#define INIT_VERTICES(withNewWallet, err_code) \
    do { \
        InitVertices(withNewWallet, err_code); \
        checkVTCSuccess(const_cast<char*>("Vertices was not built"), (err_code)); \
    } while (0)

#define RETURN_ERRCODE(err_code) \
do { \
    if( err_code != VTC_SUCCESS ) {\
        return;\
    }\
} while (0)

#define PROCESS_QUEUE_WHILE_SUCCESS(queue_size, err_code, txID) \
    do { \
        while ((queue_size) && (err_code) == VTC_SUCCESS) { \
            (err_code) = vertices_event_process(&(queue_size), (txID)); \
        } \
    } while (0)

s_account_t sender_account;

#ifdef __cplusplus
extern "C++" {
#endif

ret_code_t vertices_evt_handler(vtc_evt_t* evt) {
    ret_code_t err_code = VTC_SUCCESS;

    try
    {
        switch (evt->type) {
            case VTC_EVT_TX_READY_TO_SIGN: {
                signed_transaction_t* tx = nullptr;
                err_code = vertices_event_tx_get(evt->bufid, &tx);
                if (err_code == VTC_SUCCESS) { 
                    // libsodium wants to have private and public keys concatenated
                    unsigned char keys[crypto_sign_ed25519_SECRETKEYBYTES] = { 0 };
                    memcpy(keys, sender_account.private_key, sizeof(sender_account.private_key));
                    memcpy(&keys[32],
                        sender_account.vtc_account->public_key,
                        ADDRESS_LENGTH);

                    // prepend "TX" to the payload before signing
                    unsigned char* to_be_signed = new unsigned char[tx->payload_body_length + 2];
                    to_be_signed[0] = 'T';
                    to_be_signed[1] = 'X';

                    // copy body
                    memcpy(&to_be_signed[2],
                        &tx->payload[tx->payload_header_length],
                        tx->payload_body_length);

                    // sign the payload
                    crypto_sign_ed25519_detached(tx->signature,
                        nullptr, to_be_signed, tx->payload_body_length + 2, keys);

                    char b64_signature[128] = { 0 };
                    size_t b64_signature_len = sizeof(b64_signature);
                    b64_encode((const char*)tx->signature,
                        sizeof(tx->signature),
                        b64_signature,
                        &b64_signature_len);

                    // send event to send the signed TX
                    vtc_evt_t sched_evt;
                    sched_evt.type = VTC_EVT_TX_SENDING;
                    sched_evt.bufid = evt->bufid;
                    err_code = vertices_event_schedule(&sched_evt);
                }
            }
                break;

            case VTC_EVT_TX_SENDING: {
                // let's create transaction files which can then be used with `goal clerk ...`
                signed_transaction_t* tx = nullptr;
                err_code = vertices_event_tx_get(evt->bufid, &tx);

                // goal-generated transaction files are packed into a map of one element: `txn`.
                // the one-element map takes 4 bytes into our message packed payload <=> `txn`
                // we also add the `map` type before
                // which results in 5-bytes to be added before the payload at `payload_offset`
                char* payload = new char[tx->payload_body_length + 5];
                payload[0] = (char)(unsigned char)0x81; // starting flag for map of one element
                memcpy(&payload[1],
                    &tx->payload[tx->payload_header_length - 4],
                    tx->payload_body_length + 4);
            }
                break;

            default:
                break;
        }
    }
    catch (std::exception& ex)
    {
        FFormatNamedArguments Arguments;
        Arguments.Add(TEXT("MSG"), FText::FromString(ex.what()));
        FMessageDialog::Open(EAppMsgType::Ok, FText::Format(LOCTEXT("Error", "vertices_evt_handler {MSG}"), Arguments));
        err_code = VTC_ERROR_INVALID_PARAM;
    }
    
    return err_code;
}

#ifdef __cplusplus
} /* extern "C" */
#endif

// provider to access vertices lib
provider_info_t providers;

// bridge witch connect vertices module to thirdparty lib
vertex_t m_vertex;

namespace algorand {
    namespace vertices {
        // load thirdparty libs
        VerticesSDK::VerticesSDK() {
            loaded_ = false;
            
            loadVerticesLibrary();
            
            if (!loaded_)
            {
                FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("ThirdPartyLibraryError", "Failed to load example third party library"));
                return;
            }

            UE_LOG(LogTemp, Display, TEXT("☀️🤣 Loaded C-Vertices-sdk.dll & libsodium.dll from Third Party Plugin sample. 💎🤙"));
            setHTTPCURLs();
            vertices_usable = true;
        }

        VerticesSDK::~VerticesSDK () {

            FPlatformProcess::FreeDllHandle(VerticesHandle);
            FPlatformProcess::FreeDllHandle(SodiumHandle);

            VerticesHandle = nullptr;
            SodiumHandle = nullptr;
        }

        void VerticesSDK::loadVerticesLibrary() {
            loaded_ = false;
            // Get the base directory of this plugin
            FString BaseDir = IPluginManager::Get().FindPlugin("Algorand")->GetBaseDir();
            FString SrcSodiumPath = FPaths::Combine(*BaseDir, TEXT("Source/Vertices/Libs"));
            FString DestSodiumPath = FPaths::Combine(*BaseDir, TEXT("Binaries/Win64"));
            FString VerticesPath = FPaths::Combine(
                *BaseDir, TEXT("Source/ThirdParty/AlgorandLibrary/x64/Release/vertices.dll"));
            
            SodiumHandle = FPlatformProcess::GetDllHandle(*(SrcSodiumPath + "/libsodium.dll"));
            VerticesHandle = FPlatformProcess::GetDllHandle(*VerticesPath);

            if (SodiumHandle == nullptr || VerticesHandle == nullptr)
            {
                return;
            } else {
                // Copy sodium libs to Plugin/Binaries folder
                loaded_ = FileHelper::CopyDirectory(SrcSodiumPath, DestSodiumPath, false);
            }
        }

        void VerticesSDK::InitVertices(bool withNewWallet, ret_code_t& err_code) {
            if ( !myAlgoTokenHeader.Contains("X-Algo-API-Token") && !myAlgoTokenHeader.Contains("x-api-key") && !myAlgoTokenHeader.Equals(""))
            {
                err_code = VTC_ERROR_INVALID_PARAM;
                FFormatNamedArguments Arguments;
                
                Arguments.Add(TEXT("MSG"), FText::FromString(myAlgoTokenHeader));
                FMessageDialog::Open(EAppMsgType::Ok, FText::Format(LOCTEXT("Error","this token header is invalid value: {MSG}"), Arguments));
                checkVTCSuccess(const_cast<char*>("Init Vertices"), err_code);    
            }
            
            char* algodurl = new char[myAlgodRpc.Len()];
            char* indexerurl = new char[myIndexerRpc.Len()];
            char* tokenHeader = new char[myAlgoTokenHeader.Len()];
            memcpy(algodurl, TCHAR_TO_ANSI(*myAlgodRpc), myAlgodRpc.Len());
            memcpy(indexerurl, TCHAR_TO_ANSI(*myIndexerRpc), myIndexerRpc.Len());
            memcpy(tokenHeader, TCHAR_TO_ANSI(*myAlgoTokenHeader), myAlgoTokenHeader.Len());
            algodurl[myAlgodRpc.Len()] = 0;
            indexerurl[myIndexerRpc.Len()] = 0;
            tokenHeader[myAlgoTokenHeader.Len()] = 0;
    
            createNewVertices(algodurl,
                            indexerurl,
                                myAlgoPort,
                                tokenHeader,
                                withNewWallet,
                                err_code);
            if(withNewWallet && err_code == VTC_SUCCESS)
            {
                vertices_ping_check(err_code);
                RETURN_ERRCODE(err_code);
            }
            
            vertices_version_check(err_code);
            RETURN_ERRCODE(err_code);
            
            UE_LOG(LogTemp, Display, TEXT("🔥 Vertices: Created new vertices net."));
        }

        // pass m_vertex through vertices lib
        void VerticesSDK::createNewVertices(char* algod_url, char* indexer_url, short port, char* server_token_header, bool withNewWallet, ret_code_t& err_code) {
            providers.algod_url = algod_url;
            providers.indexer_url = indexer_url;
            providers.port = port;
            providers.header = server_token_header;
            
            sodium_init();
            
            m_vertex.provider = &providers;
            m_vertex.vertices_evt_handler = &vertices_evt_handler;
            
            // create new vertex
            err_code = vertices_new(&m_vertex, withNewWallet);
            checkVTCSuccess(const_cast<char*>("Vertices vertices_new"), err_code);
        }

        void VerticesSDK::vertices_ping_check(ret_code_t& err_code) 
        {
            // making sure the provider is accessible
            err_code = vertices_ping();
            checkVTCSuccess(const_cast<char*>("Vertices ping"),err_code);
        }

        void VerticesSDK::vertices_version_check(ret_code_t& err_code) 
        {
            // ask for provider version
            provider_version_t version = { 0 };
            err_code = vertices_version(&version);
            if (err_code == VTC_ERROR_OFFLINE) {
                UE_LOG(LogTemp, Warning, TEXT("💥 Version might not be accurate : old value is being used %d"), err_code);
            }
            
            checkVTCSuccess(const_cast<char*>("Vertices version"),err_code);
            UE_LOG(LogTemp, Display, TEXT("🔥 Running on %hs v.%u.%u.%u"),
                version.network,
                version.major,
                version.minor,
                version.patch);
        }

        ret_code_t VerticesSDK::convert_Account_Vertices()
        {
            ret_code_t err_code;
            // copy private key to vertices account
            if(main_account.secret_key.size() != ADDRESS_LENGTH)
                err_code = VTC_ERROR_INVALID_PARAM;
            checkVTCSuccess((char *)"Secret key length is not 32 byte", err_code);

            memset(sender_account.private_key, 0 , ADDRESS_LENGTH);
            memcpy(sender_account.private_key, (unsigned char*)main_account.secret_key.data(), ADDRESS_LENGTH);
            
            // copy public key to vertices account
            unsigned char pub_key[ADDRESS_LENGTH]; 
            memset(pub_key, 0 , ADDRESS_LENGTH);
            memcpy(pub_key, main_account.public_key().data(), ADDRESS_LENGTH);
            
            err_code = vertices_account_new_from_bin((char *)pub_key, &sender_account.vtc_account);

            return err_code;
        }
        
        void VerticesSDK::VerticesInitWallet(const VerticesInitWalletRequest& Request, const FVerticesInitWalletDelegate& delegate)
        {           
            AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, [this, Request, delegate]()
            {
                ret_code_t err_code = VTC_SUCCESS;
                while (1) {
                    FScopeLock lock(&m_Mutex);
                    
                    if (vertices_usable) {
                        VerticesSDK::VerticesInitWalletResponse response;
                        vertices_usable = false;
                        
                        try
                        {
                            CHECK_DLL_LOAD(loaded_);
                            INIT_VERTICES(true, err_code);
                            
                            UE_LOG(LogTemp, Display, TEXT("✔️ Vertices: Wallet was initialized."));
                            
                            response = response_builders::buildInitWalletResponse();
                            response.SetSuccessful(true);
                            response.SetResponseString("Wallet was initialized.");
                        }
                        catch(SDKException& e)
                        {
                            response.SetSuccessful(false);
                            response.SetResponseString(FString(e.what()));
                        }
                        catch(std::exception& ex)
                        {
                            response.SetSuccessful(false);
                            response.SetResponseString(FString(ex.what()));
                        }

                        AsyncTask(ENamedThreads::GameThread, [delegate, response]()
                        {
                            delegate.ExecuteIfBound(response);
                        });

                        vertices_usable = true;
                        break;
                    }
                }
            });
        }

        void VerticesSDK::VerticesWallletExists(const VerticesWalletExistenceRequest& Request, const FVerticesWalletExistenceDelegate& delegate)
        {           
            ret_code_t err_code = VTC_SUCCESS;
            while (1)
            {
                FScopeLock lock(&m_Mutex);

                if (vertices_usable)
                {
                    VerticesSDK::VerticesWalletExistenceResponse response;
                    vertices_usable = false;
            
                    try
                    {
                        CHECK_DLL_LOAD(loaded_);
                        INIT_VERTICES(false, err_code);
                        
                        bool exists = vertices_wallet_exists();

                        UE_LOG(LogTemp, Display, TEXT("✔️ Vertices: Wallet %hs"), exists ? "exists." : "does not exist.");
                        
                        response = response_builders::buildWalletExistsResponse(exists);
                        response.SetSuccessful(true);
                        response.SetResponseString("Algo Wallet Existence was checked.");
                    }
                    catch(SDKException& e)
                    {
                        response.SetSuccessful(false);
                        response.SetResponseString(FString(e.what()));   
                    }
                    catch(std::exception& ex)
                    {
                        response.SetSuccessful(false);
                        response.SetResponseString(FString(ex.what()));
                    }

                    AsyncTask(ENamedThreads::GameThread, [delegate, response]()
                        {
                            delegate.ExecuteIfBound(response);
                        });

                    vertices_usable = true;
                    break;
                }
            };
        }
        
        void VerticesSDK::VerticesLoadWallet(const VerticesLoadWalletRequest& Request, const FVerticesLoadWalletDelegate& delegate)
        {           
            ret_code_t err_code = VTC_SUCCESS;
            while (1)
            {
                FScopeLock lock(&m_Mutex);

                if (vertices_usable)
                {
                    VerticesSDK::VerticesLoadWalletResponse response;
                    vertices_usable = false;
            
                    try
                    {
                        CHECK_DLL_LOAD(loaded_);
                        INIT_VERTICES(false, err_code);
                        
                        err_code = vertices_wallet_load(StringCast<ANSICHAR>(*Request.Password.GetValue()).Get());
                        checkVTCSuccess(err_code);
                        
                        UE_LOG(LogTemp, Display, TEXT("✔️ Vertices: Wallet was loaded"));
                        
                        response = response_builders::buildLoadWalletResponse();
                        response.SetSuccessful(true);
                        response.SetResponseString("Loaded Algo Wallet.");
                    }
                    catch(SDKException& e)
                    {
                        response.SetSuccessful(false);
                        response.SetResponseString(FString(e.what()));   
                    }
                    catch(std::exception& ex)
                    {
                        response.SetSuccessful(false);
                        response.SetResponseString(FString(ex.what()));
                    }

                    AsyncTask(ENamedThreads::GameThread, [delegate, response]()
                        {
                            delegate.ExecuteIfBound(response);
                        });

                    vertices_usable = true;
                    break;
                }
            };
        }
        
        void VerticesSDK::VerticesSaveWallet(const VerticesSaveWalletRequest& Request, const FVerticesSaveWalletDelegate& delegate)
        {           
            ret_code_t err_code = VTC_SUCCESS;
            while (1)
            {
                FScopeLock lock(&m_Mutex);

                if (vertices_usable)
                {
                    VerticesSDK::VerticesSaveWalletResponse response;
                    vertices_usable = false;
            
                    try
                    {
                        CHECK_DLL_LOAD(loaded_);
                        INIT_VERTICES(false, err_code);
                        
                        err_code = vertices_wallet_save(StringCast<ANSICHAR>(*(Request.Password.GetValue())).Get());
                        checkVTCSuccess(err_code);
                        
                        UE_LOG(LogTemp, Display, TEXT("✔️ Vertices: Wallet was saved"));
                        
                        response = response_builders::buildSaveWalletResponse();
                        response.SetSuccessful(true);
                        response.SetResponseString("Saved Algo Wallet.");
                    }
                    catch(SDKException& e)
                    {
                        response.SetSuccessful(false);
                        response.SetResponseString(FString(e.what()));   
                    }
                    catch(std::exception& ex)
                    {
                        response.SetSuccessful(false);
                        response.SetResponseString(FString(ex.what()));
                    }

                    AsyncTask(ENamedThreads::GameThread, [delegate, response]()
                        {
                            delegate.ExecuteIfBound(response);
                        });

                    vertices_usable = true;
                    break;
                }
            };
        }
        
        void VerticesSDK::VerticesGetMnemonicsByAccountName(const VerticesGetMnemonicsByAccountNameRequest& Request, const FVerticesGetMnemonicsByAccountNameDelegate& delegate)
        {           
            AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, [this, Request, delegate]()
            {
                ret_code_t err_code = VTC_SUCCESS;
                while (1) {
                    FScopeLock lock(&m_Mutex);

                    if (vertices_usable) {
                        VerticesSDK::VerticesGetMnemonicsByAccountNameResponse response;
                        vertices_usable = false;
                        
                        try
                        {
                            CHECK_DLL_LOAD(loaded_);
                            INIT_VERTICES(false, err_code);

                            char *mnemonic;
                            err_code = vertices_mnemonic_from_account(StringCast<ANSICHAR>(*(Request.Name.GetValue())).Get(), &mnemonic);
                            checkVTCSuccess(err_code);
                            
                            UE_LOG(LogTemp, Display, TEXT("✔️ Vertices: mnemonic phrase was fetched, %hs"), mnemonic);
                            
                            response = response_builders::buildGetMnemonicsByAccountNameResponse(mnemonic, Request.Name.GetValue());
                            response.SetSuccessful(true);
                            response.SetResponseString("Mnemonic pharse was fetched.");
                        }
                        catch(SDKException& e)
                        {
                            response.SetSuccessful(false);
                            response.SetResponseString(FString(e.what()));   
                        }
                        catch(std::exception& ex)
                        {
                            response.SetSuccessful(false);
                            response.SetResponseString(FString(ex.what()));
                        }

                        AsyncTask(ENamedThreads::GameThread, [delegate, response]()
                        {
                            delegate.ExecuteIfBound(response);
                        });

                        vertices_usable = true;
                        break;
                    }
                }
            });
        }

        void VerticesSDK::VerticesGetAllAccounts(const VerticesGetAllAccountsRequest& Request, const FVerticesGetAllAccountsDelegate& delegate)
        {           
            AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, [this, Request, delegate]()
            {
                ret_code_t err_code = VTC_SUCCESS;
                while (1) {
                    FScopeLock lock(&m_Mutex);

                    if (vertices_usable) {
                        VerticesSDK::VerticesGetAllAccountsResponse response;
                        vertices_usable = false;
                        
                        try
                        {
                            CHECK_DLL_LOAD(loaded_);
                            INIT_VERTICES(false, err_code);

                            size_t ACCOUNT_COUNT = 5;
                            s_account_t *all_accounts;
                            
                            err_code = vertices_s_accounts_all_get(&all_accounts);
                            checkVTCSuccess(err_code);
                            
                            UE_LOG(LogTemp, Display, TEXT("✔️ Vertices: all accounts were fetched"));

                            TArray<FString> Names, Addresses;
                            if(all_accounts != nullptr)
                            {
                                for (size_t i = 0; i < ACCOUNT_COUNT; ++i)
                                {
                                    if (all_accounts[i].status == ACCOUNT_ADDED)
                                    {
                                        Names.Add(all_accounts[i].name);
                                        Addresses.Add(all_accounts[i].vtc_account->public_b32);
                                    }
                                }
                            }
                            
                            response = response_builders::buildGetAllAccountsResponse(Names, Addresses);
                            response.SetSuccessful(true);
                            response.SetResponseString("All accounts were fetched.");
                        }
                        catch(SDKException& e)
                        {
                            response.SetSuccessful(false);
                            response.SetResponseString(FString(e.what()));   
                        }
                        catch(std::exception& ex)
                        {
                            response.SetSuccessful(false);
                            response.SetResponseString(FString(ex.what()));
                        }

                        AsyncTask(ENamedThreads::GameThread, [delegate, response]()
                        {
                            delegate.ExecuteIfBound(response);
                        });

                        vertices_usable = true;
                        break;
                    }
                }
            });
        }

        void VerticesSDK::VerticesRemoveAccountByName(const VerticesRemoveAccountByNameRequest& Request, const FVerticesRemoveAccountByNameDelegate& delegate)
        {           
            AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, [this, Request, delegate]()
            {
                ret_code_t err_code = VTC_SUCCESS;
                while (1) {
                    FScopeLock lock(&m_Mutex);

                    if (vertices_usable) {
                        VerticesSDK::VerticesRemoveAccountByNameResponse response;
                        vertices_usable = false;
                        
                        try
                        {
                            CHECK_DLL_LOAD(loaded_);
                            INIT_VERTICES(false, err_code);
                            
                            err_code = vertices_s_account_init(StringCast<ANSICHAR>(*(Request.Name.GetValue())).Get());
                            checkVTCSuccess(err_code);
                            
                            UE_LOG(LogTemp, Display, TEXT("✔️ Vertices: Account was removed on wallet, %hs"), StringCast<ANSICHAR>(*(Request.Name.GetValue())).Get());
                            
                            response = response_builders::buildRemoveAccountByNameResponse();
                            response.SetSuccessful(true);
                            response.SetResponseString("Account was removed.");
                        }
                        catch(SDKException& e)
                        {
                            response.SetSuccessful(false);
                            response.SetResponseString(FString(e.what()));   
                        }
                        catch(std::exception& ex)
                        {
                            response.SetSuccessful(false);
                            response.SetResponseString(FString(ex.what()));
                        }

                        AsyncTask(ENamedThreads::GameThread, [delegate, response]()
                        {
                            delegate.ExecuteIfBound(response);
                        });

                        vertices_usable = true;
                        break;
                    }
                }
            });
        }
        
        void VerticesSDK::VerticesGenerateAccountFromMnemonics(const VerticesGenerateAccountFromMnemonicsRequest& Request, const FVerticesGenerateAccountFromMnemonicsDelegate& delegate)
        {           
            AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, [this, Request, delegate]()
            {
                ret_code_t err_code = VTC_SUCCESS;
                while (1) {
                    FScopeLock lock(&m_Mutex);

                    if (vertices_usable) {
                        VerticesSDK::VerticesGenerateAccountFromMnemonicsResponse response;
                        vertices_usable = false;
                        
                        try
                        {
                            CHECK_DLL_LOAD(loaded_);
                            INIT_VERTICES(false, err_code);
                            
                            err_code = vertices_s_account_new_from_mnemonic((char *) StringCast<ANSICHAR>(*(Request.Mnemonics)).Get(), &sender_account, (char *)StringCast<ANSICHAR>(*(Request.Name)).Get());
                            checkVTCSuccess(err_code);
                            
                            UE_LOG(LogTemp, Display, TEXT("✔️ Vertices: new secure account was created, %hs"), sender_account.vtc_account->public_b32);
                            
                            response = response_builders::buildGenerateAccountFromMnemonicsResponse(sender_account.vtc_account->public_b32,sender_account.name);
                            response.SetResponseString("New account was created in Algo Wallet.");
                            response.SetSuccessful(true);
                        }
                        catch(SDKException& e)
                        {
                            response.SetSuccessful(false);
                            response.SetResponseString(FString(e.what()));   
                        }
                        catch(std::exception& ex)
                        {
                            response.SetSuccessful(false);
                            response.SetResponseString(FString(ex.what()));
                        }

                        AsyncTask(ENamedThreads::GameThread, [delegate, response]()
                        {
                            delegate.ExecuteIfBound(response);
                        });

                        vertices_usable = true;
                        break;
                    }
                }
            });
        }

        void VerticesSDK::VerticesGenerateRandomAccount(const VerticesGenerateRandomAccountRequest& Request, const FVerticesGenerateRandomAccountDelegate& delegate)
        {           
            AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, [this, Request, delegate]()
            {
                ret_code_t err_code = VTC_SUCCESS;
                while (1) {
                    FScopeLock lock(&m_Mutex);

                    if (vertices_usable) {
                        VerticesSDK::VerticesGenerateRandomAccountResponse response;
                        vertices_usable = false;
                        
                        try
                        {
                            CHECK_DLL_LOAD(loaded_);
                            INIT_VERTICES(false, err_code);
        
                            err_code = vertices_s_account_new_random(&sender_account);
                            checkVTCSuccess(err_code);
                            
                            UE_LOG(LogTemp, Display, TEXT("✔️ Vertices: new secure account was created randomly, %hs"), sender_account.vtc_account->public_b32);

                            char *mnemonic;
                            err_code = vertices_mnemonic_from_sk(sender_account.private_key, &mnemonic);
                            checkVTCSuccess(err_code);
                            
                            UE_LOG(LogTemp, Display, TEXT("✔️ Vertices: mnemonic phrase was fetched, %hs"), mnemonic);
                            
                            response = response_builders::buildGenerateRandomAccountResponse(sender_account.vtc_account->public_b32, sender_account.name, mnemonic);
                            response.SetResponseString("New account was created randomly in Algo Wallet.");
                            response.SetSuccessful(true);
                        }
                        catch(SDKException& e)
                        {
                            response.SetSuccessful(false);
                            response.SetResponseString(FString(e.what()));   
                        }
                        catch(std::exception& ex)
                        {
                            response.SetSuccessful(false);
                            response.SetResponseString(FString(ex.what()));
                        }

                        AsyncTask(ENamedThreads::GameThread, [delegate, response]()
                        {
                            delegate.ExecuteIfBound(response);
                        });

                        vertices_usable = true;
                        break;
                    }
                }
            });
        }

        void VerticesSDK::VerticesGetAddrBalance(const VerticesGetAddrBalanceRequest& Request, const FVerticesGetAddrBalanceDelegate& delegate)
        {            
            AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, [this, Request, delegate]()
            {
                ret_code_t err_code = VTC_SUCCESS;
                account_info_t *test_account;
                while (1) {
                    FScopeLock lock(&m_Mutex);
                    
                    if (vertices_usable) {
                        VerticesSDK::VerticesGetAddrBalanceResponse response;
                        vertices_usable = false;

                        try
                        {
                            CHECK_DLL_LOAD(loaded_);
                            INIT_VERTICES(false, err_code);
                            
                            // validation Request
                            auto auto_address = StringCast<ANSICHAR>(*(Request.Address.GetValue()));
                            if ( auto_address.Length() == 0 )
                            {
                                err_code = VTC_ERROR_INVALID_PARAM;
                                checkVTCSuccess(const_cast<char*>("Address from Request was invalid."), err_code);   
                            }
                            
                            test_account = nullptr;
                            
                            err_code = vertices_account_new_from_b32((char*)auto_address.Get(), &test_account);
                            checkVTCSuccess( err_code);

                            UE_LOG(LogTemp, Display, TEXT("✔️ Vertices: Fetched Balance, %d"), test_account->amount);
                            
                            response = response_builders::buildGetAddrBalanceResponse(Request.Address.GetValue(), test_account->amount);
                            response.SetSuccessful(true);
                            
                            err_code = vertices_account_free(test_account);
                            checkVTCSuccess(const_cast<char*>("test_account can't be deleted."), err_code);
                        }
                        catch (SDKException& e)
                        {
                            response.SetSuccessful(false);
                            response.SetResponseString(FString(e.what()));
                        }
                        catch (std::exception& ex)
                        {   
                            response.SetSuccessful(false);
                            response.SetResponseString(FString(ex.what()));
                        }

                        // after build response, execute delegate to run callback with response
                        AsyncTask(ENamedThreads::GameThread, [delegate, response]()
                        {
                            delegate.ExecuteIfBound(response);
                        });

                        vertices_usable = true;
                        break;
                    }
                    
                }
            });
        }

        // transactions
        void VerticesSDK::VerticesSendPayTx(const VerticesSendPayTxRequest& Request, const FVerticesSendPayTxDelegate& delegate)
        {
            AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, [this, Request, delegate]()
            {
                ret_code_t err_code = VTC_SUCCESS;
                VerticesSDK::VerticesSendPayTxResponse response;
                account_info_t *receiver_account;
                while (1) {
                    FScopeLock lock(&m_Mutex);

                    if (vertices_usable) {
                        vertices_usable = false;

                        try
                        {
                            CHECK_DLL_LOAD(loaded_);
                            INIT_VERTICES(false, err_code);

                            err_code = vertices_s_account_get_by_name(&sender_account, StringCast<ANSICHAR>((*Request.MainAccountName.GetValue())).Get());
                            checkVTCSuccess((char *)"Main account can't be fetched.", err_code);
                            
                            // validation Request
                            char* notes;

                            if(Request.Notes->Len() == 0)
                            {
                                notes = (char *) malloc(strlen("Payment Transaction") + 1);
                                strcpy_s(notes,  strlen("Payment Transaction") + 1,"Payment Transaction");
                            } else
                            {
                                auto auto_notes = StringCast<ANSICHAR>(*(Request.Notes.GetValue()));
                                notes = const_cast<char*>(auto_notes.Get());
                            }
                            
                            if ( Request.ReceiverAddress.GetValue().Len() != PUBLIC_ADDRESS_LENGTH )
                            {
                                err_code = VTC_ERROR_INVALID_ADDR;
                                checkVTCSuccess((char *)"Please input address with correct length.", err_code);
                            }
                            
                            if (sender_account.vtc_account->amount < Request.Amount.GetValue())
                            {
                                FFormatNamedArguments Arguments;
                                Arguments.Add(TEXT("Address"), FText::FromString(sender_account.vtc_account->public_b32));
                                FMessageDialog::Open(EAppMsgType::Ok, FText::Format(LOCTEXT("Warning", "To send algos to {Address}, you should add more algos"), Arguments));
                                UE_LOG(LogTemp, Warning, TEXT("👉 Go to https://bank.testnet.algorand.network/, dispense Algos to: %s"), *FString(sender_account.vtc_account->public_b32));
                                
                                err_code = VTC_ERROR_ASSERT_FAILS;
                                checkVTCSuccess((char *)"Amount available on account is too low to pass a transaction, consider adding Algos", err_code);
                            }
                            
                            err_code = vertices_account_new_from_b32((char*)TCHAR_TO_ANSI(*(Request.ReceiverAddress.GetValue())), &receiver_account);
                            checkVTCSuccess((char *)"account info can't be fetched.", err_code);
                            UE_LOG(LogTemp, Display, TEXT("🔥 Vertices: Fetched Balance of receiver, %d"), receiver_account->amount);
                            
                            err_code =
                                vertices_transaction_pay_new(sender_account.vtc_account,
                                    (char *)receiver_account->public_key /* or ACCOUNT_RECEIVER */,
                                    (uint64_t)Request.Amount.GetValue(),
                                    notes);
                            checkVTCSuccess((char *)"Payment Tx was failed", err_code);
                            
                            unsigned char* txID = new unsigned char[TRANSACTION_HASH_STR_MAX_LENGTH];
                            
                            size_t queue_size = 1;
                            PROCESS_QUEUE_WHILE_SUCCESS(queue_size, err_code, txID);
                            
                            char *buf = nullptr;
                            if(err_code == VTC_ERROR_HTTP_BASE && vertices_provider_buf_get(&buf) == VTC_SUCCESS)
                            {
                                checkVTCSuccess(buf, err_code);
                            }
                            else
                            {
                                checkVTCSuccess((char *)"TX Process was failed.", err_code);                                 
                            }
                            
                            UE_LOG(LogTemp, Display, TEXT("✔️ Vertices: Payment TX ID, %hs"), (const char*)txID);
                            
                            err_code = vertices_account_free(receiver_account);
                            checkVTCSuccess((char *)"receiver account can't be deleted.", err_code);

                            free(notes);
                            
                            response = response_builders::buildSendPayTxResponse(FString(UTF8_TO_TCHAR(txID)));
                            response.SetSuccessful(true);
                            response.SetResponseString((char *)txID);
                        }
                        catch(SDKException& e)
                        {
                            response.SetSuccessful(false);
                            response.SetResponseString(FString(e.what()));
                        }
                        catch(std::exception& ex)
                        {   
                            response.SetSuccessful(false);
                            response.SetResponseString(FString(ex.what()));
                        }

                        // after build response, execute delegate to run callback with response
                        AsyncTask(ENamedThreads::GameThread, [delegate, response]()
                        {
                            delegate.ExecuteIfBound(response);
                        });

                        vertices_usable = true;
                        break;
                    }
                }
            });
        }
        
        void VerticesSDK::VerticesSendAcfgTx(const VerticesSendAcfgTxRequest& Request, const FVerticesSendAcfgTxDelegate& delegate)
        {
            AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, [this, Request, delegate]()
            {
                ret_code_t err_code = VTC_SUCCESS;
                account_info_t *M_Account = {0}, *R_Account = {0}, *F_Account = {0}, *C_Account = {0};  // 5 accounts for acfg
                while (1) {
                    FScopeLock lock(&m_Mutex);

                    if (vertices_usable) {
                        VerticesSDK::VerticesSendAcfgTxResponse response;
                        vertices_usable = false;

                        try
                        {
                            CHECK_DLL_LOAD(loaded_);
                            INIT_VERTICES(false, err_code);

                            err_code = vertices_s_account_get_by_name(&sender_account, StringCast<ANSICHAR>((*Request.MainAccountName.GetValue())).Get());
                            checkVTCSuccess((char *)"Main account can't be fetched.", err_code);

                            err_code = vertices_s_account_update(&sender_account);
                            checkVTCSuccess((char *)"Main account can't be updated.", err_code);
                            
                            // validation Request
                            if((uint64_t)Request.Total.GetValue() == 0)
                            {
                                err_code = VTC_ERROR_INVALID_ADDR;
                                checkVTCSuccess((char *)"Please input total count of token.", err_code);
                            }

                            if((uint64_t)Request.Decimals.GetValue() == 0)
                            {
                                err_code = VTC_ERROR_INVALID_ADDR;
                                checkVTCSuccess((char *)"Please input token decimal.", err_code);
                            }

                            if(Request.UnitName.GetValue().Len() > 8)
                            {
                                err_code = VTC_ERROR_INVALID_PARAM;
                                checkVTCSuccess((char *)"Transaction Asset Unit Name is too big.", err_code);
                            }
                            
                            auto auto_unit_name = StringCast<ANSICHAR>(*(Request.UnitName.GetValue())).Get();        // unit name

                            if(FCStringAnsi::Strlen(auto_unit_name) == 0)
                            {
                                err_code = VTC_ERROR_INVALID_ADDR;
                                checkVTCSuccess((char *)"Please input Unit Name.", err_code);
                            }

                            auto auto_asset_name = StringCast<ANSICHAR>(*(Request.AssetName.GetValue())).Get();        // asset name

                            if(FCStringAnsi::Strlen(auto_asset_name) == 0)
                            {
                                err_code = VTC_ERROR_INVALID_ADDR;
                                checkVTCSuccess((char *)"Please input Asset Name.", err_code);
                            }

                            auto auto_url = StringCast<ANSICHAR>(*(Request.Url.GetValue())).Get();        // url

                            auto auto_notes = StringCast<ANSICHAR>(*(Request.Notes.GetValue())).Get();        // notes
                            
                            if(FCStringAnsi::Strlen(auto_notes) == 0)
                            {
                                auto_notes = "Asset Config Transaction";
                            }

                            // todo Let's set Creator address to sender_account pub_b32

                            if ( Request.Manager.GetValue().Len() != PUBLIC_ADDRESS_LENGTH)
                            {
                                err_code = VTC_ERROR_INVALID_ADDR;
                                checkVTCSuccess((char *)"Please input manager address with correct length.", err_code);
                            }
                            
                            if ( Request.Reserve.GetValue().Len() != PUBLIC_ADDRESS_LENGTH && Request.Reserve.GetValue().Len() != 0)
                            {
                                err_code = VTC_ERROR_INVALID_ADDR;
                                checkVTCSuccess((char *)"Please input reserve address with correct length.", err_code);
                            }

                            if ( Request.Freeze.GetValue().Len() != PUBLIC_ADDRESS_LENGTH && Request.Freeze.GetValue().Len() != 0)
                            {
                                err_code = VTC_ERROR_INVALID_ADDR;
                                checkVTCSuccess((char *)"Please input freeze address with correct length.", err_code);
                            }

                            if ( Request.Clawback.GetValue().Len() != PUBLIC_ADDRESS_LENGTH && Request.Clawback.GetValue().Len() != 0 )
                            {
                                err_code = VTC_ERROR_INVALID_ADDR;
                                checkVTCSuccess((char *)"Please input clawback address with correct length.", err_code);
                            }

                            if (sender_account.vtc_account->amount < 1000) {
                                FFormatNamedArguments Arguments;
                                Arguments.Add(TEXT("Address"), FText::FromString(sender_account.vtc_account->public_b32));
                                FMessageDialog::Open(EAppMsgType::Ok, FText::Format(LOCTEXT("Warning", "To create a new asset or update, dispense Algos to: {Address}"), Arguments));
                            
                                UE_LOG(LogTemp, Warning, TEXT("👉 Go to https://bank.testnet.algorand.network/, dispense Algos to: %s"), *FString(sender_account.vtc_account->public_b32));
                                
                                err_code = VTC_ERROR_ASSERT_FAILS;
                                checkVTCSuccess(const_cast<char*>("Amount available on account is too low to pass a transaction, consider adding Algos"), err_code);
                            }

                            auto auto_manager = StringCast<ANSICHAR>(*(Request.Manager.GetValue())).Get();
                            
                            if (FCStringAnsi::Strlen(auto_manager) == 0)
                            {
                                auto_manager = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAY5HFKQ";
                            }
                            
                            err_code = vertices_account_new_from_b32((char *)auto_manager, &M_Account);
                            checkVTCSuccess((char *)"account info can't be fetched.", err_code);
                            UE_LOG(LogTemp, Display, TEXT("🔥 Vertices: Fetched Balance of Manager, %d"), M_Account->amount);

                            auto auto_reserve = StringCast<ANSICHAR>(*(Request.Reserve.GetValue())).Get();
                            
                            if (FCStringAnsi::Strlen(auto_reserve) == 0)
                            {
                                auto_reserve = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAY5HFKQ";
                            }
                            
                            err_code = vertices_account_new_from_b32((char *)auto_reserve, &R_Account);
                            checkVTCSuccess((char *)"account info can't be fetched.", err_code);
                            UE_LOG(LogTemp, Display, TEXT("🔥 Vertices: Fetched Balance of Reserve, %d"), R_Account->amount);
                            
                            
                            auto auto_freeze = StringCast<ANSICHAR>(*(Request.Freeze.GetValue())).Get();
                            
                            if (FCStringAnsi::Strlen(auto_freeze) == 0)
                            {
                                auto_freeze = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAY5HFKQ";
                            }
                            
                            err_code = vertices_account_new_from_b32((char *)auto_freeze, &F_Account);
                            checkVTCSuccess((char *)"account info can't be fetched.", err_code);
                            UE_LOG(LogTemp, Display, TEXT("🔥 Vertices: Fetched Balance of Freeze, %d"), F_Account->amount);
                            
                            auto auto_clawback = StringCast<ANSICHAR>(*(Request.Clawback.GetValue())).Get();
                            
                            if (FCStringAnsi::Strlen(auto_clawback) == 0)
                            {
                                auto_clawback = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAY5HFKQ";
                            }
                            
                            err_code = vertices_account_new_from_b32((char *)auto_clawback, &C_Account);
                            checkVTCSuccess((char *)"account info can't be fetched.", err_code);
                            UE_LOG(LogTemp, Display, TEXT("🔥 Vertices: Fetched Balance of Clawback, %d"), C_Account->amount);
                            
                            err_code =
                                vertices_transaction_asset_cfg(sender_account.vtc_account,
                                    (char *)M_Account->public_key /* or ACCOUNT_MANAGER */,
                                    (char *)R_Account->public_key /* or ACCOUNT_RESERVE */,
                                    (char *)F_Account->public_key /* or ACCOUNT_FREEZE */,
                                    (char *)C_Account->public_key /* or ACCOUNT_CLAWBACK */,
                                    (uint64_t)Request.AssetId.GetValue(),
                                    (uint64_t)Request.Total.GetValue(),
                                    (uint64_t)Request.Decimals.GetValue(),
                                    Request.IsFrozen.GetValue(),
                                    (char *)auto_unit_name,
                                    (char *)auto_asset_name,
                                    (char *)auto_url,
                                    (char *)auto_notes);
                            checkVTCSuccess((char *)"Acfg TX was failed.", err_code);
                            
                            unsigned char* txID = nullptr;
                            txID = new unsigned char[TRANSACTION_HASH_STR_MAX_LENGTH];
                            
                            size_t queue_size = 1;
                            while (queue_size && err_code == VTC_SUCCESS) {
                                err_code = vertices_event_process(&queue_size, txID);
                            }
                            
                            char *buf = nullptr;
                            if(err_code == VTC_ERROR_HTTP_BASE && vertices_provider_buf_get(&buf) == VTC_SUCCESS)
                            {
                            checkVTCSuccess(buf, err_code);
                            }
                            else
                            {
                                checkVTCSuccess((char *)"TX Process was failed.", err_code);                                 
                            }
                            
                            UE_LOG(LogTemp, Warning, TEXT("✔️ Vertices: Asset Config TX ID, %hs"), (const char*)txID);
                            
                            vertices_account_free(M_Account);
                            vertices_account_free(R_Account);
                            vertices_account_free(F_Account);
                            vertices_account_free(C_Account);
                            
                            // UE_LOG(LogTemp, Warning, TEXT("Asset Config TX ASSET TX Success, %hs"), (const char*)UTF8_TO_TCHAR(txID));
                            //
                            // uint64 asset_id;
                            // do{
                            //     err_code = vertices_asset_id_get(txID, &asset_id);
                            //     UE_LOG(LogTemp, Warning, TEXT("Asset Config TX ASSET ID Success, %llu"), asset_id);
                            // }
                            // while (err_code != VTC_SUCCESS);
                            //
                            // UE_LOG(LogTemp, Warning, TEXT("Asset Config TX ASSET ID Success, %llu"), asset_id);
                            
                            response = response_builders::buildSendAcfgTxResponse(FString(UTF8_TO_TCHAR(txID)), 1001/*asset_id*/);
                            response.SetSuccessful(true);
                            
                            // free(txID);
                        }
                        catch(SDKException& e)
                        {
                            response.SetSuccessful(false);
                            response.SetResponseString(FString(e.what()));
                        }
                        catch(std::exception& ex)
                        {   
                            response.SetSuccessful(false);
                            response.SetResponseString(FString(ex.what()));
                        }

                        // after build response, execute delegate to run callback with response
                        AsyncTask(ENamedThreads::GameThread, [delegate, response]()
                        {
                            delegate.ExecuteIfBound(response);
                        });

                        vertices_usable = true;
                        break;
                    }
                }
            });
        }

        void VerticesSDK::VerticesSendAxferTx(const VerticesSendAxferTxRequest& Request, const FVerticesSendAxferTxDelegate& delegate)
        {
            AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, [this, Request, delegate]()
            {
                ret_code_t err_code = VTC_SUCCESS;
                account_info_t *arcv_account = {0};
                while (1) {
                    FScopeLock lock(&m_Mutex);
                
                    if (vertices_usable) {
                        VerticesSDK::VerticesSendAxferTxResponse response;
                        vertices_usable = false;
                
                        try
                        {
                            CHECK_DLL_LOAD(loaded_);
                            INIT_VERTICES(false, err_code);

                            err_code = vertices_s_account_get_by_name(&sender_account, StringCast<ANSICHAR>((*Request.MainAccountName.GetValue())).Get());
                            checkVTCSuccess((char *)"Main account can't be fetched.", err_code);

                            err_code = vertices_s_account_update(&sender_account);
                            checkVTCSuccess((char *)"Main account can't be updated.", err_code);
                            
                            // validation Request
                            auto auto_notes = StringCast<ANSICHAR>(*(Request.Notes.GetValue())).Get();
                
                            if(FCStringAnsi::Strlen(auto_notes) == 0)
                            {
                                auto_notes = "Asset Transfer Transaction";
                            }
                
                            if (sender_account.vtc_account->amount < 1000) {
                                FFormatNamedArguments Arguments;
                                Arguments.Add(TEXT("Address"), FText::FromString(sender_account.vtc_account->public_b32));
                                FMessageDialog::Open(EAppMsgType::Ok, FText::Format(LOCTEXT("Warning", "Go to https://bank.testnet.algorand.network/, dispense Algos to: {Address}"), Arguments));
                
                                UE_LOG(LogTemp, Warning, TEXT("👉 Go to https://bank.testnet.algorand.network/, dispense Algos to: %s"), *FString(sender_account.vtc_account->public_b32));
                                
                                err_code = VTC_ERROR_ASSERT_FAILS;
                                checkVTCSuccess((char *)"Amount available on account is too low to pass a transaction, consider adding Algos", err_code);
                            }
                            
                            const FString& arcv = Request.ReceiverAddress.GetValue();

                            // validation Request
                            auto auto_receiver = StringCast<ANSICHAR>(*(Request.ReceiverAddress.GetValue())).Get();
                            
                            if ( FCStringAnsi::Strlen(auto_receiver) != PUBLIC_ADDRESS_LENGTH )
                            {
                                err_code = VTC_ERROR_INVALID_ADDR;
                                checkVTCSuccess((char *)"Please input address with correct length.", err_code);
                            }
                            
                            err_code = vertices_account_new_from_b32((char*)auto_receiver, &arcv_account);
                            checkVTCSuccess((char *)"account info can't be fetched.", err_code);
                            UE_LOG(LogTemp, Display, TEXT("🔥 Vertices: Fetched Balance of receiver, %d"), arcv_account->amount);
                            
                             err_code =
                                 vertices_transaction_asset_xfer(sender_account.vtc_account,
                                     (char *)sender_account.vtc_account->public_key /* or ACCOUNT_SENDER */,
                                     (char *)arcv_account->public_key /* or ACCOUNT_RECEIVER */,
                                     (uint64_t)Request.Asset_ID.GetValue(),
                                     (double)Request.Amount.GetValue(),
                                     (char *)auto_notes);
                             
                             checkVTCSuccess((char *)"Asset Transfer Tx was failed", err_code);
                            
                             unsigned char* txID = nullptr;
                             txID = new unsigned char[TRANSACTION_HASH_STR_MAX_LENGTH];
                            
                             size_t queue_size = 1;
                             while (queue_size && err_code == VTC_SUCCESS) {
                                 err_code = vertices_event_process(&queue_size, txID);
                             }

                             char *buf = nullptr;
                             if(err_code == VTC_ERROR_HTTP_BASE && vertices_provider_buf_get(&buf) == VTC_SUCCESS)
                             {
                                checkVTCSuccess(buf, err_code);
                             }
                             else
                             {
                                 checkVTCSuccess((char *)"TX Process was failed.", err_code);                                 
                             }
                            
                             UE_LOG(LogTemp, Display, TEXT("✔️ Vertices: Asset Transfer TX ID, %hs"), (const char*)txID);
                             
                             response = response_builders::buildSendAxferTxResponse(FString(UTF8_TO_TCHAR(txID)));
                             response.SetSuccessful(true);
                
                            // free(txID);
                        }
                        catch(SDKException& e)
                        {
                            response.SetSuccessful(false);
                            response.SetResponseString(e.what());
                        }
                        catch(std::exception& ex)
                        {
                            response.SetSuccessful(false);
                            response.SetResponseString(ex.what());
                        }
                
                        // after build response, execute delegate to run callback with response
                        AsyncTask(ENamedThreads::GameThread, [delegate, response]()
                        {
                            delegate.ExecuteIfBound(response);
                        });
                
                        vertices_usable = true;
                        break;
                    }
                }
            });
        }
        
        void VerticesSDK::VerticesSendApplCallTx(const VerticesSendApplCallTxRequest& Request, const FVerticesSendApplCallTxDelegate& delegate)
        {
            AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, [this, Request, delegate]()
            {
                ret_code_t err_code = VTC_SUCCESS;

                while (1) {
                    FScopeLock lock(&m_Mutex);

                    if (vertices_usable) {
                        VerticesSDK::VerticesSendApplCallTxResponse response;
                        vertices_usable = false;

                        try
                        {
                            CHECK_DLL_LOAD(loaded_);
                            INIT_VERTICES(false, err_code);

                            err_code = vertices_s_account_get_by_name(&sender_account, StringCast<ANSICHAR>((*Request.MainAccountName.GetValue())).Get());
                            checkVTCSuccess((char *)"Main account can't be fetched.", err_code);
                            
                            // validation request   
                            if ( Request.App_ID.GetValue() == 0 )
                            {
                                err_code = VTC_ERROR_INVALID_ADDR;
                                checkVTCSuccess((char *)"Please input correct app ID.", err_code);
                            }

                            // err_code = convert_Account_Vertices();
                            // checkVTCSuccess((char *)"Vertices convert_Account_Vertices error", err_code);

                            if (sender_account.vtc_account->amount < 1000) {
                                FFormatNamedArguments Arguments;
                                Arguments.Add(TEXT("Address"), FText::FromString(sender_account.vtc_account->public_b32));
                                FMessageDialog::Open(EAppMsgType::Ok, FText::Format(LOCTEXT("Warning", "Go to https://bank.testnet.algorand.network/, dispense Algos to: {Address}"), Arguments));
                            
                                UE_LOG(LogTemp, Warning, TEXT("👉 Go to https://bank.testnet.algorand.network/, dispense Algos to: %s"), *FString(sender_account.vtc_account->public_b32));
                                
                                err_code = VTC_ERROR_ASSERT_FAILS;
                                checkVTCSuccess((char *)"Amount available on account is too low to pass a transaction, consider adding Algos", err_code);
                            }
                            
                            // get application information
                            UE_LOG(LogTemp, Warning, TEXT("Application %llu, global states"), Request.App_ID.GetValue());
                            
                            app_values_t app_kv = { 0 };
                            err_code = vertices_application_get(Request.App_ID.GetValue(), &app_kv);
                            checkVTCSuccess(const_cast<char*>("vertices_application_get error occured"), err_code);

                            err_code = VTC_ERROR_INVALID_STATE;
                            throw SDKException(const_cast<char*>("noop_logs_get are not registered"), err_code);
                            
                            /// send application call
                            // set app args as byte_slice type
//                             app_values_t kv = {0};
//                             kv.count = Request.app_args.Num();
//                             if(kv.count > 0)
//                             {
//                                 uint8_t arg_index = 0;
//                                 for(; arg_index < kv.count; arg_index++)
//                                 {
//                                     kv.values[arg_index].type = VALUE_TYPE_BYTESLICE;
//                                     // TODO transaction_app_call
//                                     if(arg_index == 0)
//                                         memcpy(kv.values[arg_index].value_slice, Request.app_args[arg_index].GetData(), APPS_NOOP_TX_SLICE_MAX_SIZE);
//                                     else
//                                         memcpy(kv.values[arg_index].value_slice, Request.app_args[arg_index].GetData(), APPS_KV_SLICE_MAX_SIZE);
//                                 }
//                             }
//                             
//                             err_code = vertices_transaction_app_call(sender_account.vtc_account, 301624623, &kv);
//                             // err_code = vertices_transaction_app_call(sender_account.vtc_account, Request.app_ID.GetValue(), &kv);
//                             checkVTCSuccess((char *)"vertices_transaction_app_call error occured", err_code);
//
//                             unsigned char* txID = nullptr;
//                             txID = new unsigned char[TRANSACTION_HASH_STR_MAX_LENGTH];
//
//                             size_t queue_size = 1;
//                             while (queue_size && err_code == VTC_SUCCESS) {
//                                 err_code = vertices_event_process(&queue_size, txID);
//                             }
//
//                             checkVTCSuccess((char *)"vertices_event_process error occured", err_code);
//
//                             //free(txID);
//
//                             InitVertices(err_code);
//                             checkVTCSuccess((char *)"When reiniting vertices network, an error occured", err_code);
//                             
//                             unsigned char logs[MSG_LOGS_MAX_SIZE];
//                             err_code = vertices_noop_logs_get((unsigned char *) txID, logs);
//
//                             // checkVTCSuccess((char *)"When getting logs after NoOp Tx, an error occured", err_code);
//                             // UE_LOG(LogTemp, Warning, TEXT("👉 Haha This is NoOp Tx Logs Status: %llu"), (long long unsigned) err_code);
//                             // UE_LOG(LogTemp, Warning, TEXT("👉 Haha This is logs: %s"), (const char *) logs);
//                             /*unsigned char * u8_logs = new unsigned char[strlen((const char*)logs)];
//                             memset(u8_logs, 0, strlen((const char*)logs));
//                             len = strlen((const char*)logs);
//                             b64_decode((const char *)logs, strlen((const char*)logs), (char *)u8_logs, &len);
//
//                             uint64_t app_result = 0;
//                             for(int i = 0; i < 8; i++) {
//                                 app_result += u8_logs[i + 4] * pow(256, 8 - i - 1);
//                             }
//                             UE_LOG(LogTemp, Warning, TEXT("👉 Haha This is logs: %llu"), (unsigned long long int)app_result);*/
//
//                             err_code = vertices_account_free(sender_account.vtc_account);
//                             checkVTCSuccess((char *)"vertices_account_free error occured", err_code);
//                             
//                             response = response_builders::buildApplicationCallTransactionResponse(FString(UTF8_TO_TCHAR(txID)), FString(UTF8_TO_TCHAR(logs)));
//                             response.SetSuccessful(true);
                        }
                        catch(SDKException& e)
                        {
                            response.SetSuccessful(false);
                            response.SetResponseString(FString(e.what()));
                        }
                        catch(std::exception& ex)
                        {   
                            response.SetSuccessful(false);
                            response.SetResponseString(FString(ex.what()));
                        }

                        // after build response, execute delegate to run callback with response
                        AsyncTask(ENamedThreads::GameThread, [delegate, response]()
                        {
                            delegate.ExecuteIfBound(response);
                        });

                        vertices_usable = true;
                        break;
                    }
                }
            });
        }

        // Vertices Setters
        void VerticesSDK::setIndexerRpc(const FString& indexerRpc) {
            myIndexerRpc = indexerRpc;
        }
        
        void VerticesSDK::setAlgodRpc(const FString& algodRpc) {
            myAlgodRpc = algodRpc;
        }

        void VerticesSDK::setAlgoPort(const int& algoPort) {
            myAlgoPort = algoPort;
        }

        void VerticesSDK::setAlgoTokenHeader(const FString& algoTokenHeader) {
            myAlgoTokenHeader = algoTokenHeader;
        }

        void VerticesSDK::setHTTPCURLs() 
        {
            try
            {
                if(!loaded_) return;

                // Get the base directory of this plugin
                FString BaseDir = IPluginManager::Get().FindPlugin("Algorand")->GetBaseDir();

                // Add on the relative location of the third party dll and load it
                FString VerticesPath, SodiumPath;

                VerticesPath = FPaths::Combine(*BaseDir, TEXT("Source/ThirdParty/AlgorandLibrary/x64/Release/vertices.dll"));
                SodiumPath = FPaths::Combine(*BaseDir, TEXT("Source/Vertices/Libs/libsodium.dll"));
                
                VerticesHandle = LoadLibrary(*VerticesPath);
                SodiumHandle = LoadLibrary(*SodiumPath);
                
                set_http_init(&http_init);
                set_http_get(&http_get);
                set_http_post(&http_post);
                set_http_close(&http_close);
            }
            catch (std::exception &ex)
            {
                UE_LOG(LogTemp, Display, TEXT("Failed to load C-Vertices-sdk.dll & libsodium.dll. Error: %s"), *FString(ex.what()));
            }
        }

        // Vertices Exception Process
        void VerticesSDK::checkVTCSuccess(ret_code_t& err_code)
        {
            if(err_code != VTC_SUCCESS)
                throw SDKException(err_code);
        }

        void VerticesSDK::checkVTCSuccess(char* Msg, ret_code_t& err_code)
        {
            if(err_code != VTC_SUCCESS)
                throw SDKException(Msg, err_code);
        }
    }
}

#undef PUBLIC_ADDRESS_LENGTH
#undef LOCTEXT_NAMESPACE
