// Copyright 2022, Wisdom Labs. All Rights Reserved

#include "UnrealApi.h"
#include "ArcResponseBuilders.h"
#include "VerticesApiOperations.h"
#include "Arc/Arc03.h"
#include "Arc/Arc19.h"
#include "Arc/Arc69.h"
#include "Misc/MessageDialog.h"

#define LOCTEXT_NAMESPACE "FVerticesModule"

namespace {
    using Vertices = algorand::vertices::VerticesSDK;
}

namespace algorand{
namespace api {

// create vertices with its instance from Algorand module
UnrealApi::UnrealApi(TSharedPtr<algorand::vertices::VerticesSDK>& vertices)
{
    vertices_ = vertices;
}

UnrealApi::~UnrealApi() {}

void UnrealApi::setAlgodRpcInfo(const FString& algodRpc, const uint64_t& algodPort, const FString& algodTokenHeader_) {
    myAlgodRpc = algodRpc;
    myAlgodPort = algodPort;
    myAlgodTokenHeader = algodTokenHeader_;
}

void UnrealApi::setIndexerRpcInfo(const FString& indexerRpc, const uint64_t& indexerPort, const FString& indexerTokenHeader) {
    myIndexerRpc = indexerRpc;
    myIndexerPort = indexerPort;
    myIndexerTokenHeader = indexerTokenHeader;
}

void UnrealApi::AlgorandAPIInitWallet(const Vertices::VerticesInitWalletRequest& Request, const FAlgorandAPIInitWalletDelegate& Delegate) const
{
    TSharedRef<Vertices::FVerticesInitWalletDelegate> delegatePtr(MakeShared<Vertices::FVerticesInitWalletDelegate>());
    
    delegatePtr->BindLambda([this, Delegate](const Vertices::VerticesInitWalletResponse& response) {
        OnAlgorandAPIInitWalletResponse(response, Delegate);
    });
    vertices_->VerticesInitWallet( Request, delegatePtr.Get());
}

    /**
     * @brief callback after initialize new wallet
     */
    void UnrealApi::OnAlgorandAPIInitWalletResponse(const Vertices::VerticesInitWalletResponse& response, const FAlgorandAPIInitWalletDelegate& Delegate) const
{
    Delegate.ExecuteIfBound(response);
}
    
void UnrealApi::AlgorandAPILoadWallet(const Vertices::VerticesLoadWalletRequest& Request, const FAlgorandAPILoadWalletDelegate& Delegate) const
{
    TSharedRef<Vertices::FVerticesLoadWalletDelegate> delegatePtr(MakeShared<Vertices::FVerticesLoadWalletDelegate>());
    
    delegatePtr->BindLambda([this, Delegate](const Vertices::VerticesLoadWalletResponse& response) {
        OnAlgorandAPILoadWalletResponse(response, Delegate);
    });
    vertices_->VerticesLoadWallet( Request, delegatePtr.Get());
}
 
/**
 * @brief callback after restore wallet
 */
void UnrealApi::OnAlgorandAPILoadWalletResponse(const Vertices::VerticesLoadWalletResponse& response, const FAlgorandAPILoadWalletDelegate& Delegate) const
{
    Delegate.ExecuteIfBound(response);
}

void UnrealApi::AlgorandAPISaveWallet(const Vertices::VerticesSaveWalletRequest& Request, const FAlgorandAPISaveWalletDelegate& Delegate) const
{
    TSharedRef<Vertices::FVerticesSaveWalletDelegate> delegatePtr(MakeShared<Vertices::FVerticesSaveWalletDelegate>());
    
    delegatePtr->BindLambda([this, Delegate](const Vertices::VerticesSaveWalletResponse& response) {
        OnAlgorandAPISaveWalletResponse(response, Delegate);
    });
    vertices_->VerticesSaveWallet( Request, delegatePtr.Get());
}
 
