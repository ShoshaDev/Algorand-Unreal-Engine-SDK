#pragma once

#include "FUInt64.h"
#include "VerticesSDK.h"
#include "FArcAssetDetails.generated.h"

#define PACKED
#pragma pack(push, 1)

/**
 * @brief user-defined FArcAssetDetailsStruct
 */

UENUM(BlueprintType)
enum class EArcType : uint8 {
	Arc00 = 0 UMETA(Hidden),
	Arc03 = 1 UMETA(DisplayName = "Arc03"),
	Arc19 = 2 UMETA(DisplayName = "Arc19"),
	Arc69 = 3 UMETA(DisplayName = "Arc69")
};

USTRUCT(BlueprintType)
struct ALGORAND_API FArcAssetDetails
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ArcAsset")
	EArcType Standard;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ArcAsset")
	FString Unit_Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ArcAsset")
	FUInt64 Total;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ArcAsset")
	FUInt64 Decimals;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ArcAsset")
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ArcAsset")
	FString Clawback;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ArcAsset")
	FString Creator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ArcAsset")
	FString Freeze;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ArcAsset")
	FString Manager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ArcAsset")
	FString Reserve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ArcAsset")
	FString Media_Url;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ArcAsset")
	FString External_Url;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ArcAsset")
	FString Animation_Url;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ArcAsset")
	TMap<FString, FString> Properties;

	FArcAssetDetails();
	FArcAssetDetails(const algorand::vertices::VerticesSDK::VerticesArcAssetDetailsResponse& response);
	
};

USTRUCT(BlueprintType)
struct ALGORAND_API FCreatedAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CreatedAsset")
	FString ID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CreatedAsset")
	FString Name;
};

#pragma pack(pop)
#undef PACKED
