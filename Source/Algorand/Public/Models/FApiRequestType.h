#pragma once

UENUM(BlueprintType)
enum class EApiRequestType : uint8 {
	NOOP = 0 UMETA(DisplayName = "NOOP"),
	OPT_IN = 1 UMETA(DisplayName = "OPT_IN")
};