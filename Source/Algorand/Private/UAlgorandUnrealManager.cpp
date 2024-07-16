// Copyright 2022, Wisdom Labs. All Rights Reserved

#include "UAlgorandUnrealManager.h"
#include "TransactionBuilder.h"
#include "TransactionBuilderFactory.h"
#include "RequestBuilders.h"
#include <functional>

#include "VerticesApiOperations.h"
#include "Misc/MessageDialog.h"

DECLARE_LOG_CATEGORY_EXTERN(LogMyAwesomeGame, Log, All);

#define LOCTEXT_NAMESPACE "FAlgorandManagerModule"

namespace {
    using API = algorand::api::UnrealApi;
    using Vertices = algorand::vertices::VerticesSDK;
}

// create vertices_ , transactionBuilder_ , unrealApi_ and load payment address 
// Sets default values
UAlgorandUnrealManager::UAlgorandUnrealManager()
    :myAlgodRpc("https://testnet-algorand.api.purestake.io/ps2") , myAlgodPort(0) , myAlgodTokenHeader("x-api-key:bLcs4F2SyGY0InF9M6Vl9piFTIZ8Ww281OjKXyE1"), myIndexerRpc("https://testnet-algorand.api.purestake.io/idx2"), myIndexerPort(0), myIndexerTokenHeader("x-api-key:bLcs4F2SyGY0InF9M6Vl9piFTIZ8Ww281OjKXyE1")
{
    FString address;
    // create instance of Vertices library
    vertices_ = MakeShared<algorand::vertices::VerticesSDK>();

    transactionBuilder_ = createTransactionBuilder("");       
    // create instance of unreal api library
    unrealApi_ = MakeShared<algorand::api::UnrealApi>(vertices_);

    setAlgodRpcInfo(myAlgodRpc, myAlgodPort, myAlgodTokenHeader);
    setIndexerRpcInfo(myIndexerRpc, myIndexerPort, myIndexerTokenHeader);        
}

// create instance of algorand manager using blueprint
UAlgorandUnrealManager* UAlgorandUnrealManager::createInstanceWithParams(const FString& algodRpc,
                                                                         const FUInt64& algodPort,
                                                                         const FString& algodTokenHeader,
                                                                         const FString& indexerRpc,
                                                                         const FUInt64& indexerPort,
                                                                         const FString& indexerTokenHeader,
                                                                         UObject* outer)
{
    UAlgorandUnrealManager* manager = NewObject<UAlgorandUnrealManager>(outer);

    manager->setAlgodRpcInfo(algodRpc, algodPort, algodTokenHeader);
    manager->setIndexerRpcInfo(indexerRpc, indexerPort, indexerTokenHeader);
    return manager;
}

// create instance of algorand manager using blueprint
UAlgorandUnrealManager* UAlgorandUnrealManager::createInstance(UObject* outer)
{
    UAlgorandUnrealManager* manager = NewObject<UAlgorandUnrealManager>(outer);
        
    return manager;
}

/// set rpc info from algorand manager to vertices instance
void UAlgorandUnrealManager::setAlgodRpcInfo(const FString& algodRpc, const FUInt64& algodPort, const FString& algodTokenHeader)
{
    myAlgodRpc = algodRpc;
    myAlgodPort = algodPort;
    myAlgodTokenHeader = algodTokenHeader;
    
    vertices_->setAlgodRpc(myAlgodRpc);
    vertices_->setAlgoPort(myAlgodPort);
    vertices_->setAlgoTokenHeader(myAlgodTokenHeader);

    unrealApi_->setAlgodRpcInfo(myAlgodRpc, myAlgodPort, myAlgodTokenHeader);
}

/// get algod rpc info from algorand manager
FRPCInfo UAlgorandUnrealManager::getAlgodRpcInfo()
{
    FRPCInfo rpc_info;
    rpc_info.url = myAlgodRpc;
    rpc_info.port = myAlgodPort;
    rpc_info.token = myAlgodTokenHeader;

    return rpc_info;
}

