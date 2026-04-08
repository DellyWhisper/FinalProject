// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WAS_JRPG_AbilityActivationPolicy.generated.h"

UENUM(BlueprintType)
enum class EWAS_JRPG_AbilityActivationPolicy : uint8
{
	OnInputTriggered UMETA(DisplayName = "On Input Triggered"),
	WhileInputActive UMETA(DisplayName = "While Input Active"),
	OnSpawn UMETA(DisplayName = "On Spawn")
};
