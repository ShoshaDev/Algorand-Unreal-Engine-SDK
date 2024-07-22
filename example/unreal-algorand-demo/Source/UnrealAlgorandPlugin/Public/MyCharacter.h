// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../AlgorandHandler.h"
#include "Components/Button.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

UENUM(BlueprintType)
enum class ERunState : uint8 {
	READY = 0 UMETA(DisplayName = "READY"),
	START = 1 UMETA(DisplayName = "START")
};

UCLASS()
class UNREALALGORANDPLUGIN_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UAlgorandHandler* algorandHandler;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Get Mnemonics By Account Name */
	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		meta = (DisplayName = "ResultType", Keywords = "CLI ResultType"),
		Category = "AlgorandSDK")
	EResultType Result_Type;

	/** Be used to secure Algo Wallet */
	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		meta = (DisplayName = "Password", Keywords = "CLI Wallet"),
		Category = "AlgorandSDK")
	FString Password;

	/** Belongs to Algo Wallet */
	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		meta = (DisplayName = "AccountName", Keywords = "CLI Wallet"),
		Category = "AlgorandSDK")
	FString AccountName;

	/** Be used to create Algo Account */
	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		meta = (DisplayName = "Mnemonics", Keywords = "CLI Wallet" ),
		Category = "AlgorandSDK")
	FString Mnemonics;

	/** Get Mnemonics By Account Name */
	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		meta = (DisplayName = "Address", Keywords = "CLI Wallet"),
		Category = "AlgorandSDK")
	FString Address;

	/** Be used to fetch Assets. for ex: 779312090, 1019478822, 1092400027 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		meta = (DisplayName = "AssetID", Keywords = "CLI Asset"),
		Category = "AlgorandSDK")
	FString AssetID;

	/** Select Start to run current callback. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		meta = (DisplayName = "RunState", Keywords = "RunState"),
		Category = "AlgorandSDK")
	ERunState RunState;
};