/// set rpc info from algorand manager to unrealapi instance
void UAlgorandUnrealManager::setIndexerRpcInfo(const FString& indexerRpc, const FUInt64& indexerPort, const FString& indexerTokenHeader)
{
    myIndexerRpc = indexerRpc;
    myIndexerPort = indexerPort;
    myIndexerTokenHeader = indexerTokenHeader;

    vertices_->setIndexerRpc(myIndexerRpc);
    
    unrealApi_->setIndexerRpcInfo(myIndexerRpc,myIndexerPort, myIndexerTokenHeader);
}

/// get indexer rpc info from algorand manager
FRPCInfo UAlgorandUnrealManager::getIndexerRpcInfo()
{
    FRPCInfo rpc_info;
    rpc_info.url = myIndexerRpc;
    rpc_info.port = myIndexerPort;
    rpc_info.token = myIndexerTokenHeader;

    return rpc_info;
}

/// get algod rpc net info 
FString UAlgorandUnrealManager::getAlgodRpcNet()
{
    if(myAlgodRpc.Contains("mainnet"))
        return "MainNet";
    if(myAlgodRpc.Contains("testnet"))
        return "TestNet";
    if(myAlgodRpc.Contains("devnet"))
        return "DevNet";
    return "LocalNet";
}

/// get indexer rpc net info 
FString UAlgorandUnrealManager::getIndexerRpcNet()
{
    if(myIndexerRpc.Contains("mainnet"))
        return "MainNet";
    if(myIndexerRpc.Contains("testnet"))
        return "TestNet";
    if(myIndexerRpc.Contains("devnet"))
        return "DevNet";
    return "LocalNet";
}

FString UAlgorandUnrealManager::getAddress()
{
    return transactionBuilder_->paymentAddress();
}

void UAlgorandUnrealManager::setAddress(const FString& address)
{
    transactionBuilder_->setPaymentAddress(address);
}

/**
 * @brief create its context to send the request to unreal api for initialize new wallet
 */
 void UAlgorandUnrealManager::initWallet()
{
    this->requestContextManager_
        .createContext<API::FAlgorandAPIInitWalletDelegate,
        Vertices::VerticesInitWalletRequest>(
            request_builders::buildInitWalletRequest(),
            std::bind(&API::AlgorandAPIInitWallet, unrealApi_.Get(),
                std::placeholders::_1, std::placeholders::_2),
            std::bind(&UAlgorandUnrealManager::OnInitWalletCompleteCallback, this , std::placeholders::_1)
        );
}

/**
 * @brief get response from unreal api after initialize new wallet and broadcast the result to binded functions
 */
void UAlgorandUnrealManager::OnInitWalletCompleteCallback(const Vertices::VerticesInitWalletResponse& response) {
    if (response.IsSuccessful()) {
        UE_LOG(LogTemp, Display, TEXT("Wallet was inited correctly.  Output : %s"), *response.GetResponseString());
        InitWalletCallback.Broadcast(EResultType::InitWallet, FResultBoolean::ToResultBoolean(true));
    }
    else {
        if (ErrorDelegateCallback.IsBound())
        {
            UE_LOG(LogTemp, Error, TEXT("Wallet wasn't initialized.  Error : %s"), *response.GetResponseString());
            ErrorDelegateCallback.Broadcast(FError("Wallet wasn't initialized"));
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("ErrorDelegateCallback is not bound."));
        }
    }
}

/**
 * @brief create its context to send the request to unreal api for loading wallet
 */
void UAlgorandUnrealManager::loadWallet(const FString& mnemonics)
{
    this->requestContextManager_
        .createContext<API::FAlgorandAPILoadWalletDelegate,
        Vertices::VerticesLoadWalletRequest>(
            request_builders::buildLoadWalletRequest(mnemonics),
            std::bind(&API::AlgorandAPILoadWallet, unrealApi_.Get(),
                std::placeholders::_1, std::placeholders::_2),
            std::bind(&UAlgorandUnrealManager::OnLoadWalletCompleteCallback, this , std::placeholders::_1)
        );
}

