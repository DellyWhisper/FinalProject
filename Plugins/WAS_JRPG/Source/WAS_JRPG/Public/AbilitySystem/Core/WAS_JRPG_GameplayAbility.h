// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/Types/WAS_JRPG_AbilityActivationPolicy.h"
#include "WAS_JRPG_GameplayAbility.generated.h"

UCLASS(Abstract, Blueprintable)
class WAS_JRPG_API UWAS_JRPG_GameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UWAS_JRPG_GameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintPure, Category = "WAS|Ability")
	FGameplayTag GetInputTag() const;

	UFUNCTION(BlueprintPure, Category = "WAS|Ability")
	EWAS_JRPG_AbilityActivationPolicy GetActivationPolicy() const;

	UFUNCTION(BlueprintPure, Category = "WAS|Ability")
	bool ShouldActivateOnSpawn() const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WAS|Ability")
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WAS|Ability")
	EWAS_JRPG_AbilityActivationPolicy ActivationPolicy = EWAS_JRPG_AbilityActivationPolicy::OnInputTriggered;
};
