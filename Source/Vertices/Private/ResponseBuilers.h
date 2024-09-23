// Copyright 2022, Wisdom Labs. All Rights Reserved

#pragma once
#include "VerticesSDK.h"

namespace response_builders {
	algorand::vertices::VerticesSDK::VerticesInitWalletResponse
		buildInitWalletResponse();
	algorand::vertices::VerticesSDK::VerticesWalletExistenceResponse
		buildWalletExistsResponse(bool Exists);
	algorand::vertices::VerticesSDK::VerticesLoadWalletResponse
		buildLoadWalletResponse();
	algorand::vertices::VerticesSDK::VerticesSaveWalletResponse
		buildSaveWalletResponse();
	algorand::vertices::VerticesSDK::VerticesGenerateAccountFromMnemonicsResponse
		buildGenerateAccountFromMnemonicsResponse(const FString& Address, const FString& Name);
	algorand::vertices::VerticesSDK::VerticesGenerateRandomAccountResponse
		buildGenerateRandomAccountResponse(const FString& Address, const FString& Name, const FString& Mnemonics);
	algorand::vertices::VerticesSDK::VerticesGetMnemonicsByAccountNameResponse
		buildGetMnemonicsByAccountNameResponse(const FString& Mnemonics, const FString& Name);
	algorand::vertices::VerticesSDK::VerticesGetAllAccountsResponse
		buildGetAllAccountsResponse(const TArray<FString>& Names, const TArray<FString>& Addresses);
	algorand::vertices::VerticesSDK::VerticesRemoveAccountByNameResponse
		buildRemoveAccountByNameResponse();
	algorand::vertices::VerticesSDK::VerticesGetAddrBalanceResponse
		buildGetAddrBalanceResponse(const FString& Address, const uint64& Amount);
	algorand::vertices::VerticesSDK::VerticesSendPayTxResponse
		buildSendPayTxResponse(const FString& TxID);
	algorand::vertices::VerticesSDK::VerticesSendAcfgTxResponse
		buildSendAcfgTxResponse(const FString& TxID, const uint64& AssetID);
	algorand::vertices::VerticesSDK::VerticesSendAxferTxResponse
		buildSendAxferTxResponse(const FString& TxID);
	algorand::vertices::VerticesSDK::VerticesSendApplCallTxResponse
		buildSendApplCallTxResponse(const FString& TxID, const FString& Logs);
}