/**
 * @brief get response from unreal api after restore wallet and broadcast the result to binded functions
 */
void UAlgorandUnrealManager::OnLoadWalletCompleteCallback(const Vertices::VerticesLoadWalletResponse& response) {
    if (response.IsSuccessful()) {
        UE_LOG(LogTemp, Display, TEXT("Wallet was loaded.  Output : %s"), *response.GetResponseString());
        LoadWalletCallback.Broadcast(EResultType::LoadWallet, FResultBoolean::ToResultBoolean(true));
    }
    else {
        if (ErrorDelegateCallback.IsBound())
        {
            UE_LOG(LogTemp, Error, TEXT("Wallet wasn't loaded.  Error : %s"), *response.GetResponseString());
            ErrorDelegateCallback.Broadcast(FError("Wallet wasn't loaded"));
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("ErrorDelegateCallback is not bound."));
        }
    }
}

/**
 * @brief create its context to send the request to unreal api for saving wallet
 */
void UAlgorandUnrealManager::saveWallet(const FString& mnemonics)
{
    this->requestContextManager_
        .createContext<API::FAlgorandAPISaveWalletDelegate,
        Vertices::VerticesSaveWalletRequest>(
            request_builders::buildSaveWalletRequest(mnemonics),
            std::bind(&API::AlgorandAPISaveWallet, unrealApi_.Get(),
                std::placeholders::_1, std::placeholders::_2),
            std::bind(&UAlgorandUnrealManager::OnSaveWalletCompleteCallback, this , std::placeholders::_1)
        );
}

/**
 * @brief get response from unreal api after restore wallet and broadcast the result to binded functions
 */
void UAlgorandUnrealManager::OnSaveWalletCompleteCallback(const Vertices::VerticesSaveWalletResponse& response) {
    if (response.IsSuccessful()) {
        UE_LOG(LogTemp, Display, TEXT("Wallet was saved.  Output : %s"), *response.GetResponseString());
        SaveWalletCallback.Broadcast(EResultType::SaveWallet, FResultBoolean::ToResultBoolean(true));
    }
    else {
        if (ErrorDelegateCallback.IsBound())
        {
            UE_LOG(LogTemp, Error, TEXT("Wallet wasn't saved.  Error : %s"), *response.GetResponseString());
            ErrorDelegateCallback.Broadcast(FError("Wallet wasn't saved"));
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("ErrorDelegateCallback is not bound."));
        }
    }
}

/**
 * @brief create its context to send the request to unreal api for get backup mnemonic phrase
 */
 void UAlgorandUnrealManager::getMnemonicsByAccountName()
{
    this->requestContextManager_
        .createContext<API::FAlgorandAPIGetMnemonicsByAccountNameDelegate,
        Vertices::VerticesGetMnemonicsByAccountNameRequest>(
            request_builders::buildGetMnemonicsByAccountNameRequest(),
            std::bind(&API::AlgorandAPIGetMnemonicsByAccountName, unrealApi_.Get(),
                std::placeholders::_1, std::placeholders::_2),
            std::bind(&UAlgorandUnrealManager::OnGetMnemonicsByAccountNameCompleteCallback, this , std::placeholders::_1)
        );
}

/**
 * @brief get response from unreal api after get backup mnemonic phrase and broadcast the result to binded functions
 */
void UAlgorandUnrealManager::OnGetMnemonicsByAccountNameCompleteCallback(const Vertices::VerticesGetMnemonicsByAccountNameResponse& response) {
    if (response.IsSuccessful()) {
        UE_LOG(LogTemp, Display, TEXT("Mnemonics were fetched.  Mnemonics : %s, Output : %s"), *response.Mnemonics, *response.GetResponseString());
        GetMnemonicsByAccountNameCallback.Broadcast(EResultType::GetMnemonics, FResultString::ToResultString(response.Mnemonics));
    }
    else {
        if (ErrorDelegateCallback.IsBound())
        {
            UE_LOG(LogTemp, Error, TEXT("Wallet wasn't initialized.  Error : %s"), *response.GetResponseString());
            ErrorDelegateCallback.Broadcast(FError("Wallet wasn't initialized"));
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("ErrorDelegateCallback is not bound"));
        }
    }
}

