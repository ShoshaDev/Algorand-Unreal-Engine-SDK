// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	algorandHandler = CreateDefaultSubobject<UAlgorandHandler>(TEXT("AlgorandHandler"));

	Result_Type = EResultType::InitWallet;
	Password = "algorand-sdk";
	AccountName = "dragon";
	Mnemonics = "rally relief lucky maple primary chair syrup economy tired hurdle slot upset clever chest curve bitter weekend prepare movie letter lamp alert then able taste";
	Address = "A6KIDEH35E56GWUDYZCDFVTLKDIC7P5HQRHGCIM4PVALCRTE2HZBFE7CKM";
	AssetID = "779312090";
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	RunState = ERunState::READY;
	
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(RunState == ERunState::START)
	{
		algorandHandler->Result_Type = Result_Type;
		algorandHandler->Password = Password;
		algorandHandler->AccountName = AccountName;
		algorandHandler->Mnemonics = Mnemonics;
		algorandHandler->Address = Address;
		algorandHandler->AssetID = UUInt64Factory::UInt64FromString(AssetID);
		
		algorandHandler->RunSomeLogic();
		RunState = ERunState::READY;
	}
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

