// Copyright 2022, Wisdom Labs. All Rights Reserved

#include "./ResponseBuilers.h"
#include "VerticesApiOperations.h"

namespace {
	using Vertices = algorand::vertices::VerticesSDK;
}

namespace response_builders {
	Vertices::VerticesInitWalletResponse
		buildInitWalletResponse()
	{
		return Vertices::VerticesInitWalletResponse();
	}

	Vertices::VerticesWalletExistenceResponse
		buildWalletExistsResponse(bool Exists)
	{
		return Vertices::VerticesWalletExistenceResponse(Exists);
	}
	
	Vertices::VerticesLoadWalletResponse
		buildLoadWalletResponse()
	{
		return Vertices::VerticesLoadWalletResponse();
	}

	Vertices::VerticesSaveWalletResponse
		buildSaveWalletResponse()
	{
		return Vertices::VerticesSaveWalletResponse();
	}

	Vertices::VerticesGenerateAccountFromMnemonicsResponse
		buildGenerateAccountFromMnemonicsResponse(const FString& Address, const FString& Name)
	{
		return Vertices::VerticesGenerateAccountFromMnemonicsResponse(Address, Name);
	}

	Vertices::VerticesGenerateRandomAccountResponse
		buildGenerateRandomAccountResponse(const FString& Address, const FString& Name, const FString& Mnemonics)
	{
		return Vertices::VerticesGenerateRandomAccountResponse(Address, Name, Mnemonics);
	}
	
	Vertices::VerticesGetMnemonicsByAccountNameResponse
		buildGetMnemonicsByAccountNameResponse(const FString& Mnemonics, const FString& Name)
	{
		return Vertices::VerticesGetMnemonicsByAccountNameResponse(Mnemonics, Name);
	}

	Vertices::VerticesGetAllAccountsResponse
		buildGetAllAccountsResponse(const TArray<FString>& Names, const TArray<FString>& Addresses)
	{
		return Vertices::VerticesGetAllAccountsResponse(Names, Addresses);
	}

	Vertices::VerticesRemoveAccountByNameResponse
		buildRemoveAccountByNameResponse()
	{
		return Vertices::VerticesRemoveAccountByNameResponse();
	}

	Vertices::VerticesGetAddrBalanceResponse
		buildGetAddrBalanceResponse(const FString& Address, const uint64& Amount)
	{
		return Vertices::VerticesGetAddrBalanceResponse(Address, Amount);
	}

	Vertices::VerticesSendPayTxResponse
		buildSendPayTxResponse(const FString& TxID)
	{
		return Vertices::VerticesSendPayTxResponse(TxID);
	}

	Vertices::VerticesSendAcfgTxResponse
		buildSendAcfgTxResponse(const FString& TxID, const uint64& AssetID)
	{
		return Vertices::VerticesSendAcfgTxResponse(TxID, AssetID);
	}

	Vertices::VerticesSendAxferTxResponse
		buildSendAxferTxResponse(const FString& TxID)
	{
		return Vertices::VerticesSendAxferTxResponse(TxID);
	}

	Vertices::VerticesSendApplCallTxResponse
		buildSendApplCallTxResponse(const FString& TxID, const FString& Logs)
	{
		return Vertices::VerticesSendApplCallTxResponse(TxID, Logs);
	}
}