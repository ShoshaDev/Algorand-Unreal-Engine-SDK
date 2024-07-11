// Copyright 2022, Wisdom Labs. All Rights Reserved

#include "./ResponseBuilers.h"
#include "VerticesApiOperations.h"

namespace {
	using Vertices = algorand::vertices::VerticesSDK;
}

namespace response_builders {
	Vertices::VerticesInitWalletResponse
		buildInitWalletResponse(const FString& output)
	{
		Vertices::VerticesInitWalletResponse response;
		response.output = output;
		return response;
	}
	
	Vertices::VerticesLoadWalletResponse
		buildLoadWalletResponse(const FString& output)
	{
		Vertices::VerticesLoadWalletResponse response;
		response.output = output;
		return response;
	}

	Vertices::VerticesSaveWalletResponse
		buildSaveWalletResponse(const FString& output)
	{
		Vertices::VerticesSaveWalletResponse response;
		response.output = output;
		return response;
	}

	Vertices::VerticesGenerateAccountFromMnemonicsResponse
		buildGenerateAccountFromMnemonicsResponse(const FString& output)
	{
		Vertices::VerticesGenerateAccountFromMnemonicsResponse response;
		response.output = output;
		return response;
	}
	
	Vertices::VerticesGetMnemonicsByAccountNameResponse
		BuildGetMnemonicsByAccountNameResponse(const FString& output)
	{
		Vertices::VerticesGetMnemonicsByAccountNameResponse response;
		response.output = output;
		return response;
	}

	Vertices::VerticesGetAddrBalanceResponse
		buildGetAddrBalanceResponse(const uint64& amount)
	{
		Vertices::VerticesGetAddrBalanceResponse response;
		response.Amount = amount;
		return response;
	}

	Vertices::VerticesSendPayTxResponse
		buildSendPayTxResponse(const FString& txID)
	{
		Vertices::VerticesSendPayTxResponse response;
		response.txID = txID;
		return response;
	}

	Vertices::VerticesSendAcfgTxResponse
		buildSendAcfgTxResponse(const FString& txID, const uint64& assetID)
	{
		Vertices::VerticesSendAcfgTxResponse response;
		response.txID = txID;
		response.assetID = assetID;
		return response;
	}

	Vertices::VerticesSendAxferTxResponse
		buildSendAxferTxResponse(const FString& txID)
	{
		Vertices::VerticesSendAxferTxResponse response;
		response.txID = txID;
		return response;
	}

	Vertices::VerticesSendApplCallTxResponse
		buildSendApplCallTxResponse(const FString& txID, const FString& logs)
	{
		Vertices::VerticesSendApplCallTxResponse response;
		response.txID = txID;
		response.logs = logs;
		return response;
	}
}