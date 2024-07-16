// Copyright 2022, Wisdom Labs. All Rights Reserved

#pragma once
#include "VerticesSDK.h"

namespace response_builders {
	algorand::vertices::VerticesSDK::VerticesInitWalletResponse
		buildInitWalletResponse();
	algorand::vertices::VerticesSDK::VerticesLoadWalletResponse
		buildLoadWalletResponse();
	algorand::vertices::VerticesSDK::VerticesSaveWalletResponse
		buildSaveWalletResponse();
	algorand::vertices::VerticesSDK::VerticesGenerateAccountFromMnemonicsResponse
		buildGenerateAccountFromMnemonicsResponse(const FString& Address, const FString& Name);
	algorand::vertices::VerticesSDK::VerticesGetMnemonicsByAccountNameResponse
		buildGetMnemonicsByAccountNameResponse(const FString& Mnemonics, const FString& Name);
	algorand::vertices::VerticesSDK::VerticesGetAddrBalanceResponse
		buildGetAddrBalanceResponse(const FString& Address, const uint64& Amount);
	algorand::vertices::VerticesSDK::VerticesSendPayTxResponse
		buildSendPayTxResponse(const FString& TxID);
	algorand::vertices::VerticesSDK::VerticesSendAcfgTxResponse
		buildAcfgTxResponse(const FString& TxID, const uint64& AssetID);
	algorand::vertices::VerticesSDK::VerticesSendAxferTxResponse
		buildAxferTxResponse(const FString& TxID);
	algorand::vertices::VerticesSDK::VerticesSendApplCallTxResponse
		buildSendApplCallTxResponse(const FString& TxID, const FString& Logs);
}