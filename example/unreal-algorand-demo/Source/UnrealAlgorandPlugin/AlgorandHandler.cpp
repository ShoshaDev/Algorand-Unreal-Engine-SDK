#include "AlgorandHandler.h"

#include "..\..\Plugins\Algorand\Source\Algorand\Public\Models\FAppNoOpTX.h"

UAlgorandHandler::UAlgorandHandler()
{
	algorandManager =
		CreateDefaultSubobject<UAlgorandUnrealManager>(TEXT("AlgorandManager"));
	
	BooleanDelegate.BindUFunction(this, FName("OnResultBooleanCallback"));
	StringDelegate.BindUFunction(this, FName("OnResultStringCallback"));
	UInt64Delegate.BindUFunction(this, FName("OnResultUInt64Callback"));
	AccountDelegate.BindUFunction(this, FName("OnResultAccountCallback"));
	AccountsDelegate.BindUFunction(this, FName("OnResultAccountsCallback"));
	AcfgTxDelegate.BindUFunction(this, FName("OnResultAcfgTxCallback"));
	ApplTxDelegate.BindUFunction(this, FName("OnResultApplTxCallback"));
	ArcAssetsDelegate.BindUFunction(this, FName("OnResultArcAssetDetailsCallback"));
	AccAssetsDelegate.BindUFunction(this, FName("OnResultAccAssetsCallback"));
	ErrorDelegate.BindUFunction(this, FName("OnErrorCallback"));

	algorandManager->ErrorDelegateCallback.Add(ErrorDelegate);
	algorandManager->InitWalletCallback.Add(BooleanDelegate);
	algorandManager->LoadWalletCallback.Add(BooleanDelegate);
	algorandManager->SaveWalletCallback.Add(BooleanDelegate);
	algorandManager->GetMnemonicsByAccountNameCallback.Add(StringDelegate);
	algorandManager->GetAllAccountsCallback.Add(AccountsDelegate);
	algorandManager->GenerateRandomAccountCallback.Add(AccountDelegate);
	algorandManager->GenerateAccountFromMnemonicsCallback.Add(AccountDelegate);
	algorandManager->GetAddressBalanceCallback.Add(UInt64Delegate);
	algorandManager->SendPaymentTxCallback.Add(StringDelegate);
	algorandManager->SendAssetConfigTransactionCallback.Add(AcfgTxDelegate);
	algorandManager->SendAssetTransferTransactionCallback.Add(StringDelegate);
	algorandManager->SendApplicationCallTransactionCallback.Add(ApplTxDelegate);
	algorandManager->FetchArcAssetDetailsCallback.Add(ArcAssetsDelegate);
	algorandManager->FetchAccountInformationCallback.Add(AccAssetsDelegate);
	algorandManager->RemoveAccountByNameCallback.Add(BooleanDelegate);
}

void UAlgorandHandler::OnResultBooleanCallback(const EResultType& ResultType, const FResultBoolean& Result)
{
	switch (ResultType)
	{
	case EResultType::InitWallet:
		UE_LOG(LogTemp, Display, TEXT("🚀 Algo Wallet was initialized."));
		break;
	case EResultType::LoadWallet:
		UE_LOG(LogTemp, Display, TEXT("🚀 Algo Wallet was loaded."));
		break;
	case EResultType::SaveWallet:
		UE_LOG(LogTemp, Display, TEXT("🚀 Algo Wallet was saved."));
		break;
	case EResultType::RemAccount:
		UE_LOG(LogTemp, Display, TEXT("🚀 Algo Account was removed."));
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("🙉 FResultBoolean: Unexpected Result Type"));
	}
}

void UAlgorandHandler::OnResultStringCallback(const EResultType& ResultType, const FResultString& Result)
{
	switch (ResultType)
	{
	case EResultType::GetMnemonics:
		UE_LOG(LogTemp, Display, TEXT("🚀 Fetched mnemonics are following: %s"), *Result.Result);
		break;
	case EResultType::PayTx:
		UE_LOG(LogTemp, Display, TEXT("🚀 Payment TX Hash are following: %s"), *Result.Result);
		break;
	case EResultType::AxferTx:
		UE_LOG(LogTemp, Display, TEXT("🚀 Asset Transfer TX Hash are following: %s"), *Result.Result);
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("🙉 FResultBoolean: Unexpected Result Type"));
	}
}

