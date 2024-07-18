// Copyright 2022, Wisdom Labs. All Rights Reserved

#pragma once
#include "CoreMinimal.h"
#include "VerticesApiOperations.h"
#include "VerticesApiOperations.h"
#include "VerticesApiOperations.h"
#include "VerticesApiOperations.h"
#include "VerticesApiOperations.h"
#include "VerticesApiOperations.h"
#include "VerticesApiOperations.h"
#include "VerticesApiOperations.h"
#include "VerticesApiOperations.h"
#include "VerticesApiOperations.h"
#include "VerticesSDK.h"

namespace {
    using Vertices = algorand::vertices::VerticesSDK;
}

/**
 * ToDo:
 * for asset based on new transactions and wallet provider
 * add following txs
* Key Registration, Asset Configuration, Freeze and Transafer, State Proof
 */

/**
 * Unreal API
 * how send request to vertices SDK 
 * and run callback function by delegate after complete tx process
 */

namespace algorand{
namespace api{

class ALGORANDAPI_API UnrealApi
{
public:
    UnrealApi(TSharedPtr<algorand::vertices::VerticesSDK>&);
    ~UnrealApi();
	
	DECLARE_DELEGATE_OneParam(FAlgorandAPIInitWalletDelegate, const Vertices::VerticesInitWalletResponse&);
	
    DECLARE_DELEGATE_OneParam(FAlgorandAPILoadWalletDelegate, const Vertices::VerticesLoadWalletResponse&);
	
	DECLARE_DELEGATE_OneParam(FAlgorandAPISaveWalletDelegate, const Vertices::VerticesSaveWalletResponse&);
	
    DECLARE_DELEGATE_OneParam(FAlgorandAPIGetMnemonicsByAccountNameDelegate, const Vertices::VerticesGetMnemonicsByAccountNameResponse&);
	
    DECLARE_DELEGATE_OneParam(FAlgorandAPIGenerateAccountFromMnemonicsDelegate, const Vertices::VerticesGenerateAccountFromMnemonicsResponse&);

	DECLARE_DELEGATE_OneParam(FAlgorandAPIGenerateRandomAccountDelegate, const Vertices::VerticesGenerateRandomAccountResponse&);
	
    DECLARE_DELEGATE_OneParam(FAlgorandAPIGetAddrBalanceDelegate, const Vertices::VerticesGetAddrBalanceResponse&);
	
    DECLARE_DELEGATE_OneParam(FAlgorandAPISendPayTxDelegate, const Vertices::VerticesSendPayTxResponse&);
	
	DECLARE_DELEGATE_OneParam(FAlgorandAPISendAcfgTxDelegate, const Vertices::VerticesSendAcfgTxResponse&);
	
	DECLARE_DELEGATE_OneParam(FAlgorandAPISendAxferTxDelegate, const Vertices::VerticesSendAxferTxResponse&);
	
    DECLARE_DELEGATE_OneParam(FAlgorandAPISendApplCallTxDelegate, const Vertices::VerticesSendApplCallTxResponse&);
	
	DECLARE_DELEGATE_OneParam(FAlgorandAPIArcAssetDetailsDelegate, const Vertices::VerticesArcAssetDetailsResponse&);
	
	DECLARE_DELEGATE_OneParam(FAlgorandAPIAccountInformationDelegate, const Vertices::VerticesAccountInformationResponse&);

	/**
		 * @brief set algod rpc url, port, token of algorand node 
		 * @param algodRpc algodRpc rpc url
		 * * @param algodPort algodRpc rpc port
		 * * @param algodTokenHeader_ algodRpc rpc token
		 */
	void setAlgodRpcInfo(const FString& algodRpc, const uint64_t& algodPort, const FString& algodTokenHeader_);

	/**
		 * @brief set indexer rpc url, port, token of algorand node 
		 * @param indexerRpc indexerRpc rpc url
		 * @param indexerPort indexerRpc rpc port
		 * @param indexerTokenHeader indexerRpc rpc token
		 */
	void setIndexerRpcInfo(const FString& indexerRpc, const uint64_t& indexerPort, const FString& indexerTokenHeader);

	/**
	 * @brief send api request for initializing the algorand wallet
	 * @param Request value to send as params for calling api
	 * @param Delegate is used to implement async task after get response as api result
	 */
	void AlgorandAPIInitWallet(const Vertices::VerticesInitWalletRequest& Request,
								   const FAlgorandAPIInitWalletDelegate& Delegate = FAlgorandAPIInitWalletDelegate()) const;
	
    /**
     * @brief send api request for loading wallet
     * @param Request value to send as params for calling api
     * @param Delegate is used to implement async task after get response as api result
     */
    void AlgorandAPILoadWallet(const Vertices::VerticesLoadWalletRequest& Request,
								   const FAlgorandAPILoadWalletDelegate& Delegate = FAlgorandAPILoadWalletDelegate()) const;