    /**
     * @brief callback after restore wallet
     */
    void UnrealApi::OnAlgorandAPISaveWalletResponse(const Vertices::VerticesSaveWalletResponse& response, const FAlgorandAPISaveWalletDelegate& Delegate) const
{
    Delegate.ExecuteIfBound(response);
}
    
void UnrealApi::AlgorandAPIGetMnemonicsByAccountName(const Vertices::VerticesGetMnemonicsByAccountNameRequest& Request, const FAlgorandAPIGetMnemonicsByAccountNameDelegate& Delegate) const
{
    TSharedRef<Vertices::FVerticesGetMnemonicsByAccountNameDelegate> delegatePtr(MakeShared<Vertices::FVerticesGetMnemonicsByAccountNameDelegate>());
    
    delegatePtr->BindLambda([this, Delegate](const Vertices::VerticesGetMnemonicsByAccountNameResponse& response) {
        OnAlgorandAPIGetMnemonicsByAccountNameResponse(response, Delegate);
    });
    vertices_->VerticesGetMnemonicsByAccountName( Request, delegatePtr.Get());
}

/**
 * @brief callback after get backup mnemonic phrase
 */
void UnrealApi::OnAlgorandAPIGetMnemonicsByAccountNameResponse(const Vertices::VerticesGetMnemonicsByAccountNameResponse& response, const FAlgorandAPIGetMnemonicsByAccountNameDelegate& Delegate) const
{
    Delegate.ExecuteIfBound(response);
}
    
void UnrealApi::AlgorandAPIGenerateAccountFromMnemonics(const Vertices::VerticesGenerateAccountFromMnemonicsRequest& Request, const FAlgorandAPIGenerateAccountFromMnemonicsDelegate& Delegate) const
{
    TSharedRef<Vertices::FVerticesGenerateAccountFromMnemonicsDelegate> delegatePtr(MakeShared<Vertices::FVerticesGenerateAccountFromMnemonicsDelegate>());
    
    delegatePtr->BindLambda([this, Delegate](const Vertices::VerticesGenerateAccountFromMnemonicsResponse& response) {
        OnAlgorandAPIGenerateAccountFromMnemonicsGetResponse(response, Delegate);
    });
    vertices_->VerticesGenerateAccountFromMnemonics( Request, delegatePtr.Get());
}

/**
 * @brief callback after generate mnemonics
 */
void UnrealApi::OnAlgorandAPIGenerateAccountFromMnemonicsGetResponse(const Vertices::VerticesGenerateAccountFromMnemonicsResponse& response, const FAlgorandAPIGenerateAccountFromMnemonicsDelegate& Delegate) const
{
    Delegate.ExecuteIfBound(response);
}

void UnrealApi::AlgorandAPIGenerateRandomAccount(const Vertices::VerticesGenerateRandomAccountRequest& Request, const FAlgorandAPIGenerateRandomAccountDelegate& Delegate) const
{
    TSharedRef<Vertices::FVerticesGenerateRandomAccountDelegate> delegatePtr(MakeShared<Vertices::FVerticesGenerateRandomAccountDelegate>());
    
    delegatePtr->BindLambda([this, Delegate](const Vertices::VerticesGenerateRandomAccountResponse& response) {
        OnAlgorandAPIGenerateRandomAccountGetResponse(response, Delegate);
    });
    vertices_->VerticesGenerateRandomAccount( Request, delegatePtr.Get());
}

/**
 * @brief callback after generate new account
 */
void UnrealApi::OnAlgorandAPIGenerateRandomAccountGetResponse(const Vertices::VerticesGenerateRandomAccountResponse& response, const FAlgorandAPIGenerateRandomAccountDelegate& Delegate) const
{
    Delegate.ExecuteIfBound(response);
}

void UnrealApi::AlgorandAPIGetAddrBalance(const Vertices::VerticesGetAddrBalanceRequest& Request, const FAlgorandAPIGetAddrBalanceDelegate& Delegate) const
{
    TSharedRef<Vertices::FVerticesGetAddrBalanceDelegate> delegatePtr(MakeShared<Vertices::FVerticesGetAddrBalanceDelegate>());
    
    delegatePtr->BindLambda([this, Delegate](const Vertices::VerticesGetAddrBalanceResponse& response) {
        OnAlgorandAPIGetAddrBalanceResponse(response, Delegate);
    });
    vertices_->VerticesGetAddrBalance( Request, delegatePtr.Get());
    
}

/**
 * @brief callback after get balance
 */
void UnrealApi::OnAlgorandAPIGetAddrBalanceResponse(const Vertices::VerticesGetAddrBalanceResponse& response, const FAlgorandAPIGetAddrBalanceDelegate& Delegate) const
{
    Delegate.ExecuteIfBound(response);
}

void UnrealApi::AlgorandAPISendPayTx(const Vertices::VerticesSendPayTxRequest& Request, const FAlgorandAPISendPayTxDelegate& Delegate) const
{
    TSharedRef<Vertices::FVerticesSendPayTxDelegate> delegatePtr(MakeShared<Vertices::FVerticesSendPayTxDelegate>());

    delegatePtr->BindLambda([this, Delegate](const Vertices::VerticesSendPayTxResponse& response) {
        OnAlgorandAPISendPayTxResponse(response, Delegate);
        });
    vertices_->VerticesSendPayTx(Request, delegatePtr.Get());
}

/**
 * @brief callback after payment tx
 */
void UnrealApi::OnAlgorandAPISendPayTxResponse(const Vertices::VerticesSendPayTxResponse& response, const FAlgorandAPISendPayTxDelegate& Delegate) const
{
    Delegate.ExecuteIfBound(response);
}

void UnrealApi::AlgorandAPISendAcfgTx(const Vertices::VerticesSendAcfgTxRequest& Request, const FAlgorandAPISendAcfgTxDelegate& Delegate) const
{
    TSharedRef<Vertices::FVerticesSendAcfgTxDelegate> delegatePtr(MakeShared<Vertices::FVerticesSendAcfgTxDelegate>());

    delegatePtr->BindLambda([this, Delegate](const Vertices::VerticesSendAcfgTxResponse& response) {
        OnAlgorandAPISendAcfgTxResponse(response, Delegate);
        });
    vertices_->VerticesSendAcfgTx(Request, delegatePtr.Get());
}

/**
 * @brief callback after asset config tx
 */
void UnrealApi::OnAlgorandAPISendAcfgTxResponse(const Vertices::VerticesSendAcfgTxResponse& response, const FAlgorandAPISendAcfgTxDelegate& Delegate) const
{
    Delegate.ExecuteIfBound(response);
}

void UnrealApi::AlgorandAPISendAxferTx(const Vertices::VerticesSendAxferTxRequest& Request, const FAlgorandAPISendAxferTxDelegate& Delegate) const
{
    TSharedRef<Vertices::FVerticesSendAxferTxDelegate> delegatePtr(MakeShared<Vertices::FVerticesSendAxferTxDelegate>());

    delegatePtr->BindLambda([this, Delegate](const Vertices::VerticesSendAxferTxResponse& response) {
        OnAlgorandAPISendAxferTxResponse(response, Delegate);
        });
    vertices_->VerticesSendAxferTx(Request, delegatePtr.Get());
}

/**
 * @brief callback after asset transfer tx
 */
void UnrealApi::OnAlgorandAPISendAxferTxResponse(const Vertices::VerticesSendAxferTxResponse& response, const FAlgorandAPISendAxferTxDelegate& Delegate) const
{
    Delegate.ExecuteIfBound(response);
}
    
void UnrealApi::AlgorandAPISendApplCallTx(const Vertices::VerticesSendApplCallTxRequest& Request, const FAlgorandAPISendApplCallTxDelegate& Delegate) const
{
    TSharedRef<Vertices::FVerticesSendApplCallTxDelegate> delegatePtr(MakeShared<Vertices::FVerticesSendApplCallTxDelegate>());

    delegatePtr->BindLambda([this, Delegate](const Vertices::VerticesSendApplCallTxResponse& response) {
        OnAlgorandAPISendApplCallTxResponse(response, Delegate);
    });

    vertices_->VerticesSendApplCallTx(Request, delegatePtr.Get());
}

/**
 * @brief callback after application call tx
 */
void UnrealApi::OnAlgorandAPISendApplCallTxResponse(const Vertices::VerticesSendApplCallTxResponse& response, const FAlgorandAPISendApplCallTxDelegate& Delegate) const
{
    Delegate.ExecuteIfBound(response);
}

void UnrealApi::AlgorandAPIArcAssetDetails(const Vertices::VerticesArcAssetDetailsRequest& Request, const FAlgorandAPIArcAssetDetailsDelegate& Delegate) const
{
    AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, [this, Request, Delegate]()
    {   
        TSharedRef<ArcResponseBuilders::FAPIArcAssetDetailsGetDelegate> delegatePtr(MakeShared<ArcResponseBuilders::FAPIArcAssetDetailsGetDelegate>());

        ArcBase temp_arc(myIndexerRpc, myIndexerPort, myIndexerTokenHeader);
        temp_arc.from_asset(Request.asset_ID.GetValue());
        
        Arc03 arc03_data(temp_arc.asset, myIndexerRpc, myIndexerPort, myIndexerTokenHeader);
        if(arc03_data.IsVerify())
        {
            auto param_url = StringCast<ANSICHAR>(*(arc03_data.asset.params.url));
            std::string s_url = param_url.Get();
            arc03_data.from_asset_url(s_url);
            
            delegatePtr->BindLambda([this, Delegate](const Vertices::VerticesArcAssetDetailsResponse& response) {
                OnAlgorandAPIArcAssetDetailsResponse(response, Delegate);
            });

            ArcResponseBuilders::buildArcAssetDetailsResponse(arc03_data, delegatePtr.Get());
            return;
        }
        
        Arc19 arc19_data(temp_arc.asset, myIndexerRpc, myIndexerPort, myIndexerTokenHeader);
        if(arc19_data.IsVerify())
        {
            arc19_data.ParseASAUrl();
            arc19_data.from_temp_ipfs();

            delegatePtr->BindLambda([this, Delegate](const Vertices::VerticesArcAssetDetailsResponse& response) {
                OnAlgorandAPIArcAssetDetailsResponse(response, Delegate);
            });

            ArcResponseBuilders::buildArcAssetDetailsResponse(arc19_data, delegatePtr.Get());
            return;
        }
        
        Arc69 arc69_data(temp_arc.asset, myIndexerRpc, myIndexerPort, myIndexerTokenHeader);
        if(arc69_data.IsVerify())
        {
            auto tx_note = StringCast<ANSICHAR>(*(arc69_data.tx.note));
            std::string s_tx_note = tx_note.Get();
            arc69_data.from_tx_note(s_tx_note);

            delegatePtr->BindLambda([this, Delegate](const Vertices::VerticesArcAssetDetailsResponse& response) {
                OnAlgorandAPIArcAssetDetailsResponse(response, Delegate);
            });

            ArcResponseBuilders::buildArcAssetDetailsResponse(arc69_data, delegatePtr.Get());
            return;
        }

        Vertices::VerticesArcAssetDetailsResponse response;
        response.SetSuccessful(false);
        response.SetResponseString("This arc asset doesn't have correct type.");
        OnAlgorandAPIArcAssetDetailsResponse(response, Delegate);
        AsyncTask(ENamedThreads::GameThread, [Delegate, response]()
        {
            Delegate.ExecuteIfBound(response);
        });
    });
}

/**
 * @brief callback after arc asset details
 */
void UnrealApi::OnAlgorandAPIArcAssetDetailsResponse(const Vertices::VerticesArcAssetDetailsResponse& response, const FAlgorandAPIArcAssetDetailsDelegate& Delegate) const
{
    Delegate.ExecuteIfBound(response);
}

void UnrealApi::AlgorandAPIAccountInformation(const Vertices::VerticesAccountInformationRequest& Request, const FAlgorandAPIAccountInformationDelegate& Delegate) const
{
    AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, [this, Request, Delegate]()
    {   
        TSharedRef<ArcResponseBuilders::FAPIAccountInfoGetDelegate> delegatePtr(MakeShared<ArcResponseBuilders::FAPIAccountInfoGetDelegate>());

        if ( Request.address.GetValue().Len() == 58 )
        {
            AccountAsset accountInfo(myAlgodRpc, myAlgodPort, myAlgodTokenHeader);
            accountInfo.getInformation(Request.address.GetValue());
            
            delegatePtr->BindLambda([this, Delegate](const Vertices::VerticesAccountInformationResponse& response) {
                OnAlgorandAPIAccountInformationGetResponse(response, Delegate);
            });

            ArcResponseBuilders::buildAccountInformationResponse(accountInfo, delegatePtr.Get());   
        }
        else
        {
            Vertices::VerticesAccountInformationResponse response;
            response.SetSuccessful(false);
            response.SetResponseString("Address Length is invalid.");
            
            AsyncTask(ENamedThreads::GameThread, [Delegate, response]()
		    {
			    Delegate.ExecuteIfBound(response);
		    });
        }
    });
}

/**
 * @brief callback after arc asset details
 */
void UnrealApi::OnAlgorandAPIAccountInformationGetResponse(const Vertices::VerticesAccountInformationResponse& response, const FAlgorandAPIAccountInformationDelegate& Delegate) const
{
    Delegate.ExecuteIfBound(response);
}

}
}

#undef LOCTEXT_NAMESPACE