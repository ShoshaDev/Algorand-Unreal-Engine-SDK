#pragma once

#include "Models/FArcAssetDetails.h"
#include "Models/FUInt64.h"
#include "CoreMinimal.h"
#include "UAlgorandUnrealManager.h"

#include "AlgorandHandler.generated.h"

UCLASS(minimalapi)
class UAlgorandHandler : public UObject {
	GENERATED_BODY()

public:
	UAlgorandHandler();

	UPROPERTY()
		UAlgorandUnrealManager* algorandManager;
	
	UWorld* GetWorld() const override;
	
	UFUNCTION(BlueprintCallable, Category = "AlgorandHandler")
	void RunSomeLogic();

	UFUNCTION(BlueprintCallable, Category = "AlgorandHandler")
		void OnResultBooleanCallback(const EResultType& ResultType, const FResultBoolean& Result);

	UFUNCTION(BlueprintCallable, Category = "AlgorandHandler")
		void OnResultStringCallback(const EResultType& ResultType, const FResultString& Result);

	UFUNCTION(BlueprintCallable, Category = "AlgorandHandler")
		void OnResultAccountCallback(const EResultType& ResultType, const FResultAccount& Result);

	UFUNCTION(BlueprintCallable, Category = "AlgorandHandler")
		void OnResultAccountsCallback(const EResultType& ResultType, const FResultAccounts& Result);

	UFUNCTION(BlueprintCallable, Category = "AlgorandHandler")
		void OnResultUInt64Callback(const EResultType& ResultType, const FResultUInt64& Result);

	UFUNCTION(BlueprintCallable, Category = "AlgorandHandler")
		void OnResultAcfgTxCallback(const EResultType& ResultType, const FResultAcfgTx& Result);

	UFUNCTION(BlueprintCallable, Category = "AlgorandHandler")
		void OnResultApplTxCallback(const EResultType& ResultType, const FResultApplTx& Result);

	UFUNCTION(BlueprintCallable, Category = "AlgorandHandler")
		void OnResultArcAssetDetailsCallback(const EResultType& ResultType, const FArcAssetDetails& Result);

	UFUNCTION(BlueprintCallable, Category = "AlgorandHandler")
		void OnResultAccAssetsCallback(const EResultType& ResultType, const FResultAccAssets& Result);

	UFUNCTION(BlueprintCallable, Category = "AlgorandHandler")
		void OnErrorCallback(const FError& Error);

	
	EResultType Result_Type;
	FString Password;
	FString AccountName;
	FString Mnemonics;
	FString Address;
	FUInt64 AssetID;
	FUInt64 Amount;

	FScriptDelegate BooleanDelegate; // Init, Load, Save Wallet
	FScriptDelegate StringDelegate; // Get Mnemonics, PayTx, AxferTx
	FScriptDelegate UInt64Delegate; // Get Balance
	FScriptDelegate AccountDelegate; // GeneratedAccount
	FScriptDelegate AccountsDelegate; // Accounts
	FScriptDelegate AcfgTxDelegate; // AcfgTx
	FScriptDelegate ApplTxDelegate; // ApplTx
	FScriptDelegate ArcAssetsDelegate; // ArcAssets
	FScriptDelegate AccAssetsDelegate; // AccAssets
	FScriptDelegate ErrorDelegate;
};