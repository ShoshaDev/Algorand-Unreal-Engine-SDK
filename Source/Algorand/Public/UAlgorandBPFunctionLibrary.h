// Copyright 2022, Wisdom Labs. All Rights Reserved

#pragma once
#include "CoreMinimal.h"
#include "Models/FUInt64.h"

#include "UAlgorandBPFunctionLibrary.generated.h"

/**
 * @brief BlueprintFunctionLibrary
 * how library converts user-defined and standard data type
 * to wanted one on blueprint
 */

UCLASS()
class UAlgorandBPFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * @brief convert new defined FUInt64 to FString
     * @param value value for convert
     * @return converted value as string 
     */
    UFUNCTION(BlueprintPure,
        meta = (DisplayName = "FUInt64 To String", CompactNodeTitle = "->",
            BlueprintAutocast),
        Category = "FUInt64")
        static FString Conv_FUInt64ToString(const FUInt64& value);

    /**
     * @brief convert FString to new defined FUInt64
     * @param value value for convert
     * @return converted value as FUInt64 
     */
    UFUNCTION(BlueprintPure,
        meta = (DisplayName = "String FUInt64", CompactNodeTitle = "->",
            BlueprintAutocast),
        Category = "String")
        static FUInt64 Conv_FStringToUInt64(const FString& value);
};
