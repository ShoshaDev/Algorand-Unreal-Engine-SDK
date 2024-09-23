// Copyright 2022, Wisdom Labs. All Rights Reserved

#pragma once
#include "BaseModel.h"
#include "CoreMinimal.h"
#include "Misc/TVariant.h"
#include "UnrealApi.h"

/**
 * ToDo:
 * for asset bases on transactions and wallet provider
 * add like txs
 * Key Registration, Asset Configuration, Freeze and Transafer, State Proof
 */

/**
 * declare ApiDelegate and Request to build request context
 */

/// declaring templates of Api Delegate as TVariant
using ApiDelegate = TVariant<
	algorand::api::UnrealApi::FAlgorandAPIInitWalletDelegate,
	algorand::api::UnrealApi::FAlgorandAPIWalletExistenceDelegate,
	algorand::api::UnrealApi::FAlgorandAPILoadWalletDelegate,
	algorand::api::UnrealApi::FAlgorandAPISaveWalletDelegate,
	algorand::api::UnrealApi::FAlgorandAPIGetMnemonicsByAccountNameDelegate,
	algorand::api::UnrealApi::FAlgorandAPIGetAllAccountsDelegate,
	algorand::api::UnrealApi::FAlgorandAPIGenerateRandomAccountDelegate,
	algorand::api::UnrealApi::FAlgorandAPIGenerateAccountFromMnemonicsDelegate,
	algorand::api::UnrealApi::FAlgorandAPIGetAddrBalanceDelegate,
	algorand::api::UnrealApi::FAlgorandAPISendPayTxDelegate,
	algorand::api::UnrealApi::FAlgorandAPISendAcfgTxDelegate,
	algorand::api::UnrealApi::FAlgorandAPISendAxferTxDelegate,
	algorand::api::UnrealApi::FAlgorandAPISendApplCallTxDelegate,
	algorand::api::UnrealApi::FAlgorandAPIArcAssetDetailsDelegate,
	algorand::api::UnrealApi::FAlgorandAPIAccountInformationDelegate,
	algorand::api::UnrealApi::FAlgorandAPIRemoveAccountByNameDelegate
>;

/// Request Contxet info
struct RequestContext {

	/// request pointer 
	TSharedPtr<algorand::vertices::Request>  request;
 
	///delegate callback
	TSharedPtr<ApiDelegate> delegate;
};