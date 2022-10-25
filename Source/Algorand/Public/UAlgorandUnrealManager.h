#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"

#include "RequestContextManager.h"
#include "Models/FUInt64.h"
#include "Models/FError.h"
#include "TResult.h"
#include "UnrealApi.h"
#include "VerticesSDK.h"

#include "UAlgorandUnrealManager.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FGetBalanceResponseReceivedDelegate, const FUInt64&, money);
DECLARE_DYNAMIC_DELEGATE_OneParam(FErrorReceivedDelegate, const FError&, error);

class TransactionBuilder;

UCLASS(BlueprintType, Blueprintable)
class ALGORAND_API UAlgorandUnrealManager : public UObject
{
    GENERATED_BODY()

public:
    UAlgorandUnrealManager();

    UFUNCTION(BlueprintCallable, Category = "StratisUnrealManager")
    void getBalance(const FGetBalanceResponseReceivedDelegate& delegate,
                    const FErrorReceivedDelegate& errorDelegate);
    void getBalance(TFunction<void(const TResult<int64>&)> callback);

    UWorld* GetWorld() const override;

private:
    TSharedPtr<TransactionBuilder> transactionBuilder_;
    TSharedPtr<algorand::api::UnrealApi> unrealApi_;
    TSharedPtr<algorand::vertices::VerticesSDK> vertices_;
    
    RequestContextManager requestContextManager_;
};