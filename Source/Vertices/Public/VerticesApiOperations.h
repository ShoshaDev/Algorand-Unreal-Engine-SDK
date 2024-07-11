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
            FString output;
        };
        
        // for Loading algorand wallet
        class VerticesSDK::VerticesLoadWalletRequest : public Request
        {
        public:
            virtual ~VerticesLoadWalletRequest() {}
            TOptional<FString> Mnemonics;
        };
        
        class VerticesSDK::VerticesLoadWalletResponse : public Response
        {
        public:
            ~VerticesLoadWalletResponse() {}
            FString output;
        };

        // for Saving algorand wallet
        class VerticesSDK::VerticesSaveWalletRequest : public Request
        {
        public:
            virtual ~VerticesSaveWalletRequest() {}
            TOptional<FString> Mnemonics;
        };
        
        class VerticesSDK::VerticesSaveWalletResponse : public Response
        {
        public:
            ~VerticesSaveWalletResponse() {}
            FString output;
        };

        // for Generating Account From Mnemonics
        class VerticesSDK::VerticesGenerateAccountFromMnemonicsRequest : public Request
        {
        public:
            virtual ~VerticesGenerateAccountFromMnemonicsRequest() {}
        };
        
        class VerticesSDK::VerticesGenerateAccountFromMnemonicsResponse : public Response
        {
        public:
            ~VerticesGenerateAccountFromMnemonicsResponse() {}
            FString output;
        };

        // for Getting Mnemonics by account name
        class VerticesSDK::VerticesGetMnemonicsByAccountNameRequest : public Request
        {
        public:
            virtual ~VerticesGetMnemonicsByAccountNameRequest() {}
        };
        
        class VerticesSDK::VerticesGetMnemonicsByAccountNameResponse : public Response
        {
        public:
            ~VerticesGetMnemonicsByAccountNameResponse() {}
            FString output;
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
            ~VerticesGetAddrBalanceResponse() {}
            uint64 Amount;
        };

        // for sending Payment TX
        class VerticesSDK::VerticesSendPayTxRequest : public Request
        {
        public:
            virtual ~VerticesSendPayTxRequest() {}

            TOptional<FString> senderAddress;
            TOptional<FString> receiverAddress;
            TOptional<uint64_t> amount;
            TOptional<FString> notes;
        };
        
        class VerticesSDK::VerticesSendPayTxResponse : public Response
        {
        public:
            ~VerticesSendPayTxResponse() {}
            FString txID;
        };

        // for sending Asset Config TX
        class VerticesSDK::VerticesSendAcfgTxRequest : public Request
        {
        public:
            virtual ~VerticesSendAcfgTxRequest() {}

            TOptional<FString>  Creator; 
            TOptional<FString>  Manager;
            TOptional<FString>  Reserve;
            TOptional<FString>  Freeze;
            TOptional<FString>  Clawback;
            TOptional<uint64_t>  AssetId;
            TOptional<uint64_t>  Total;
            TOptional<uint64_t>  Decimals;
            TOptional<uint8_t>  IsFrozen;
            TOptional<FString>  UnitName;
            TOptional<FString>  AssetName;
            TOptional<FString>  Url;
            TOptional<FString>  Notes;
        };

        // for sending Asset Config TX
        class VerticesSDK::VerticesSendAcfgTxResponse : public Response
        {
        public:
            ~VerticesSendAcfgTxResponse() {}
            FString txID;
            uint64 assetID;
        };

        // for sending Asset Transfer TX
        class VerticesSDK::VerticesSendAxferTxRequest : public Request
        {
        public:
            virtual ~VerticesSendAxferTxRequest() {}

            TOptional<FString> senderAddress;
            TOptional<FString> receiverAddress;
            TOptional<uint64_t> asset_id;
            TOptional<double> amount;
            TOptional<FString> notes;
        };
        
        class VerticesSDK::VerticesSendAxferTxResponse : public Response
        {
        public:
            ~VerticesSendAxferTxResponse() {}
            FString txID;
        };

        // for sending Application Call TX
        class VerticesSDK::VerticesSendApplCallTxRequest : public Request
        {
        public:
            virtual ~VerticesSendApplCallTxRequest() {}

            TOptional<FString> senderAddress;
            TOptional<uint64_t> app_ID;
            TArray<TArray<uint8_t>> app_args;
            EAppCompleteTX app_complete_tx; 
        };
        
        class VerticesSDK::VerticesSendApplCallTxResponse : public Response
        {
        public:
            ~VerticesSendApplCallTxResponse() {}
            FString txID;
            FString logs;
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
            
            uint8 standard;
            FString unit_name;
            uint64_t total;
            uint64_t decimals;
            FString description;
            FString clawback;
            FString creator;
            FString freeze;
            FString manager;
            FString reserve;
            FString media_url;
            FString external_url;
            FString animation_url;
            TMap<FString, FString> properties;
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
            
            TArray<FString> assetIDs;
            TArray<FString> assetNames;
        };
    }
}
