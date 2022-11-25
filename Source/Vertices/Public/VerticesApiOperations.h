#pragma once

#include "CoreMinimal.h"
#include "BaseModel.h"

namespace algorand {
    namespace vertices {

        class VerticesSDK::VerticesGenerateWalletGetRequest : public Request
        {
        public:
            virtual ~VerticesGenerateWalletGetRequest() {}

        };

        class VerticesSDK::VerticesGenerateWalletGetResponse : public Response
        {
        public:
            ~VerticesGenerateWalletGetResponse() {}
            FString Address;
        };

        class VerticesSDK::VerticesGetaddressbalanceGetRequest : public Request
        {
        public:
            virtual ~VerticesGetaddressbalanceGetRequest() {}

            TOptional<FString> Address;
        };

        class VerticesSDK::VerticesGetaddressbalanceGetResponse : public Response
        {
        public:
            ~VerticesGetaddressbalanceGetResponse() {}
            uint64 Amount;
        };

        class VerticesSDK::VerticesPaymentTransactionGetRequest : public Request
        {
        public:
            virtual ~VerticesPaymentTransactionGetRequest() {}

            TOptional<FString> senderAddress;
            TOptional<FString> receiverAddress;
            TOptional<uint64_t> amount;
            TOptional<FString> notes;
        };

        class VerticesSDK::VerticesPaymentTransactionGetResponse : public Response
        {
        public:
            ~VerticesPaymentTransactionGetResponse() {}
            FString txID;
        };

        class VerticesSDK::VerticesApplicationCallTransactionGetRequest : public Request
        {
        public:
            virtual ~VerticesApplicationCallTransactionGetRequest() {}

            TOptional<FString> senderAddress;
            TOptional<uint64_t> app_ID;
        };

        class VerticesSDK::VerticesApplicationCallTransactionGetResponse : public Response
        {
        public:
            ~VerticesApplicationCallTransactionGetResponse() {}
            FString txID;
        };
    }
}