/**
 * @brief create its context to send the request to unreal api for generate mnemonics
 */
 void UAlgorandUnrealManager::generateAccountFromMnemonics()
{
    this->requestContextManager_
        .createContext<API::FAlgorandAPIGenerateAccountFromMnemonicsDelegate,
        Vertices::VerticesGenerateAccountFromMnemonicsRequest>(
            request_builders::buildGenerateAccountFromMnemonicsRequest(),
            std::bind(&API::AlgorandAPIGenerateAccountFromMnemonics, unrealApi_.Get(),
                std::placeholders::_1, std::placeholders::_2),
            std::bind(&UAlgorandUnrealManager::OnGenerateAccountFromMnemonicsCompleteCallback, this , std::placeholders::_1)
        );
}

/**
 * @brief get response from unreal api after generate mnemonics and broadcast the result to binded functions
 */
void UAlgorandUnrealManager::OnGenerateAccountFromMnemonicsCompleteCallback(const Vertices::VerticesGenerateAccountFromMnemonicsResponse& response) {
    if (response.IsSuccessful()) {
        UE_LOG(LogTemp, Display, TEXT("Account was generated.  Address : %s, Output : %s"), *response.Address, *response.GetResponseString());
        GenerateAccountFromMnemonicsCallback.Broadcast(EResultType::GenAccount, FResultAccount::ToResultAccount(response.Address, response.Name));
    }
    else {
        if (ErrorDelegateCallback.IsBound())
        {
            UE_LOG(LogTemp, Error, TEXT("Account wasn't generated.  Error : %s"), *response.GetResponseString());
            ErrorDelegateCallback.Broadcast(FError("Wallet wasn't initialized"));
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("ErrorDelegateCallback is not bound."));
        }
    }
}

/**
 * @brief create its context to send the request to unreal api for get balance
 */
void UAlgorandUnrealManager::getAddressBalance(const FString& address)
{
    this->requestContextManager_
        .createContext<API::FAlgorandAPIGetAddrBalanceDelegate,
        Vertices::VerticesGetAddrBalanceRequest>(
            request_builders::buildGetBalanceRequest(address),
            std::bind(&API::AlgorandAPIGetAddrBalance, unrealApi_.Get(),
                std::placeholders::_1, std::placeholders::_2),
            std::bind(&UAlgorandUnrealManager::OnGetAddressBalanceCompleteCallback, this , std::placeholders::_1)
        );
}

/**
 * @brief get response from unreal api after get balance and broadcast the result to binded functions
 */
void UAlgorandUnrealManager::OnGetAddressBalanceCompleteCallback(const Vertices::VerticesGetAddrBalanceResponse& response) {
    if (response.IsSuccessful()) {
        UE_LOG(LogTemp, Display, TEXT("Algo Balance was fetched.  Address : %llu, Output : %s"), response.Amount, *response.GetResponseString());
        GetAddressBalanceCallback.Broadcast(EResultType::AddrBalance, FResultUInt64::ToResultUInt64(response.Amount));
        FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("Get Balance Action", "Got Balance successfully."));

        if(response.Amount < 1000)
        {
            FFormatNamedArguments Arguments;
            Arguments.Add(TEXT("Address"), FText::FromString(response.Address));
            FMessageDialog::Open(EAppMsgType::Ok, FText::Format(LOCTEXT("Warning", "Go to https://bank.testnet.algorand.network/, dispense Algos to: {Address}"), Arguments));
        }
    }
    else {
        if (ErrorDelegateCallback.IsBound())
        {
            UE_LOG(LogTemp, Error, TEXT("Algo Balance can't be got.  Error : %s"), *response.GetResponseString());
            ErrorDelegateCallback.Broadcast(FError("Algo Balance can't be got"));
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("ErrorDelegateCallback is not bound"));
        }
    }
}

