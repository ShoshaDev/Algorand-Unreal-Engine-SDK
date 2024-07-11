// Copyright 2022, Wisdom Labs. All Rights Reserved

#pragma once
#include "VerticesSDK.h"

namespace response_builders {
	algorand::vertices::VerticesSDK::VerticesInitWalletResponse
		buildInitWalletResponse(const FString& output);
	algorand::vertices::VerticesSDK::VerticesLoadWalletResponse
		buildLoadWalletResponse(const FString& output);
	algorand::vertices::VerticesSDK::VerticesSaveWalletResponse
		buildSaveWalletResponse(const FString& output);
	algorand::vertices::VerticesSDK::VerticesGenerateAccountFromMnemonicsResponse
		buildGenerateAccountFromMnemonicsResponse(const FString& output);
	algorand::vertices::VerticesSDK::VerticesGetMnemonicsByAccountNameResponse
		buildGetMnemonicsByAccountNameResponse(const FString& output);
	algorand::vertices::VerticesSDK::VerticesGetAddrBalanceResponse
		buildGetAddrBalanceResponse(const uint64& amount);
	algorand::vertices::VerticesSDK::VerticesSendPayTxResponse
		buildSendPayTxResponse(const FString& txID);
	algorand::vertices::VerticesSDK::VerticesSendAcfgTxResponse
		buildAcfgTxResponse(const FString& txID, const uint64& assetID);
	algorand::vertices::VerticesSDK::VerticesSendAxferTxResponse
		buildAxferTxResponse(const FString& txID);
	algorand::vertices::VerticesSDK::VerticesSendApplCallTxResponse
		buildSendApplCallTxResponse(const FString& txID, const FString& logs);
}