// Copyright 2022, Wisdom Labs. All Rights Reserved

#include "RequestBuilders.h"
#include "BaseModel.h"
#include "VerticesApiOperations.h"

namespace {
	using Vertices = algorand::vertices::VerticesSDK;
}
namespace request_builders {
	Vertices::VerticesInitWalletRequest
	buildInitWalletRequest()
	{
		Vertices::VerticesInitWalletRequest request;
		return request;
	}
	
	Vertices::VerticesLoadWalletRequest
	buildLoadWalletRequest(const FString& Password)
	{
		return Vertices::VerticesLoadWalletRequest(Password);
	}

	Vertices::VerticesSaveWalletRequest
	buildSaveWalletRequest(const FString& Password)
	{
		return Vertices::VerticesSaveWalletRequest(Password);
	}

	Vertices::VerticesGetMnemonicsByAccountNameRequest
	buildGetMnemonicsByAccountNameRequest()
	{
		Vertices::VerticesGetMnemonicsByAccountNameRequest request;
		return request;
	}

	Vertices::VerticesGenerateAccountFromMnemonicsRequest
	buildGenerateAccountFromMnemonicsRequest()
	{
		Vertices::VerticesGenerateAccountFromMnemonicsRequest request;
		return request;
	}

	Vertices::VerticesGetAddrBalanceRequest
	buildGetBalanceRequest(const FString& address)
	{
		Vertices::VerticesGetAddrBalanceRequest request;
		request.Address = address;
		return request;
	}
	
	Vertices::VerticesSendPayTxRequest
		buildPaymentTransactionRequest(const FString& sender, 
									   const FString& receiver,
									   const FUInt64& amount,
									   const FString& notes)
	{
		Vertices::VerticesSendPayTxRequest request;
		request.senderAddress = sender;
		request.receiverAddress = receiver;
		request.amount = amount;
		request.notes = notes;
		return request;
	}

	Vertices::VerticesSendAcfgTxRequest
		buildAssetConfigTransactionRequest(const FString& creator, 
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
									   const FString& notes)
	{
		Vertices::VerticesSendAcfgTxRequest request;
		request.Creator = creator;
		request.Manager = manager;
		request.Reserve = reserve;
		request.Freeze = freeze;
		request.Clawback = clawback;
		request.AssetId = asset_id;
		request.Total = total;
		request.Decimals = decimals;
		if(isFrozen.ToLower().Contains("true"))
			request.IsFrozen = 1;
		else
			request.IsFrozen = 0;
		request.UnitName = unit_name;
		request.AssetName = asset_name;
		request.Url = url;
		request.Notes = notes;
		return request;
	}

	Vertices::VerticesSendAxferTxRequest
		buildAssetTransferTransactionRequest(const FString& sender, 
									   const FString& receiver,
									   const FUInt64& asset_id,
									   const FString& amount,
									   const FString& notes)
	{
		Vertices::VerticesSendAxferTxRequest request;
		request.senderAddress = sender;
		request.receiverAddress = receiver;
		request.asset_id = asset_id;
		request.amount = atof(TCHAR_TO_ANSI(*amount)); 
		request.notes = notes;
		return request;
	}

	Vertices::VerticesSendApplCallTxRequest
		buildApplicationCallTransactionRequest(const FString& sender, 
									   const FUInt64& app_ID,
									   const TArray<FAppArg>& app_args,
									   const EAppOnCompleteTX& app_complete_tx)
	{
		Vertices::VerticesSendApplCallTxRequest request;
		uint8_t index;
		request.senderAddress = sender;
		request.app_ID = app_ID;
		request.app_args.Empty();
		for(index = 0; index < app_args.Num(); index++)
			request.app_args.Add(app_args[index].Value_Bytes);
		
		request.app_complete_tx = NOOP;			// set initial value
		switch (app_complete_tx)
		{
		case EAppOnCompleteTX::NOOP: request.app_complete_tx = NOOP;
			break;
		case EAppOnCompleteTX::OPT_IN: request.app_complete_tx = OPT_IN;
			break;
		case EAppOnCompleteTX::CLOSE_OUT: request.app_complete_tx = CLOSE_OUT;
			break;
		case EAppOnCompleteTX::CLEAR_STATE: request.app_complete_tx = CLEAR_STATE;
			break;
		case EAppOnCompleteTX::UPDATE_APP: request.app_complete_tx = UPDATE_APP;
			break;
		case EAppOnCompleteTX::DELETE_APP: request.app_complete_tx = DELETE_APP;
			break;
		}
		return request;
	}

	Vertices::VerticesArcAssetDetailsRequest
		buildArcAssetDetailsRequest(const FUInt64& asset_ID)
	{
		Vertices::VerticesArcAssetDetailsRequest request;
		request.asset_ID = asset_ID;
		return request;
	}

	Vertices::VerticesAccountInformationRequest
		buildAccountInformationRequest(const FString& address)
	{
		Vertices::VerticesAccountInformationRequest request;
		request.address = address;
		return request;
	}
}