/**
 * @brief create its context to send the request to unreal api for payment tx
 */
void UAlgorandUnrealManager::sendPaymentTransaction(const FString& receiverAddress,
                                                    const FUInt64& amount,
                                                    const FString& notes)
{
    this->requestContextManager_
        .createContext<API::FAlgorandAPISendPayTxDelegate,
        Vertices::VerticesSendPayTxRequest>(
            request_builders::buildPaymentTransactionRequest(this->getAddress(),
                                                             receiverAddress,
                                                             amount,
                                                             notes),
            std::bind(&API::AlgorandAPISendPayTx, unrealApi_.Get(),
                std::placeholders::_1, std::placeholders::_2),
            std::bind(&UAlgorandUnrealManager::OnSendPaymentTransactionCompleteCallback, this, std::placeholders::_1)
            );
}

/**
 * @brief get response from unreal api after payment TX and broadcast the result to binded functions
 */
void UAlgorandUnrealManager::OnSendPaymentTransactionCompleteCallback(const Vertices::VerticesSendPayTxResponse& response) {
    if (response.IsSuccessful()) {
        UE_LOG(LogTemp, Display, TEXT("PaymentTx was completed.  Transaction ID : %s, Output : %s"), *response.TxID, *response.GetResponseString());
        SendPaymentTxCallback.Broadcast(EResultType::PayTx, FResultString::ToResultString(response.TxID));
        FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("Payment Transaction", "Sent payment tx successfully."));
    }
    else {
        if (ErrorDelegateCallback.IsBound())
        {
            UE_LOG(LogTemp, Error, TEXT("PaymentTx was failed.  Error : %s"), *response.GetResponseString());
            ErrorDelegateCallback.Broadcast(FError("PaymentTx was failed"));
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("ErrorDelegateCallback is not bound."));
        }
    }
}

/**
 * @brief create its context to send the request to unreal api for asset config tx
 */
void UAlgorandUnrealManager::sendAssetConfigTransaction(const FString& manager,
                                                        const FString& reserve,
                                                        const FString& freeze,
                                                        const FString& clawback,
                                                        const FUInt64& asset_id,
                                                        const FUInt64& total,
                                                        const FUInt64& decimals,
                                                        const FString& isFrozen,
                                                        const FString& unit_name,
                                                        const FString& asset_name,
                                                        const FString& url,
                                                        const FString& notes)
{
    this->requestContextManager_
        .createContext<API::FAlgorandAPISendAcfgTxDelegate,
        Vertices::VerticesSendAcfgTxRequest>(
            request_builders::buildAssetConfigTransactionRequest(this->getAddress(),
                                                                 manager,
                                                                 reserve,
                                                                 freeze,
                                                                 clawback,
                                                                 asset_id,
                                                                 total,
                                                                 decimals,
                                                                 isFrozen,
                                                                 unit_name,
                                                                 asset_name,
                                                                 url,
                                                                 notes),
            std::bind(&API::AlgorandAPISendAcfgTx, unrealApi_.Get(),
                std::placeholders::_1, std::placeholders::_2),
            std::bind(&UAlgorandUnrealManager::OnSendAssetConfigTransactionCompleteCallback, this, std::placeholders::_1)
            );
}

/**
 * @brief get response from unreal api after asset config TX and broadcast the result to binded functions
 */
void UAlgorandUnrealManager::OnSendAssetConfigTransactionCompleteCallback(const Vertices::VerticesSendAcfgTxResponse& response) {
    if (response.IsSuccessful()) {
        UE_LOG(LogTemp, Display, TEXT("AcfgTx was completed.  Transaction ID : %s, Output : %s"), *response.TxID, *response.GetResponseString());
        SendAssetConfigTransactionCallback.Broadcast(EResultType::AcfgTx, FResultAcfgTx::ToResultAcfgTx(response.TxID, response.AssetID));
        FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("Asset Config Transaction", "Sent asset config tx successfully."));
    }
    else {
        if (ErrorDelegateCallback.IsBound())
        {
            UE_LOG(LogTemp, Error, TEXT("AcfgTx was failed.  Error : %s"), *response.GetResponseString());
            ErrorDelegateCallback.Broadcast(FError("AcfgTx was failed."));
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("ErrorDelegateCallback is not bound"));
        }
    }
}