	/**
	 * @brief send api request for saving wallet
	 * @param Request value to send as params for calling api
	 * @param Delegate is used to implement async task after get response as api result
	 */
	void AlgorandAPISaveWallet(const Vertices::VerticesSaveWalletRequest& Request,
								   const FAlgorandAPISaveWalletDelegate& Delegate = FAlgorandAPISaveWalletDelegate()) const;

	/**
     * @brief send api request for Getting Mnemonic Phrase by account name
     * @param Request value to send as params for calling api
     * @param Delegate is used to implement async task after get response as api result
     */
    void AlgorandAPIGetMnemonicsByAccountName(const Vertices::VerticesGetMnemonicsByAccountNameRequest& Request,
								   const FAlgorandAPIGetMnemonicsByAccountNameDelegate& Delegate = FAlgorandAPIGetMnemonicsByAccountNameDelegate()) const;

	/**
     * @brief send api request for Generating new account from Mnemonics
     * @param Request value to send as params for calling api
     * @param Delegate is used to implement async task after get response as api result
     */
    void AlgorandAPIGenerateAccountFromMnemonics(const Vertices::VerticesGenerateAccountFromMnemonicsRequest& Request,
								   const FAlgorandAPIGenerateAccountFromMnemonicsDelegate& Delegate = FAlgorandAPIGenerateAccountFromMnemonicsDelegate()) const;

	/**
	 * @brief send api request for Generating new random account
	 * @param Request value to send as params for calling api
	 * @param Delegate is used to implement async task after get response as api result
	 */
	void AlgorandAPIGenerateRandomAccount(const Vertices::VerticesGenerateRandomAccountRequest& Request,
								   const FAlgorandAPIGenerateRandomAccountDelegate& Delegate = FAlgorandAPIGenerateRandomAccountDelegate()) const;

	/**
	 * @brief send api request for getting balance of any address
	 * @param Request value to send as params for calling api
	 * @param Delegate is used to implement async task after get response as api result
	 */
    void AlgorandAPIGetAddrBalance(const Vertices::VerticesGetAddrBalanceRequest& Request, const FAlgorandAPIGetAddrBalanceDelegate& Delegate = FAlgorandAPIGetAddrBalanceDelegate()) const;

	/**
	 * @brief send api request for sending payment tx
	 * @param Request value to send as params for calling api
	 * @param Delegate is used to implement async task after get response as api result
	 */
    void AlgorandAPISendPayTx(const Vertices::VerticesSendPayTxRequest& Request, const FAlgorandAPISendPayTxDelegate& Delegate = FAlgorandAPISendPayTxDelegate()) const;

	/**
	 * @brief send api request for sending asset config tx
	 * @param Request value to send as params for calling api
	 * @param Delegate is used to implement async task after get response as api result
	 */
	void AlgorandAPISendAcfgTx(const Vertices::VerticesSendAcfgTxRequest& Request, const FAlgorandAPISendAcfgTxDelegate& Delegate = FAlgorandAPISendAcfgTxDelegate()) const;

	/**
	 * @brief send api request for sending asset transfer tx
	 * @param Request value to send as params for calling api
	 * @param Delegate is used to implement async task after get response as api result
	 */
	void AlgorandAPISendAxferTx(const Vertices::VerticesSendAxferTxRequest& Request, const FAlgorandAPISendAxferTxDelegate& Delegate = FAlgorandAPISendAxferTxDelegate()) const;

	/**
	 * @brief send api request for sending application call tx
	 * @param Request value to send as params for calling api
	 * @param Delegate is used to implement async task after get response as api result
	 */
    void AlgorandAPISendApplCallTx(const Vertices::VerticesSendApplCallTxRequest& Request, const FAlgorandAPISendApplCallTxDelegate& Delegate = FAlgorandAPISendApplCallTxDelegate()) const;

	/**
	 * @brief send api request for send arc asset details
	 * @param Request value to send as params for calling api
	 * @param Delegate is used to implement async task after get response as api result
	 */
	void AlgorandAPIArcAssetDetails(const Vertices::VerticesArcAssetDetailsRequest& Request, const FAlgorandAPIArcAssetDetailsDelegate& Delegate = FAlgorandAPIArcAssetDetailsDelegate()) const;

	/**
	 * @brief send api request for send account information
	 * @param Request value to send as params for calling api
	 * @param Delegate is used to implement async task after get response as api result
	 */
	void AlgorandAPIAccountInformation(const Vertices::VerticesAccountInformationRequest& Request, const FAlgorandAPIAccountInformationDelegate& Delegate = FAlgorandAPIAccountInformationDelegate()) const;

private:

	/**
	 * @brief callback function to be run after api request of initializing algorand wallet
	 * @param response is used to send as Vertices Response type to Algorand module 
	 * @param Delegate is used to execute binded callback from Algorand module
	 */
	void OnAlgorandAPIInitWalletResponse(const Vertices::VerticesInitWalletResponse& response, const FAlgorandAPIInitWalletDelegate& Delegate) const;
	
    /**
     * @brief callback function to be run after api request of loading algorand wallet
     * @param response is used to send as Vertices Response type to Algorand module 
     * @param Delegate is used to execute binded callback from Algorand module
     */
    void OnAlgorandAPILoadWalletResponse(const Vertices::VerticesLoadWalletResponse& response, const FAlgorandAPILoadWalletDelegate& Delegate) const;

