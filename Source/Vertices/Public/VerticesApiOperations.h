// Copyright 2022, Wisdom Labs. All Rights Reserved

#pragma once
#include "CoreMinimal.h"
#include "BaseModel.h"
#include "VerticesSDK.h"

namespace {
    using VerticesSDK = algorand::vertices::VerticesSDK;
}

namespace algorand {
    namespace vertices {
        // for Initializing algorand wallet
        class VerticesSDK::VerticesInitWalletRequest : public Request
        {
        public:
            virtual ~VerticesInitWalletRequest() {}
        };
        
        class VerticesSDK::VerticesInitWalletResponse : public Response
        {
        public:
            ~VerticesInitWalletResponse() {}
        };

        // for checking if algorand wallet exists
        class VerticesSDK::VerticesWalletExistenceRequest : public Request
        {
        public:
            VerticesWalletExistenceRequest() {}
            virtual ~VerticesWalletExistenceRequest() {}
        };
        
        class VerticesSDK::VerticesWalletExistenceResponse : public Response
        {
        public:
            VerticesWalletExistenceResponse() : Exists(false) {}
            VerticesWalletExistenceResponse(bool Exists) : Exists(Exists){}
            ~VerticesWalletExistenceResponse() {}
            bool Exists;
        };
        
        // for Loading algorand wallet
        class VerticesSDK::VerticesLoadWalletRequest : public Request
        {
        public:
            VerticesLoadWalletRequest(const FString& Password) : Password(Password) {}
            virtual ~VerticesLoadWalletRequest() {}
            TOptional<FString> Password;
        };
        
        class VerticesSDK::VerticesLoadWalletResponse : public Response
        {
        public:
            ~VerticesLoadWalletResponse() {}
        };

        // for Saving algorand wallet
        class VerticesSDK::VerticesSaveWalletRequest : public Request
        {
        public:
            VerticesSaveWalletRequest(const FString& Password) : Password(Password) {}
            virtual ~VerticesSaveWalletRequest() {}
            TOptional<FString> Password;
        };
        
        class VerticesSDK::VerticesSaveWalletResponse : public Response
        {
        public:
            ~VerticesSaveWalletResponse() {}
        };

        // for Generating Account From Mnemonics
        class VerticesSDK::VerticesGenerateAccountFromMnemonicsRequest : public Request
        {
        public:
            VerticesGenerateAccountFromMnemonicsRequest(const FString& Mnemonics, const FString& Name) : Mnemonics(Mnemonics), Name(Name) {}
            virtual ~VerticesGenerateAccountFromMnemonicsRequest() {}
            FString Mnemonics;
            FString Name;
        };
        
        class VerticesSDK::VerticesGenerateAccountFromMnemonicsResponse : public Response
        {
        public:
            VerticesGenerateAccountFromMnemonicsResponse() {}
            VerticesGenerateAccountFromMnemonicsResponse(const FString& Address, const FString& Name) : Address(Address), Name(Name) {}
            virtual ~VerticesGenerateAccountFromMnemonicsResponse() {}
            FString Address;
            FString Name;
        };

        // for Generating Random Account
        class VerticesSDK::VerticesGenerateRandomAccountRequest : public Request
        {
        public:
            virtual ~VerticesGenerateRandomAccountRequest() {}
        };
        
        class VerticesSDK::VerticesGenerateRandomAccountResponse : public Response
        {
        public:
            VerticesGenerateRandomAccountResponse() {}
            VerticesGenerateRandomAccountResponse(const FString& Address, const FString& Name, const FString& Mnemonics) : Address(Address)
            {
                // Convert Mnemonics to Mnemonic Phrase as array
                Mnemonics.ParseIntoArray(MnemonicPhrase, TEXT(" "), true);
            }
            virtual ~VerticesGenerateRandomAccountResponse() {}
            FString Address;
            TArray<FString> MnemonicPhrase;
            
        };

        // for Getting Mnemonics by account name
        class VerticesSDK::VerticesGetMnemonicsByAccountNameRequest : public Request
        {
        public:
            VerticesGetMnemonicsByAccountNameRequest(const FString& Name) : Name(Name) {}
            virtual ~VerticesGetMnemonicsByAccountNameRequest() {}
            TOptional<FString> Name;
        };
        
        class VerticesSDK::VerticesGetMnemonicsByAccountNameResponse : public Response
        {
        public:
            VerticesGetMnemonicsByAccountNameResponse() {}
            VerticesGetMnemonicsByAccountNameResponse(const FString& Mnemonics, const FString& Name) : Mnemonics(Mnemonics), Name(Name) {}
            ~VerticesGetMnemonicsByAccountNameResponse() {}
            FString Mnemonics;
            FString Name;
        };

        // for Getting ALL Accounts from a memory wallet 
        class VerticesSDK::VerticesGetAllAccountsRequest : public Request
        {
        public:
            virtual ~VerticesGetAllAccountsRequest() {}
        };
        
        class VerticesSDK::VerticesGetAllAccountsResponse : public Response
        {
        public:
            VerticesGetAllAccountsResponse() {}
            VerticesGetAllAccountsResponse(const TArray<FString>& Names, const TArray<FString> Addresses) : Names(Names), Addresses(Addresses) {}
            ~VerticesGetAllAccountsResponse() {}
            TArray<FString> Names;
            TArray<FString> Addresses;
        };

        // for Getting balance by any address
        class VerticesSDK::VerticesGetAddrBalanceRequest : public Request
        {
        public:
            virtual ~VerticesGetAddrBalanceRequest() {}