void UAlgorandHandler::OnResultAccountCallback(const EResultType& ResultType, const FResultAccount& Result)
{
	switch (ResultType)
	{
	case EResultType::GenAccount:
		UE_LOG(LogTemp, Display, TEXT("🚀 Generated address and account name: %s -- %s\n"), *Result.Address,
		       *Result.Name);
		if(!Result.MnemonicPhrase.IsEmpty())
		{
			UE_LOG(LogTemp, Display, TEXT("🚀☀️ Mnemonic phrase of the account: %s"), *FString::Join(Result.MnemonicPhrase, TEXT(" ")));	
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("🚀☀️ Mnemonic phrase of the account doesn't exist in this Callback"));
		}
			
		break;
	case EResultType::GenRandomAccount:
		UE_LOG(LogTemp, Display, TEXT("🚀 Generated address and account name randomly: %s -- %s"), *Result.Address,
		       *Result.Name);
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("🙉 FResultAccount: Unexpected Result Type"));
	}
}

void UAlgorandHandler::OnResultAccountsCallback(const EResultType& ResultType, const FResultAccounts& Result)
{
	switch (ResultType)
	{
	case EResultType::GetAllAccounts:
		UE_LOG(LogTemp, Display, TEXT("🚀 Accounts were fetched.\n"));
		
		for(size_t i = 0; i < Result.Names.Num(); i++)
		{
			UE_LOG(LogTemp, Display, TEXT("🚀☀️ Account: %s -- %s\n"), *Result.Names[i], *Result.Addresses[i]);
		}
		
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("🙉 FResultAccount: Unexpected Result Type"));
	}
}


void UAlgorandHandler::OnResultUInt64Callback(const EResultType& ResultType, const FResultUInt64& Result)
{
	switch (ResultType)
	{
	case EResultType::AddrBalance:
		UE_LOG(LogTemp, Display, TEXT("🚀 Got Balance: %llu"), Result.Result.Value);
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("🙉 FResultUInt64: Unexpected Result Type"));
	}
}

void UAlgorandHandler::OnResultAcfgTxCallback(const EResultType& ResultType, const FResultAcfgTx& Result)
{
	switch (ResultType)
	{
	case EResultType::AcfgTx:
		UE_LOG(LogTemp, Display, TEXT("🚀 Fetched Asset Config TX Hash and Asset ID: %s -- %llu"), *Result.TxID,
		       Result.AssetID.Value);
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("🙉 FResultAcfgTx: Unexpected Result Type"));
	}
}

void UAlgorandHandler::OnResultApplTxCallback(const EResultType& ResultType, const FResultApplTx& Result)
{
	switch (ResultType)
	{
	case EResultType::ApplTx:
		{
			FString App_Result = UAppNoOpTXFactory::FAppResultFromLogs(EAppLogType::LOG_INT, Result.Logs);
			UE_LOG(LogTemp, Display, TEXT("🚀 Application Call TX ID & TX LOG: %s -- %s"), *Result.TxID, *App_Result);
			break;
		}
	default:
		UE_LOG(LogTemp, Warning, TEXT("🙉 FResultApplTx: Unexpected Result Type"));
	}
}

void UAlgorandHandler::OnResultArcAssetDetailsCallback(const EResultType& ResultType, const FArcAssetDetails& Result)
{
	switch (ResultType)
	{
	case EResultType::ArcAsset:
		UE_LOG(LogTemp, Display, TEXT("🚀 Fetched Arc Asset Details: %s"), *Result.Description);
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("🙉 FArcAssetDetails: Unexpected Result Type"));
	}
}

