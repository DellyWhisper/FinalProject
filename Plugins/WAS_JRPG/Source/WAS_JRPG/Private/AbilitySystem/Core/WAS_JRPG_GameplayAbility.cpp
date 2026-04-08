// Copyright Epic Games, Inc. All Rights Reserved.

#include "AbilitySystem/Core/WAS_JRPG_GameplayAbility.h"

UWAS_JRPG_GameplayAbility::UWAS_JRPG_GameplayAbility(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

FGameplayTag UWAS_JRPG_GameplayAbility::GetInputTag() const
{
	return InputTag;
}

EWAS_JRPG_AbilityActivationPolicy UWAS_JRPG_GameplayAbility::GetActivationPolicy() const
{
	return ActivationPolicy;
}

bool UWAS_JRPG_GameplayAbility::ShouldActivateOnSpawn() const
{
	return ActivationPolicy == EWAS_JRPG_AbilityActivationPolicy::OnSpawn;
}
