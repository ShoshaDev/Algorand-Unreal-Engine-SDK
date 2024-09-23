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

	algorand::vertices::VerticesSDK::VerticesWalletExistenceRequest
	buildWalletExistenceRequest()
	{
		Vertices::VerticesWalletExistenceRequest request;
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
	buildGetMnemonicsByAccountNameRequest(const FString& Name)
	{
		return Vertices::VerticesGetMnemonicsByAccountNameRequest(Name);
	}

	Vertices::VerticesGetAllAccountsRequest
	buildGetAllAccountsRequest()
	{
		return Vertices::VerticesGetAllAccountsRequest();
	}

	Vertices::VerticesGenerateAccountFromMnemonicsRequest
	buildGenerateAccountFromMnemonicsRequest(const FString& Mnemonics, const FString& Name)
	{
		return Vertices::VerticesGenerateAccountFromMnemonicsRequest(Mnemonics, Name);
	}
	
	Vertices::VerticesGenerateRandomAccountRequest
	buildGenerateRandomAccountRequest()
	{
		return Vertices::VerticesGenerateRandomAccountRequest();
	}

	Vertices::VerticesGetAddrBalanceRequest
	buildGetBalanceRequest(const FString& address)
	{
		Vertices::VerticesGetAddrBalanceRequest request;
		request.Address = address;
		return request;
	}
	
	Vertices::VerticesSendPayTxRequest
		buildPaymentTransactionRequest(const FString& mainAccountName, 
									   const FString& receiver,
									   const FUInt64& amount,
									   const FString& notes)
	{
		Vertices::VerticesSendPayTxRequest request;
		request.MainAccountName = mainAccountName;
		request.ReceiverAddress = receiver;
		request.Amount = amount;
		request.Notes = notes;
		return request;
	}

	Vertices::VerticesSendAcfgTxRequest
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
										   const FString& notes)
	{
		Vertices::VerticesSendAcfgTxRequest request;
		request.MainAccountName = mainAccountName;
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
		buildAssetTransferTransactionRequest(const FString& mainAccountName,
		                                     const FString& receiver,
		                                     const FUInt64& asset_id,
		                                     const FUInt64& amount,
		                                     const FString& notes)
	{
		Vertices::VerticesSendAxferTxRequest request;
		request.MainAccountName = mainAccountName;
		request.ReceiverAddress = receiver;
		request.Asset_ID = asset_id;
		request.Amount = amount; 
		request.Notes = notes;
		return request;
	}

	Vertices::VerticesSendApplCallTxRequest
		buildApplicationCallTransactionRequest(const FString& mainAccountName,
		                                       const FUInt64& app_ID,
		                                       const TArray<FAppArg>& app_args,
		                                       const EAppOnCompleteTX& app_complete_tx)
	{
		Vertices::VerticesSendApplCallTxRequest request;
		uint8_t index;
		request.MainAccountName = mainAccountName;
		request.App_ID = app_ID;
		request.App_Args.Empty();
		for(index = 0; index < app_args.Num(); index++)
			request.App_Args.Add(app_args[index].Value_Bytes);
		
		request.App_Complete_TX = NOOP;			// set initial value
		switch (app_complete_tx)
		{
		case EAppOnCompleteTX::NOOP: request.App_Complete_TX = NOOP;
			break;
		case EAppOnCompleteTX::OPT_IN: request.App_Complete_TX = OPT_IN;
			break;
		case EAppOnCompleteTX::CLOSE_OUT: request.App_Complete_TX = CLOSE_OUT;
			break;
		case EAppOnCompleteTX::CLEAR_STATE: request.App_Complete_TX = CLEAR_STATE;
			break;
		case EAppOnCompleteTX::UPDATE_APP: request.App_Complete_TX = UPDATE_APP;
			break;
		case EAppOnCompleteTX::DELETE_APP: request.App_Complete_TX = DELETE_APP;
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

	Vertices::VerticesRemoveAccountByNameRequest
	buildRemoveAccountByNameRequest(const FString& Name)
	{
		return Vertices::VerticesRemoveAccountByNameRequest(Name);
	}
}