/**
 * @brief create its context to send the request to unreal api for asset transfer tx
 */
void UAlgorandUnrealManager::sendAssetTransferTransaction(const FString& senderAddress,
                                                        const FString& receiverAddress,
                                                        const FUInt64& asset_ID,
                                                        const FString& amount,
                                                        const FString& notes)
{
    this->requestContextManager_
        .createContext<API::FAlgorandAPISendAxferTxDelegate,
        Vertices::VerticesSendAxferTxRequest>(
            request_builders::buildAssetTransferTransactionRequest(senderAddress,
                                                             receiverAddress,
                                                             asset_ID,
                                                             amount,
                                                             notes),
            std::bind(&API::AlgorandAPISendAxferTx, unrealApi_.Get(),
                std::placeholders::_1, std::placeholders::_2),
            std::bind(&UAlgorandUnrealManager::OnSendAssetTransferTransactionCompleteCallback, this, std::placeholders::_1)
            );
}

/**
 * @brief get response from unreal api after asset config TX and broadcast the result to binded functions
 */
void UAlgorandUnrealManager::OnSendAssetTransferTransactionCompleteCallback(const Vertices::VerticesSendAxferTxResponse& response) {
    if (response.IsSuccessful()) {
        UE_LOG(LogTemp, Display, TEXT("AxferTx was completed.  Transaction ID : %s, Output : %s"), *response.TxID, *response.GetResponseString());
        SendAssetTransferTransactionCallback.Broadcast(EResultType::AxferTx, FResultString::ToResultString(response.TxID));
        FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("Asset Transfer Transaction", "Sent asset transfer tx successfully."));
    }
    else {
        if (ErrorDelegateCallback.IsBound())
        {
            UE_LOG(LogTemp, Error, TEXT("AxferTx was failed.  Error : %s"), *response.GetResponseString());
            ErrorDelegateCallback.Broadcast(FError("Axfer was failed"));
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("ErrorDelegateCallback is not bound."));
        }
    }
}

/**
 * @brief create its context to send the request to unreal api for application call TX
 */
void UAlgorandUnrealManager::sendApplicationCallTransaction(const FUInt64& app_ID, const TArray<FAppArg>& app_args, const EAppOnCompleteTX& app_complete_tx)
{
    this->requestContextManager_
        .createContext<API::FAlgorandAPISendApplCallTxDelegate,
        Vertices::VerticesSendApplCallTxRequest>(
            request_builders::buildApplicationCallTransactionRequest(this->getAddress(),
                                                                     app_ID,
                                                                     app_args,
                                                                     app_complete_tx),
            std::bind(&API::AlgorandAPISendApplCallTx, unrealApi_.Get(),
                std::placeholders::_1, std::placeholders::_2),
            std::bind(&UAlgorandUnrealManager::OnSendApplicationCallTransactionCompleteCallback, this, std::placeholders::_1)
            );
}

/**
 * @brief get response from unreal api after application call TX and broadcast the result to binded functions
 */
void UAlgorandUnrealManager::OnSendApplicationCallTransactionCompleteCallback(const Vertices::VerticesSendApplCallTxResponse& response) {
    if (response.IsSuccessful()) {
        UE_LOG(LogTemp, Display, TEXT("ApplTx was completed.  Transaction ID : %s, Output : %s"), *response.TxID, *response.GetResponseString());
        SendApplicationCallTransactionCallback.Broadcast(EResultType::ApplTx, FResultApplTx::ToResultApplTx(response.TxID, response.Logs));
        FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("Application Transaction", "Sent application tx successfully."));
    }
    else {
        if (ErrorDelegateCallback.IsBound())
        {
            UE_LOG(LogTemp, Error, TEXT("ApplTx was failed.  Error : %s"), *response.GetResponseString());
            ErrorDelegateCallback.Broadcast(FError("ApplTx was failed"));
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("ErrorDelegateCallback is not bound."));
        }
    }
}

