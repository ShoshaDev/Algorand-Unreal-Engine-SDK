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
	GenAccount = 5 UMETA(DisplayName = "GenerateAccountFromMnemonics"),
	AddrBalance = 6 UMETA(DisplayName = "GetAddressBalance"),
	PayTx = 7 UMETA(DisplayName = "SendPaymentTx"),
	AcfgTx = 8 UMETA(DisplayName = "SendAssetConfigTx"),
	AxferTx = 9 UMETA(DisplayName = "SendAssetTransferTx"),
	ApplTx = 10 UMETA(DisplayName = "SendApplicationTx"),
	ArcAsset = 11 UMETA(DisplayName = "ArcAsset"),
	AccAssets = 12 UMETA(DisplayName = "AccountAssets")
};

USTRUCT(BlueprintType)
struct ALGORAND_API FResultBoolean {
	GENERATED_USTRUCT_BODY()
	
	bool Result;
	FResultBoolean();
	FResultBoolean(const bool& Result);
	static FResultBoolean ToResultBoolean(const bool& Result);
};

USTRUCT(BlueprintType)
struct ALGORAND_API FResultString {
	GENERATED_USTRUCT_BODY()
	
	FString Result;
	FResultString();
	FResultString(const FString& Result);
	static FResultString ToResultString(const FString& Result);
};

USTRUCT(BlueprintType)
struct ALGORAND_API FResultUInt64 {
	GENERATED_USTRUCT_BODY()
	
	FUInt64 Result;
	FResultUInt64();
	FResultUInt64(const FUInt64& Result);
	static FResultUInt64 ToResultUInt64(const FUInt64& Result);
};

USTRUCT(BlueprintType)
struct ALGORAND_API FResultAccount {
	GENERATED_USTRUCT_BODY()
	
	FString Address;
	FString Name;
	FResultAccount();
	FResultAccount(const FString& Address, const FString& Name);
	static FResultAccount ToResultAccount(const FString& Address, const FString& Name);
};

USTRUCT(BlueprintType)
struct ALGORAND_API FResultAcfgTx {
	GENERATED_USTRUCT_BODY()
	
	FString TxID;
	FUInt64 AssetID;
	FResultAcfgTx();
	FResultAcfgTx(const FString& TxID, const FUInt64& AssetID);
	static FResultAcfgTx ToResultAcfgTx(const FString& TxID, const FUInt64& AssetID);
};

USTRUCT(BlueprintType)
struct ALGORAND_API FResultApplTx {
	GENERATED_USTRUCT_BODY()
	
	FString TxID;
	FString Result;
	FResultApplTx();
	FResultApplTx(const FString& TxID, const FString& Result);
	static FResultApplTx ToResultApplTx(const FString& TxID, const FString& Result);
};

USTRUCT(BlueprintType)
struct ALGORAND_API FResultAccAssets {
	GENERATED_USTRUCT_BODY()
	
	TArray<FString> AssetIDs;
	TArray<FString> AssetNames;
	FResultAccAssets();
	FResultAccAssets(const TArray<FString>& AssetIDs, const TArray<FString>& AssetNames);
	static FResultAccAssets ToResultAccAssets(const TArray<FString>& AssetIDs, const TArray<FString>& AssetNames);
};


#pragma pack(pop)
#undef PACKED