void UAlgorandHandler::OnResultAccAssetsCallback(const EResultType& ResultType, const FResultAccAssets& Result)
{
	switch (ResultType)
	{
	case EResultType::AccAssets:
		for (int i = 0; i < Result.AssetIDs.Num(); i++)
			UE_LOG(LogTemp, Display, TEXT("🚀 Arc Account Info: %s -- %s \n"), *Result.AssetIDs[i], *Result.AssetNames[i]);
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("🙉 FResultAccAssets: Unexpected Result Type"));
	}
}

void UAlgorandHandler::OnErrorCallback(const FError& Error)
{
	UE_LOG(LogTemp, Error, TEXT("☠️ FError: Error occured, %s"), *Error.message);
}

void UAlgorandHandler::RunSomeLogic()
{
	switch (Result_Type)
	{
	case EResultType::InitWallet:
		algorandManager->initWallet();
		break;
	case EResultType::LoadWallet:
		algorandManager->loadWallet(Password);
		break;
	case EResultType::SaveWallet:
		algorandManager->saveWallet(Password);
		break;
	case EResultType::GetMnemonics:
		algorandManager->getMnemonicsByAccountName(AccountName);
		break;
	case EResultType::GetAllAccounts:
		algorandManager->getAllAccounts();
		break;
	case EResultType::GenAccount:
		algorandManager->generateAccountFromMnemonics(Mnemonics, AccountName);
		break;
	case EResultType::GenRandomAccount:
		algorandManager->generateRandomAccount(AccountName);
		break;
	case EResultType::AddrBalance:
		algorandManager->getAddressBalance(Address);
		break;
	case EResultType::PayTx:
		algorandManager->sendPaymentTransaction(AccountName,
		                                        Address,
		                                        Amount, FString(
			                                        "Sent 100 algo to " + Address));
		break;
	case EResultType::AcfgTx:
		algorandManager->sendAssetConfigTransaction(AccountName,
			FString("Z5C3T63QHLEAAXV2A2L3Y2LY6TURAPEESMQZC7777PGGPPAAGC4F4GWHHA"),
			FString("Z5C3T63QHLEAAXV2A2L3Y2LY6TURAPEESMQZC7777PGGPPAAGC4F4GWHHA"),
			FString("Z5C3T63QHLEAAXV2A2L3Y2LY6TURAPEESMQZC7777PGGPPAAGC4F4GWHHA"),
			FString("Z5C3T63QHLEAAXV2A2L3Y2LY6TURAPEESMQZC7777PGGPPAAGC4F4GWHHA"),
			0,
			1000,
			8,
			"false",
			FString("USD"),
			FString("Shosha"),
			FString("https://myurl.com"),
			FString("fe"));
		break;
	case EResultType::AxferTx:
		algorandManager->sendAssetTransferTransaction(AccountName,
			Address,
			AssetID,
			Amount,
			FString("Sent tokens to " + Address));

		break;
	case EResultType::ApplTx:
		{
			// Application Call - Math Calculation through Smart Contract on algorand testnet node
			TArray<FAppArg> app_args;
			EAppOnCompleteTX app_complete_tx = EAppOnCompleteTX::NOOP;

			app_args.Add(UAppNoOpTXFactory::FAppArgFromString(EAppArgType::ARG_METHOD, "mul(uint64,uint64)uint64"));
			app_args.Add(UAppNoOpTXFactory::FAppArgFromString(EAppArgType::ARG_INT, "1000"));
			app_args.Add(UAppNoOpTXFactory::FAppArgFromString(EAppArgType::ARG_INT, "2"));

			algorandManager->sendApplicationCallTransaction(AccountName, 301624623, app_args, app_complete_tx);
			break;
		}
	case EResultType::ArcAsset:
		algorandManager->fetchArcAssetDetails(AssetID);
		break;
	case EResultType::AccAssets:
		algorandManager->fetchAccountInformation("6WII6ES4H6UW7G7T7RJX63CUNPKJEPEGQ3PTYVVU3JHJ652W34GCJV5OVY");
		break;
	case EResultType::RemAccount:
		algorandManager->removeAccountByName(AccountName);
		break;
	case EResultType::NONE:
		break;
	}
}

UWorld* UAlgorandHandler::GetWorld() const { return GetOuter()->GetWorld(); }