/**
 * @brief create its context to send the request to unreal api for arc asset details
 */
void UAlgorandUnrealManager::fetchArcAssetDetails(const FUInt64& asset_ID)
{
    this->requestContextManager_
        .createContext<API::FAlgorandAPIArcAssetDetailsDelegate,
        Vertices::VerticesArcAssetDetailsRequest>(
            request_builders::buildArcAssetDetailsRequest(asset_ID),
            std::bind(&API::AlgorandAPIArcAssetDetails, unrealApi_.Get(),
                std::placeholders::_1, std::placeholders::_2),
            std::bind(&UAlgorandUnrealManager::OnFetchArcAssetDetailsCompleteCallback, this, std::placeholders::_1)
            );
}

/**
 * @brief get response from unreal api after fetching arc asset details and broadcast the result to binded functions
 */
void UAlgorandUnrealManager::OnFetchArcAssetDetailsCompleteCallback(const Vertices::VerticesArcAssetDetailsResponse& response) {
    if (response.IsSuccessful()) {
        FArcAssetDetails arcNft(response);
        FetchArcAssetDetailsCallback.Broadcast(EResultType::ArcAsset, arcNft);
        UE_LOG(LogTemp, Display, TEXT("ArcAsset was fetched.  Output : %s"), *response.GetResponseString());
        FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("Arc Asset Details", "Sent request to fetch details of arc asset successfully."));
    }
    else {
        if (ErrorDelegateCallback.IsBound())
        {
            UE_LOG(LogTemp, Error, TEXT("ArcAsset wasn't fetched.  Error : %s"), *response.GetResponseString());
            ErrorDelegateCallback.Broadcast(FError("ArcAsset wasn't fetched"));
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("ErrorDelegateCallback is not bound."));
        }
    }
}

/**
 * @brief create its context to send the request to unreal api for account information
 */
void UAlgorandUnrealManager::fetchAccountInformation(const FString& address)
{
    this->requestContextManager_
        .createContext<API::FAlgorandAPIAccountInformationDelegate,
        Vertices::VerticesAccountInformationRequest>(
            request_builders::buildAccountInformationRequest(address),
            std::bind(&API::AlgorandAPIAccountInformation, unrealApi_.Get(),
                std::placeholders::_1, std::placeholders::_2),
            std::bind(&UAlgorandUnrealManager::OnFetchAccountInformationCompleteCallback, this, std::placeholders::_1)
            );
}

/**
 * @brief get response from unreal api after fetching arc asset details and broadcast the result to binded functions
 */
void UAlgorandUnrealManager::OnFetchAccountInformationCompleteCallback(const Vertices::VerticesAccountInformationResponse& response) {
    if (response.IsSuccessful()) {
        FetchAccountInformationCallback.Broadcast(EResultType::AccAssets, FResultAccAssets::ToResultAccAssets(response.AssetIDs, response.AssetNames));
        FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("Account Information", "sent request to fetch account info successfully."));
    }
    else {
        FFormatNamedArguments Arguments;
        Arguments.Add(TEXT("MSG"), FText::FromString(response.GetResponseString()));
        FMessageDialog::Open(EAppMsgType::Ok, FText::Format(LOCTEXT("Error", "{MSG}"), Arguments));
        
        if (ErrorDelegateCallback.IsBound())
        {
            UE_LOG(LogTemp, Error, TEXT("Wallet wasn't initialized.  Error : %s"), *response.GetResponseString());
            ErrorDelegateCallback.Broadcast(FError("Wallet wasn't initialized"));
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("ErrorDelegateCallback is not bound.  Warning : %s"), *response.GetResponseString());
        }
    }
}

UWorld* UAlgorandUnrealManager::GetWorld() const
{
    return GetOuter()->GetWorld();
}

#undef LOCTEXT_NAMESPACE