            TOptional<FString> Address;
        };
        
        class VerticesSDK::VerticesGetAddrBalanceResponse : public Response
        {
        public:
            VerticesGetAddrBalanceResponse(): Amount(0) {}
            VerticesGetAddrBalanceResponse(const FString& Address, const uint64& Amount) : Address(Address), Amount(Amount) {}
            ~VerticesGetAddrBalanceResponse() {}
            FString Address;
            uint64 Amount;
        };

        // for sending Payment TX
        class VerticesSDK::VerticesSendPayTxRequest : public Request
        {
        public:
            virtual ~VerticesSendPayTxRequest() {}

            TOptional<FString> MainAccountName;
            TOptional<FString> ReceiverAddress;
            TOptional<uint64_t> Amount;
            TOptional<FString> Notes;
        };
        
        class VerticesSDK::VerticesSendPayTxResponse : public Response
        {
        public:
            VerticesSendPayTxResponse() {}
            VerticesSendPayTxResponse(const FString& TxID) : TxID(TxID) {}
            virtual ~VerticesSendPayTxResponse() override {}
            FString TxID;
        };

        // for sending Asset Config TX
        class VerticesSDK::VerticesSendAcfgTxRequest : public Request
        {
        public:
            virtual ~VerticesSendAcfgTxRequest() {}

            TOptional<FString> MainAccountName;
            TOptional<FString> Manager;
            TOptional<FString> Reserve;
            TOptional<FString> Freeze;
            TOptional<FString> Clawback;
            TOptional<uint64_t> AssetId;
            TOptional<uint64_t> Total;
            TOptional<uint64_t> Decimals;
            TOptional<bool> IsFrozen;
            TOptional<FString> UnitName;
            TOptional<FString> AssetName;
            TOptional<FString> Url;
            TOptional<FString> Notes;
        };

        // for sending Asset Config TX
        class VerticesSDK::VerticesSendAcfgTxResponse : public Response
        {
        public:
            VerticesSendAcfgTxResponse(): AssetID(0) {}
            VerticesSendAcfgTxResponse(const FString& TxID, const uint64& AssetID) : TxID(TxID), AssetID(AssetID) {}
            ~VerticesSendAcfgTxResponse() {}
            FString TxID;
            uint64 AssetID;
        };

        // for sending Asset Transfer TX
        class VerticesSDK::VerticesSendAxferTxRequest : public Request
        {
        public:
            virtual ~VerticesSendAxferTxRequest() {}

            TOptional<FString> MainAccountName;
            TOptional<FString> ReceiverAddress;
            TOptional<uint64_t> Asset_ID;
            TOptional<uint64_t> Amount;
            TOptional<FString> Notes;
        };
        
        class VerticesSDK::VerticesSendAxferTxResponse : public Response
        {
        public:
            VerticesSendAxferTxResponse() {}
            VerticesSendAxferTxResponse(const FString& TxID) : TxID(TxID) {}
            ~VerticesSendAxferTxResponse() {}
            FString TxID;
        };

        // for sending Application Call TX
        class VerticesSDK::VerticesSendApplCallTxRequest : public Request
        {
        public:
            virtual ~VerticesSendApplCallTxRequest() {}

            TOptional<FString> MainAccountName;
            TOptional<uint64_t> App_ID;
            TArray<TArray<uint8_t>> App_Args;
            EAppCompleteTX App_Complete_TX; 
        };
        
        class VerticesSDK::VerticesSendApplCallTxResponse : public Response
        {
        public:
            VerticesSendApplCallTxResponse() {}
            VerticesSendApplCallTxResponse(const FString& TxID, const FString& Logs) : TxID(TxID), Logs(Logs) {}
            ~VerticesSendApplCallTxResponse() {}
            FString TxID;
            FString Logs;
        };

        // for Arc Asset Details
        class VerticesSDK::VerticesArcAssetDetailsRequest : public Request
        {
        public:
            virtual ~VerticesArcAssetDetailsRequest() {}
            
            TOptional<uint64_t> asset_ID;
        };

        // Response for Arc Asset Details
        class VerticesSDK::VerticesArcAssetDetailsResponse : public Response
        {
        public:
            ~VerticesArcAssetDetailsResponse() {}
            
            uint8 Standard;
            FString Unit_Name;
            uint64_t Total;
            uint64_t Decimals;
            FString Description;
            FString Clawback;
            FString Creator;
            FString Freeze;
            FString Manager;
            FString Reserve;
            FString Media_Url;
            FString External_Url;
            FString Animation_Url;
            TMap<FString, FString> Properties;
        };

        // Request for Account Information
        class VerticesSDK::VerticesAccountInformationRequest : public Request
        {
        public:
            virtual ~VerticesAccountInformationRequest() {}
            
            TOptional<FString> address;
        };

        // Response for Account Information
        class VerticesSDK::VerticesAccountInformationResponse : public Response
        {
        public:
            ~VerticesAccountInformationResponse() {}
            
            TArray<FString> AssetIDs;
            TArray<FString> AssetNames;
        };

        // for Removing an account by its name
        class VerticesSDK::VerticesRemoveAccountByNameRequest : public Request
        {
        public:
            VerticesRemoveAccountByNameRequest(const FString& Name) : Name(Name) {}
            virtual ~VerticesRemoveAccountByNameRequest() {}
            TOptional<FString> Name;
        };
        
        class VerticesSDK::VerticesRemoveAccountByNameResponse : public Response
        {
        public:
            ~VerticesRemoveAccountByNameResponse() {}
        };
    }
}
