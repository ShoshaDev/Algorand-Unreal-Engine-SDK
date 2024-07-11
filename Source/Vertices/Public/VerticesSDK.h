// Copyright 2022, Wisdom Labs. All Rights Reserved

#pragma once

#include "Modules/ModuleManager.h"
#include "include/vertices/vertices.h"
#include "include/utils/base32.h"
#include "include/utils/base64.h"
#include "http_weak.h"
#include "../Private/include/sodium.h"
#include <cstring>
#include "../Private/Account.h"
#include "../Private/SDKException.h"

/**
 * ToDo:
 * for asset bases on transactions and wallet provider
 * add like txs
 * Key Registration and Freeze, State Proof
 */

/**
 * Vertices SDK
 * how call thirdparty library and send request
 * and execute allocated delegate with response
 * ue5 async task is used for multi-tasking
 */

namespace algorand {
namespace vertices {

    class VERTICES_API VerticesSDK
    {
    public:
        VerticesSDK();
        ~VerticesSDK();

        /**
            * @brief set rpc url of algorand node 
            * @param indexerRpc indexer rpc url
            */
        void setIndexerRpc(const FString& indexerRpc);
     
        /**
         * @brief set rpc url of algorand node 
         * @param algoRpc algoRpc rpc url
         */
        void setAlgodRpc(const FString& algodRpc);

        /**
         * @brief set rpc port of algorand node
         * @param algoPort algoPort rpc port
         */
        void setAlgoPort(const int& algoPort);

        /**
         * @brief set rpc tokenHeader of algorand node
         * @param algoTokenHeader algorand rpc tokenHeader
         */
        void setAlgoTokenHeader(const FString& algoTokenHeader);

        /**
         * @brief load vertices library from thirdparty module
         */
        void loadVerticesLibrary();

        /**
         * @brief pass its pointer of  updated functions through Vertices for HttpCurls 
         */
        void setHTTPCURLs();

        /**
         * @brief validate rpc info and set them to Vertices lib
         * and create new vertices instance and validate how the lib works
         */
        void InitVertices(ret_code_t&);

        /**
         * @brief create new Vertices with rpc info
         */
        void createNewVertices(char* , char*,  short, char* , ret_code_t&);

        /**
         * @brief convert mnemonic account to vertices account
         * @return return status of converting
         */
        ret_code_t convert_Account_Vertices();
     
        /**
         * @brief check whether vertices lib works using simple api request
         */
        void vertices_ping_check(ret_code_t&);

        /**
         * @brief check vertices vertions with api request
         */
        void vertices_version_check(ret_code_t&);

        /**
         * @brief check whether vertices actions is successful with err_code
         * @param err_code if VTC_SUCCESS, successful, not failed
         */
        void checkVTCSuccess(ret_code_t& err_code); // error handling

        /**
         * @brief check status of vertices actions with err_code
         * @param Msg message as output for exception process
         * @param err_code if VTC_SUCCESS, successful, not failed
         */
        void checkVTCSuccess(char* Msg, ret_code_t& err_code);

        /**
         * @brief set Request and Response for building api request and its result
         */
        class VerticesInitWalletRequest;
        class VerticesInitWalletResponse;
        class VerticesLoadWalletRequest;
        class VerticesLoadWalletResponse;
        class VerticesSaveWalletRequest;
        class VerticesSaveWalletResponse;
        class VerticesGenerateAccountFromMnemonicsRequest;
        class VerticesGenerateAccountFromMnemonicsResponse;
        class VerticesGetMnemonicsByAccountNameRequest;
        class VerticesGetMnemonicsByAccountNameResponse;
        class VerticesGetAddrBalanceRequest;
        class VerticesGetAddrBalanceResponse;
        class VerticesSendPayTxRequest;
        class VerticesSendPayTxResponse;
        class VerticesSendAcfgTxRequest;
        class VerticesSendAcfgTxResponse;
        class VerticesSendAxferTxRequest;
        class VerticesSendAxferTxResponse;
        class VerticesSendApplCallTxRequest;
        class VerticesSendApplCallTxResponse;
        class VerticesArcAssetDetailsRequest;
        class VerticesArcAssetDetailsResponse;
        class VerticesAccountInformationRequest;
        class VerticesAccountInformationResponse;

