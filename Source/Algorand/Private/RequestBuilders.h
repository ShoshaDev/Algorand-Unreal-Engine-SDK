// Copyright 2022, Wisdom Labs. All Rights Reserved

#pragma once
#include "Models/FUInt64.h"
#include "..\Public\Models\FAppNoOpTX.h"
#include "VerticesSDK.h"

/**
 * ToDo:
 * @brief for asset bases on transactions and wallet provider
 * add requests for new txs
 * Key Registration, Asset Configuration, Freeze and Transafer, State Proof
 */

/**
 * Request Builder
 * how this builds request for wallet and txs
 * according to Request of Vertices Library
 */

namespace request_builders {
	
	algorand::vertices::VerticesSDK::VerticesInitWalletRequest
	buildInitWalletRequest();
	
	algorand::vertices::VerticesSDK::VerticesLoadWalletRequest
	buildLoadWalletRequest(const FString& mnemonics);
	
	algorand::vertices::VerticesSDK::VerticesSaveWalletRequest
	buildSaveWalletRequest(const FString& mnemonics);
	
	algorand::vertices::VerticesSDK::VerticesGetMnemonicsByAccountNameRequest
	buildGetMnemonicsByAccountNameRequest(const FString& Name);
	
	algorand::vertices::VerticesSDK::VerticesGenerateAccountFromMnemonicsRequest
	buildGenerateAccountFromMnemonicsRequest(const FString& Mnemonics, const FString& Name);

	algorand::vertices::VerticesSDK::VerticesGenerateRandomAccountRequest
	buildGenerateRandomAccountRequest(const FString& Name);
	
	algorand::vertices::VerticesSDK::VerticesGetAddrBalanceRequest
	buildGetBalanceRequest(const FString& address);
	
	algorand::vertices::VerticesSDK::VerticesSendPayTxRequest
		buildPaymentTransactionRequest(const FString& mainAccountName,
									   const FString& receiver,
									   const FUInt64& amount,
									   const FString& notes);
	
	 algorand::vertices::VerticesSDK::VerticesSendAcfgTxRequest
		buildAssetConfigTransactionRequest(const FString& mainAccountName, 
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
	
	algorand::vertices::VerticesSDK::VerticesSendAxferTxRequest
		buildAssetTransferTransactionRequest(const FString& mainAccountName,
		                                     const FString& sender,
		                                     const FString& receiver,
		                                     const FUInt64& asset_id,
		                                     const FString& amount,
		                                     const FString& notes);
	
	algorand::vertices::VerticesSDK::VerticesSendApplCallTxRequest
	buildApplicationCallTransactionRequest(const FString& mainAccountName,
	                                       const FUInt64& app_ID,
	                                       const TArray<FAppArg>& app_args,
	                                       const EAppOnCompleteTX& app_complete_tx);
	
	algorand::vertices::VerticesSDK::VerticesArcAssetDetailsRequest
		buildArcAssetDetailsRequest(const FUInt64& asset_ID);

	algorand::vertices::VerticesSDK::VerticesAccountInformationRequest
		buildAccountInformationRequest(const FString& address);
}