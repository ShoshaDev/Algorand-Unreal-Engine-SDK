#pragma once

#define PACKED
#pragma pack(push, 1)
#include "FUInt64.h"
#include "FResultType.generated.h"

/**
 * @brief user-defined FArcAssetDetailsStruct
 */

UENUM(BlueprintType)
enum class EResultType : uint8 {
	NONE = 0 UMETA(Hidden),
	InitWallet = 1 UMETA(DisplayName = "InitWallet"),
	LoadWallet = 2 UMETA(DisplayName = "LoadWallet"),
	SaveWallet = 3 UMETA(DisplayName = "SaveWallet"),
	GetMnemonics = 4 UMETA(DisplayName = "GetMneominicsByAccountName"),
	GetAllAccounts = 5 UMETA(DisplayName = "GetAllAccounts"),
	GenRandomAccount = 6 UMETA(DisplayName = "GenerateRandomAccount"),
	GenAccount = 7 UMETA(DisplayName = "GenerateAccountFromMnemonics"),
	AddrBalance = 8 UMETA(DisplayName = "GetAddressBalance"),
	PayTx = 9 UMETA(DisplayName = "SendPaymentTx"),
	AcfgTx = 10 UMETA(DisplayName = "SendAssetConfigTx"),
	AxferTx = 11 UMETA(DisplayName = "SendAssetTransferTx"),
	ApplTx = 12 UMETA(DisplayName = "SendApplicationTx"),
	ArcAsset = 13 UMETA(DisplayName = "ArcAsset"),
	AccAssets = 14 UMETA(DisplayName = "AccountAssets"),
	RemAccount = 15 UMETA(DisplayName = "RemoveAccountByName")
};

USTRUCT(BlueprintType)
struct ALGORAND_API FResultBoolean {
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	bool Result;
	
	FResultBoolean();
	FResultBoolean(const bool& Result);
	static FResultBoolean ToResultBoolean(const bool& Result);
};

USTRUCT(BlueprintType)
struct ALGORAND_API FResultString {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString Result;
	FResultString();
	
	FResultString(const FString& Result);
	static FResultString ToResultString(const FString& Result);
};

USTRUCT(BlueprintType)
struct ALGORAND_API FResultUInt64 {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite)
	FUInt64 Result;
	
	FResultUInt64();
	FResultUInt64(const FUInt64& Result);
	static FResultUInt64 ToResultUInt64(const FUInt64& Result);
};

USTRUCT(BlueprintType)
struct ALGORAND_API FResultAccount {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString Address;
	UPROPERTY(BlueprintReadWrite)
	FString Name;
	UPROPERTY(BlueprintReadWrite)
	TArray<FString> MnemonicPhrase;
	
	FResultAccount();
	FResultAccount(const FString& Address, const FString& Name, const TArray<FString>& MnemonicPhrase);
	static FResultAccount ToResultAccount(const FString& Address, const FString& Name, const TArray<FString>& MnemonicPhrase);
};

USTRUCT(BlueprintType)
struct ALGORAND_API FResultAccounts {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite)
	TArray<FString> Addresses;
	UPROPERTY(BlueprintReadWrite)
	TArray<FString> Names;
	
	FResultAccounts();
	FResultAccounts(const TArray<FString>& Names, const TArray<FString>& Addresses);
	static FResultAccounts ToResultAccounts(const TArray<FString>& Names, const TArray<FString>& Addresses);
};

USTRUCT(BlueprintType)
struct ALGORAND_API FResultAcfgTx {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString TxID;
	UPROPERTY(BlueprintReadWrite)
	FUInt64 AssetID;
	
	FResultAcfgTx();
	FResultAcfgTx(const FString& TxID, const FUInt64& AssetID);
	static FResultAcfgTx ToResultAcfgTx(const FString& TxID, const FUInt64& AssetID);
};

USTRUCT(BlueprintType)
struct ALGORAND_API FResultApplTx {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString TxID;
	UPROPERTY(BlueprintReadWrite)
	FString Logs;
	
	FResultApplTx();
	FResultApplTx(const FString& TxID, const FString& Logs);
	static FResultApplTx ToResultApplTx(const FString& TxID, const FString& Logs);
};

USTRUCT(BlueprintType)
struct ALGORAND_API FResultAccAssets {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite)
	TArray<FString> AssetIDs;
	UPROPERTY(BlueprintReadWrite)
	TArray<FString> AssetNames;
	
	FResultAccAssets();
	FResultAccAssets(const TArray<FString>& AssetIDs, const TArray<FString>& AssetNames);
	static FResultAccAssets ToResultAccAssets(const TArray<FString>& AssetIDs, const TArray<FString>& AssetNames);
};


#pragma pack(pop)
#undef PACKED