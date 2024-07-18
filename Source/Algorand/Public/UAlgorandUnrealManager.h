// Copyright 2022, Wisdom Labs. All Rights Reserved

#pragma once
#include "CoreMinimal.h"
#include "Engine/World.h"
#include "RequestContextManager.h"
#include "Models/FArcAssetDetails.h"
#include "Models/FResultType.h"
#include "Models/FUInt64.h"
#include "Models/FError.h"
#include "UnrealApi.h"
#include "VerticesSDK.h"
#include <exception>
#include "UAlgorandUnrealManager.generated.h"

/**
 * ToDo:
 * for asset bases on transactions and wallet provider
 * add txs
 * Key Registration, Asset Freeze, State Proof
 */

/**
 * Algorand Manager
 * how callback works for c++ and blueprint events
 * dynamic and multicast delegates are used for c++ and blueprint callback
 * when c++ callback is called, it calls sendEvent to trigger the delegate.
 * ue5 async task is used for calling delegation.
 */

namespace {
    // Export Procedures
    using Vertices = algorand::vertices::VerticesSDK;
}

/**
 * initialize algorand wallet callback 
 * @param ResultType generated address
 * @param Result boolean value which displays wallet state
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInitWalletDelegate, const EResultType&, ResultType, const FResultBoolean&, Result);

/**
 * load wallet callback 
 * @param Result boolean value which displays state of loading wallet
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FLoadWalletDelegate, const EResultType&, ResultType, const FResultBoolean&, Result);

/**
 * save wallet callback 
 * @param Result boolean value which displays state of saving wallet
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSaveWalletDelegate, const EResultType&, ResultType, const FResultBoolean&, Result);

/**
 * get mnemonics by account name callback 
 * @param MnemonicByName backup mnemonics
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGetMnemonicsByAccountNameDelegate, const EResultType&, ResultType, const FResultString&, MnemonicByName);

/**
 * generate account from mnemonics callback 
 * @param GeneratedAccount generated account
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGenerateAccountFromMnemonicsDelegate, const EResultType&, ResultType, const FResultAccount&, GeneratedAccount);

/**
 * generate random account callback 
 * @param GeneratedAccount generated account
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGenerateRandomAccountDelegate, const EResultType&, ResultType, const FResultAccount&, GeneratedAccount);

/**
 * get balance by any address callback
 * @param money account balance
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGetAddressBalanceDelegate, const EResultType&, ResultType, const FResultUInt64&, BalanceByAddress);

/**
 * send payment tx callback
 * @param txID transaction hash
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSendPaymentTxDelegate, const EResultType&, ResultType, const FResultString&, TxID);

/**
 * send asset config tx callback
 * @param txID transaction hash
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSendAssetConfigTxDelegate, const EResultType&, ResultType, const FResultAcfgTx&, AcfgTX);

/**
 * send asset transfer tx callback
 * @param txID transaction hash
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSendAssetTransferTxDelegate, const EResultType&, ResultType, const FResultString&, TxID);

/**
 * send application call tx callback
 * @param txID transaction hash
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSendApplicationCallTxDelegate, const EResultType&, ResultType, const FResultApplTx&, ApplTx);

/**
 * nft details callback
 * @param assetDetails Arc Asset details
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FArcAssetDetailsDelegate, const EResultType&, ResultType, const FArcAssetDetails&, AssetDetails);

/**
 * account information callback
 * @param accountInfo Account Asset details
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAccountInfoDelegate, const EResultType&, ResultType, const FResultAccAssets&, AccAssets);

/**
 * error callback
 * @param error 
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FErrorDelegate, const FError&, error);

USTRUCT(BlueprintType)
struct ALGORAND_API FRPCInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Algod, Indexer")
	FString url;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Algod, Indexer")
	FUInt64 port;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Algod, Indexer")
	FString token;
};

class TransactionBuilder;

UCLASS(Blueprintable, BlueprintType)
class ALGORAND_API UAlgorandUnrealManager : public UObject
{
    GENERATED_BODY()

public:
    UAlgorandUnrealManager();

    /**
     * Create Instance of Algorand manager
     * @param algodRpc algorand rpc url https://testnet-api.algonode.cloud
     * @param algodPort algorand rpc port 443
     * @param algodTokenHeader algorand rpc token header "" , if localhost, X-Algo-API-Token: , if using purestake api, x-api-key:
	 * @param indexerRpc algorand rpc url https://testnet-idx.algonode.network
	 * @param indexerPort algorand rpc port 443
	 * @param indexerTokenHeader algorand rpc token header "" , if localhost, X-Algo-API-Token: , if using purestake api, x-api-key:
     * @param outer root on level
     * @return AlgorandUnrealManager as a pointer
     */
    UFUNCTION(BlueprintCallable,
			  meta = (DisplayName = "createInstanceWithParams", Keywords = "AlgorandManager"),
			  Category = "AlgorandUnrealManager")
        static UAlgorandUnrealManager* createInstanceWithParams(const FString& algodRpc,
																const FUInt64& algodPort,
																const FString& algodTokenHeader_,
																const FString& indexerRpc,
																const FUInt64& indexerPort,
																const FString& indexerTokenHeader_,
																UObject* outer);

	/**
	 * Create Instance of Algorand manager
	 * @param outer root on level
	 * @return AlgorandUnrealManager as a pointer
	 * 
	 */
	UFUNCTION(BlueprintCallable,
			  meta = (DisplayName = "CreateInstance", Keywords = "AlgorandManager"),
			  Category = "AlgorandUnrealManager")
		static UAlgorandUnrealManager* createInstance(UObject* outer);

	/**
	 * Create Instance of Algorand manager
	 * @param algodRpc algorand rpc url https://testnet-api.algonode.cloud
	 * @param algodPort algorand rpc port 443
	 * @param algodTokenHeader algorand rpc token header "" , if localhost, X-Algo-API-Token: , if using purestake api, x-api-key:
	 * @return AlgorandUnrealManager as a pointer
	 */
	UFUNCTION(BlueprintCallable,
			  meta = (DisplayName = "setAlgodRpcInfo", Keywords = "AlgorandManager"),
			  Category = "AlgorandUnrealManager")
		void setAlgodRpcInfo(const FString& algodRpc, const FUInt64& algodPort, const FString& algodTokenHeader_);

	/**
	 * Fetch Algod RPC Info of Algorand manager
	 * @param RPC Info for algod
	 * @return AlgorandUnrealManager as a pointer
	 */
	UFUNCTION(BlueprintCallable,
			  meta = (DisplayName = "getAlgodRpcInfo", Keywords = "AlgorandManager"),
			  Category = "AlgorandUnrealManager")
	FRPCInfo getAlgodRpcInfo();

	/**
	 * Create Instance of Algorand manager
	 * @param indexerRpc algorand rpc url https://testnet-idx.algonode.network
	 * @param indexerPort algorand rpc port 443
	 * @param indexerTokenHeader algorand rpc token header "" , if localhost, X-Algo-API-Token: , if using purestake api, x-api-key:
	 * @return AlgorandUnrealManager as a pointer
	 */
	UFUNCTION(BlueprintCallable,
			  meta = (DisplayName = "setIndexerRpcInfo", Keywords = "AlgorandManager"),
			  Category = "AlgorandUnrealManager")
		void setIndexerRpcInfo(const FString& indexerRpc, const FUInt64& indexerPort, const FString& indexerTokenHeader);

	/**
	 * Fetch Indexer RPC Info of Algorand manager
	 * @param RPC Info for indexer
	 * @return AlgorandUnrealManager as a pointer
	 */
	UFUNCTION(BlueprintCallable,
			  meta = (DisplayName = "getIndexerRpcInfo", Keywords = "AlgorandManager"),
			  Category = "AlgorandUnrealManager")
	FRPCInfo getIndexerRpcInfo();

	/**
	 * get current rpc net connected to algorand node
	 * @return rpc net as string
	 */
	UFUNCTION(BlueprintCallable,
			  meta = (DisplayName = "getAlgodRpcNet", Keywords = "AlgodRpcInfo"),
			  Category = "AlgorandUnrealManager")
		FString getAlgodRpcNet();

	/**
	 * get current rpc net connected to algorand node
	 * @return rpc net as string
	 */
	UFUNCTION(BlueprintCallable,
			  meta = (DisplayName = "getIndexerRpcNet", Keywords = "IndexerRpcInfo"),
			  Category = "AlgorandUnrealManager")
		FString getIndexerRpcNet();
	
    /**
     * get account address connected to algorand node
     * @return account address as string
     */
    UFUNCTION(BlueprintCallable,
			  meta = (DisplayName = "getAddress", Keywords = "Address"),
    		  Category = "AlgorandUnrealManager")
        FString getAddress();

    /**
     * set account address connected to algorand node
     * @param  address account connected to node
     */
    void setAddress(const FString& address);

    /**
     * error information callback
     */
    UPROPERTY(BlueprintAssignable, Category = "MultiCastDelegate")
        FErrorDelegate ErrorDelegateCallback;

	UFUNCTION(BlueprintCallable,
			  meta = (DisplayName = "Initialize Algorand Wallet", Keywords = "Wallet"),
			  Category = "AlgorandUnrealManager")
	void initWallet();
	
	UPROPERTY(BlueprintAssignable, Category = "MultiCastDelegate")
        FInitWalletDelegate InitWalletCallback;

	/**
	 * get response after initializing algorand wallet
	 * @param response address got by initializing algorand wallet
	 */
    void OnInitWalletCompleteCallback(const Vertices::VerticesInitWalletResponse& response);

	/**
	 * Load Wallet
	 */
	UFUNCTION(BlueprintCallable,
			  meta = (DisplayName = "Load Wallet", Keywords = "Wallet"),
			  Category = "AlgorandUnrealManager")
	void loadWallet(const FString& Password);
	
	UPROPERTY(BlueprintAssignable, Category = "MultiCastDelegate")
	FLoadWalletDelegate LoadWalletCallback;
	
	/**
	 * get response after loading wallet
	 * @param response address got by loading wallet
	 */
	void OnLoadWalletCompleteCallback(const Vertices::VerticesLoadWalletResponse& response);

	/**
	 * Save Wallet
	 */
	UFUNCTION(BlueprintCallable,
			  meta = (DisplayName = "Save Wallet", Keywords = "wallet"),
			  Category = "AlgorandUnrealManager")
	void saveWallet(const FString& mnemonics);
	
	UPROPERTY(BlueprintAssignable, Category = "MultiCastDelegate")
	FLoadWalletDelegate SaveWalletCallback;
	
	/**
	 * get response after saving wallet
	 * @param response address got by saving wallet
	 */
	void OnSaveWalletCompleteCallback(const Vertices::VerticesSaveWalletResponse& response);
	
	/**
	 * Get Mnemonics By Account Name
	 */
    UFUNCTION(BlueprintCallable,
			  meta = (DisplayName = "Get Mnemonics By Account Name", Keywords = "wallet"),
    		  Category = "AlgorandUnrealManager")
    void getMnemonicsByAccountName(const FString& Name);

	UPROPERTY(BlueprintAssignable, Category = "MultiCastDelegate")
        FGetMnemonicsByAccountNameDelegate GetMnemonicsByAccountNameCallback;

	/**
	 * Get mnemonics by account name
	 * @param response mnemonics phrase of backup
	 */
    void OnGetMnemonicsByAccountNameCompleteCallback(const Vertices::VerticesGetMnemonicsByAccountNameResponse& response);
    
	/**
	 * Generate Account From mnemonics
	 */
    UFUNCTION(BlueprintCallable,
			  meta = (DisplayName = "Generate Account From Mnemonics", Keywords = "wallet"),
    		  Category = "AlgorandUnrealManager")
    void generateAccountFromMnemonics(const FString& Mnemonics, const FString& Name);
	
	UPROPERTY(BlueprintAssignable, Category = "MultiCastDelegate")
        FGenerateAccountFromMnemonicsDelegate GenerateAccountFromMnemonicsCallback;

	/**
	 * get response after generating account from mnemonics
	 * @param response address got by generating account from given mnemonics
	 */
    void OnGenerateAccountFromMnemonicsCompleteCallback(const Vertices::VerticesGenerateAccountFromMnemonicsResponse& response);

	/**
	 * Generate a random Account
	 */
	UFUNCTION(BlueprintCallable,
			  meta = (DisplayName = "Generate Random Account ", Keywords = "wallet"),
			  Category = "AlgorandUnrealManager")
	void generateRandomAccount(const FString& Name);
	
	UPROPERTY(BlueprintAssignable, Category = "MultiCastDelegate")
	FGenerateRandomAccountDelegate GenerateRandomAccountCallback;

	/**
	 * get response after generating random account 
	 * @param response account got by generating random account
	 */
	void OnGenerateRandomAccountCompleteCallback(const Vertices::VerticesGenerateRandomAccountResponse& response);
    
	/**
	 * get balance by specific address
	 * 
	 */
    UFUNCTION(BlueprintCallable,
			  meta = (DisplayName = "get Address Balance", Keywords = "balance"),
    		  Category = "AlgorandUnrealManager")
    void getAddressBalance(const FString& address);
	
	UPROPERTY(BlueprintAssignable, Category = "MultiCastDelegate")
        FGetAddressBalanceDelegate GetAddressBalanceCallback;

	/**
	 * get response after getting balance
	 * @param response token amount by specific address
	 */
    void OnGetAddressBalanceCompleteCallback(const Vertices::VerticesGetAddrBalanceResponse& response);

	/**
	 * send payment TX to algorand node
	 * @param receiverAddress address which take token 
	 * @param amount token amount to send
	 * @param notes tx description when send payment tx
	 */
    UFUNCTION(BlueprintCallable,
			  meta = (DisplayName = "Send Payment TX", Keywords = "pay"),
    		  Category = "AlgorandUnrealManager")
    void sendPaymentTransaction(const FString& mainAccountName,
    							const FString& receiverAddress,
                                const FUInt64& amount,
                                const FString& notes);   
	
	UPROPERTY(BlueprintAssignable, Category = "MultiCastDelegate")
        FSendPaymentTxDelegate SendPaymentTxCallback;

	/**
	 * get response after payment tx
	 * @param response txID after payment tx
	 */
    void OnSendPaymentTransactionCompleteCallback(const Vertices::VerticesSendPayTxResponse& response);

	/**
	 * send asset config TX to algorand node
	 * @param creator address which create token
	 * @param manager address which take token 
	 * @param reserve address which take token 
	 * @param freeze address which take token 
	 * @param clawback address which take token 
	 * @param asset_id of token to be sent
	 * @param total amount of token to be sent
	 * @param decimals of token to be sent
	 * @param unit_name of token to be sent
	 * @param asset_name of token to be sent
	 * @param url of token to be sent
	 * @param notes tx description when send payment tx
	 */
	UFUNCTION(BlueprintCallable,
			  meta = (DisplayName = "Send Asset Config TX", Keywords = "acfg"),
			  Category = "AlgorandUnrealManager")
	void sendAssetConfigTransaction(const FString& mainAccountName,
									const FString& manager,
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
									const FString& notes);

    /**
     * asset transfer transaction information callback
     */ 
	UPROPERTY(BlueprintAssignable, Category = "MultiCastDelegate")
	FSendAssetConfigTxDelegate SendAssetConfigTransactionCallback;

	/**
	 * get response after asset config tx
	 * @param response txID after asset config tx
	 */
	void OnSendAssetConfigTransactionCompleteCallback(const Vertices::VerticesSendAcfgTxResponse& response);

	/**
	 * send asset transfer TX to algorand node
	 * @param senderAddress address which take token
	 * @param receiverAddress address which take token 
	 * @param asset_ID of token to be sent
	 * @param amount of token to be sent
	 * @param notes tx description when send payment tx
	 */
	UFUNCTION(BlueprintCallable,
			  meta = (DisplayName = "Send Asset Transfer TX", Keywords = "axfer"),
			  Category = "AlgorandUnrealManager")
	void sendAssetTransferTransaction(const FString& mainAccountName,
								const FString& senderAddress,
								const FString& receiverAddress,
								const FUInt64& asset_ID,
								const FString& amount,
								const FString& notes);   
	
	UPROPERTY(BlueprintAssignable, Category = "MultiCastDelegate")
	FSendAssetTransferTxDelegate SendAssetTransferTransactionCallback;

	/**
	 * get response after asset transfer tx
	 * @param response txID after asset transfer tx
	 */
	void OnSendAssetTransferTransactionCompleteCallback(const Vertices::VerticesSendAxferTxResponse& response);

	/**
	 * send application call TX to algorand node
	 * @param app_ID application id created by algorand node
	 */
    UFUNCTION(BlueprintCallable,
			  meta = (DisplayName = "Send Application Call TX", Keywords = "applcall"),	
    		  Category = "AlgorandUnrealManager")
    void sendApplicationCallTransaction(const FString& mainAccountName,
    									const FUInt64& app_ID,
    									const TArray<FAppArg>& app_Args,
    									const EAppOnCompleteTX& app_complete_tx);
	
	UPROPERTY(BlueprintAssignable, Category = "MultiCastDelegate")
        FSendApplicationCallTxDelegate SendApplicationCallTransactionCallback;

	/**
	 * get response after application call tx
	 * @param response txID after application call tx
	 */ 
    void OnSendApplicationCallTransactionCompleteCallback(const Vertices::VerticesSendApplCallTxResponse& response);

	/**
	 * send request to algorand node to fetch arc-asset details
	 * @param asset_ID asset id to be fetched from algorand node
	 */
	UFUNCTION(BlueprintCallable,
			  meta = (DisplayName = "Fetch ArcAsset Details", Keywords = "arcasset"),	
			  Category = "AlgorandUnrealManager")
	void fetchArcAssetDetails(const FUInt64& asset_ID);
	
	UPROPERTY(BlueprintAssignable, Category = "MultiCastDelegate")
	FArcAssetDetailsDelegate FetchArcAssetDetailsCallback;

	/**
	 * get response after arc asset details
	 * @param response after arc asset details
	 */ 
	void OnFetchArcAssetDetailsCompleteCallback(const Vertices::VerticesArcAssetDetailsResponse& response);

	/**
	 * send request to algorand node to fetech account information
	 * @param asset_ID asset id to be fetched from algorand node
	 */
	UFUNCTION(BlueprintCallable,
			  meta = (DisplayName = "Fetch Account Information", Keywords = "asset"),	
			  Category = "AlgorandUnrealManager")
	void fetchAccountInformation(const FString& address);
	
	UPROPERTY(BlueprintAssignable, Category = "MultiCastDelegate")
	FAccountInfoDelegate FetchAccountInformationCallback;

	/**
	 * get response after account information
	 * @param response after account information
	 */ 
	void OnFetchAccountInformationCompleteCallback(const Vertices::VerticesAccountInformationResponse& response);

	/**
	 * broadcast @link ErrorDelegateCallback and leave logs
	 * @par Description explains error msg
	 * @param Msg is an error msg
	 **/
	void processErrorCallback(const FString& Description, const FString& Msg);
	
	/**
	 * return world of outer
	 * @return outer world
	 */ 
    UWorld* GetWorld() const override;

private:
	// algorand rpc information
    FString myAlgodRpc;
    FUInt64 myAlgodPort;
    FString myAlgodTokenHeader;

	FString myIndexerRpc;
	FUInt64 myIndexerPort;
	FString myIndexerTokenHeader;

	// Algorand modules
    TSharedPtr<TransactionBuilder> transactionBuilder_;
    TSharedPtr<algorand::api::UnrealApi> unrealApi_;
    TSharedPtr<algorand::vertices::VerticesSDK> vertices_;

	// context manager to send api request 
    RequestContextManager requestContextManager_;
     
};