        /// callback, which initialize the algorand wallet
        DECLARE_DELEGATE_OneParam(FVerticesInitWalletDelegate, const VerticesInitWalletResponse&);
     
        /// callback ,which loads a stored wallet
        DECLARE_DELEGATE_OneParam(FVerticesLoadWalletDelegate, const VerticesLoadWalletResponse&);

        /// callback ,which saves the algorand wallet
        DECLARE_DELEGATE_OneParam(FVerticesSaveWalletDelegate, const VerticesSaveWalletResponse&);

        /// callback, which generate a new account on wallet if the wallet is empty 
        DECLARE_DELEGATE_OneParam(FVerticesGenerateAccountFromMnemonicsDelegate, const VerticesGenerateAccountFromMnemonicsResponse&);

        /// callback, which backups mnemonic phrase by account name on a wallet
        DECLARE_DELEGATE_OneParam(FVerticesGetMnemonicsByAccountNameDelegate, const VerticesGetMnemonicsByAccountNameResponse&);

        /// callabck , which get balance by any algorand address
        DECLARE_DELEGATE_OneParam(FVerticesGetAddrBalanceDelegate, const VerticesGetAddrBalanceResponse&);

        /// callback , which enables sending pay tx
        DECLARE_DELEGATE_OneParam(FVerticesSendPayTxDelegate, const VerticesSendPayTxResponse&);

        /// callabck , which enables sending acfg tx 
        DECLARE_DELEGATE_OneParam(FVerticesSendAcfgTxDelegate, const VerticesSendAcfgTxResponse&);

        /// callabck , which enables sending axfer tx 
        DECLARE_DELEGATE_OneParam(FVerticesSendAxferTxDelegate, const VerticesSendAxferTxResponse&);

        /// callback , which enables sending appl call tx
        DECLARE_DELEGATE_OneParam(FVerticesSendApplCallTxDelegate, const VerticesSendApplCallTxResponse&);

        void VerticesInitWallet(const VerticesInitWalletRequest&, const FVerticesInitWalletDelegate&);
        void VerticesLoadWallet(const VerticesLoadWalletRequest&, const FVerticesLoadWalletDelegate&);
        void VerticesSaveWallet(const VerticesSaveWalletRequest&, const FVerticesSaveWalletDelegate&);
        void VerticesGenerateAccountFromMnemonics(const VerticesGenerateAccountFromMnemonicsRequest&, const FVerticesGenerateAccountFromMnemonicsDelegate&);
        void VerticesGetMnemonicsByAccountName(const VerticesGetMnemonicsByAccountNameRequest&, const FVerticesGetMnemonicsByAccountNameDelegate&);
        void VerticesGetAddrBalance(const VerticesGetAddrBalanceRequest&, const FVerticesGetAddrBalanceDelegate&);
        void VerticesSendPayTx(const VerticesSendPayTxRequest&, const FVerticesSendPayTxDelegate&);
        void VerticesSendAcfgTx(const VerticesSendAcfgTxRequest&, const FVerticesSendAcfgTxDelegate&);
        void VerticesSendAxferTx(const VerticesSendAxferTxRequest&, const FVerticesSendAxferTxDelegate&);
        void VerticesSendApplCallTx(const VerticesSendApplCallTxRequest&, const FVerticesSendApplCallTxDelegate&);
    
    private:
        // algorand rpc info for creating vertices
        FString myIndexerRpc;
        FString myAlgodRpc;
        int myAlgoPort;
        FString myAlgoTokenHeader;

        // main algo account
        Account main_account;
     
        // mutex for multi-threads
        FCriticalSection m_Mutex;

        // variables for loading thirdparty libs
        void* VerticesHandle;
        void* SodiumHandle;

     
        bool loaded_;
        bool vertices_usable;
        FString config_path;
    };

}
}