	/**
	 * @brief callback function to be run after api request of savging algorand wallet
	 * @param response is used to send as Vertices Response type to Algorand module 
	 * @param Delegate is used to execute binded callback from Algorand module
	 */
	void OnAlgorandAPISaveWalletResponse(const Vertices::VerticesSaveWalletResponse& response, const FAlgorandAPISaveWalletDelegate& Delegate) const;

	/**
     * @brief callback function to be run after api request of getting mnemonics by account name
     * @param response is used to send as Vertices Response type to Algorand module 
     * @param Delegate is used to execute binded callback from Algorand module
     */
    void OnAlgorandAPIGetMnemonicsByAccountNameResponse(const Vertices::VerticesGetMnemonicsByAccountNameResponse& response, const FAlgorandAPIGetMnemonicsByAccountNameDelegate& Delegate) const;

	/**
     * @brief callback function to be run after api request of generating account from mnemonics
     * @param response is used to send as Vertices Response type to Algorand module 
     * @param Delegate is used to execute binded callback from Algorand module
     */
    void OnAlgorandAPIGenerateAccountFromMnemonicsGetResponse(const Vertices::VerticesGenerateAccountFromMnemonicsResponse& response, const FAlgorandAPIGenerateAccountFromMnemonicsDelegate& Delegate) const;

	/**
	 * @brief callback function to be run after api request of generating random account
	 * @param response is used to send as Vertices Response type to Algorand module 
	 * @param Delegate is used to execute binded callback from Algorand module
	 */
	void OnAlgorandAPIGenerateRandomAccountGetResponse(const Vertices::VerticesGenerateRandomAccountResponse& response, const FAlgorandAPIGenerateRandomAccountDelegate& Delegate) const;

	/**
	 * @brief callback function to be run after api request of getting balance of any address
	 * @param response is used to send as Vertices Response type to Algorand module 
	 * @param Delegate is used to execute binded callback from Algorand module
	 */
    void OnAlgorandAPIGetAddrBalanceResponse(const Vertices::VerticesGetAddrBalanceResponse& response, const FAlgorandAPIGetAddrBalanceDelegate& Delegate) const;

	/**
	 * @brief callback function to be run after api request of sending payment tx
	 * @param response is used to send as Vertices Response type to Algorand module 
	 * @param Delegate is used to execute binded callback from Algorand module
	 */
    void OnAlgorandAPISendPayTxResponse(const Vertices::VerticesSendPayTxResponse& response, const FAlgorandAPISendPayTxDelegate& Delegate) const;

	/**
	 * @brief callback function to be run after api request of sending asset config tx
	 * @param response is used to send as Vertices Response type to Algorand module 
	 * @param Delegate is used to execute binded callback from Algorand module
	 */
	void OnAlgorandAPISendAcfgTxResponse(const Vertices::VerticesSendAcfgTxResponse& response, const FAlgorandAPISendAcfgTxDelegate& Delegate) const;

	/**
	 * @brief callback function to be run after api request of sending asset transfer tx
	 * @param response is used to send as Vertices Response type to Algorand module 
	 * @param Delegate is used to execute binded callback from Algorand module
	 */
	void OnAlgorandAPISendAxferTxResponse(const Vertices::VerticesSendAxferTxResponse& response, const FAlgorandAPISendAxferTxDelegate& Delegate) const;

	/**
	 * @brief callback function to be run after api request of sending application call tx
	 * @param response is used to send as Vertices Response type to Algorand module 
	 * @param Delegate is used to execute binded callback from Algorand module
	 */
    void OnAlgorandAPISendApplCallTxResponse(const Vertices::VerticesSendApplCallTxResponse& response, const FAlgorandAPISendApplCallTxDelegate& Delegate) const;

	/**
	 * @brief callback function to be run after api request of sending arc asset details
	 * @param response is used to send as Vertices Response type to Algorand module 
	 * @param Delegate is used to execute binded callback from Algorand module
	 */
	void OnAlgorandAPIArcAssetDetailsResponse(const Vertices::VerticesArcAssetDetailsResponse& response, const FAlgorandAPIArcAssetDetailsDelegate& Delegate) const;

	/**
	 * @brief callback function to be run after api request of sending account information
	 * @param response is used to send as Vertices Response type to Algorand module 
	 * @param Delegate is used to execute binded callback from Algorand module
	 */
	void OnAlgorandAPIAccountInformationGetResponse(const Vertices::VerticesAccountInformationResponse& response, const FAlgorandAPIAccountInformationDelegate& Delegate) const;

	// Algorand modules
    TSharedPtr<algorand::vertices::VerticesSDK> vertices_;

	// algorand rpc information
	FString myAlgodRpc;
	uint64_t myAlgodPort;
	FString myAlgodTokenHeader;

	FString myIndexerRpc;
	uint64_t myIndexerPort;
	FString myIndexerTokenHeader;
};
       
}
}