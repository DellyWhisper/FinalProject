// Copyright Epic Games, Inc. All Rights Reserved.

#include "AbilitySystem/Core/WAS_JRPG_AbilitySystemComponent.h"

#include "AbilitySystem/Core/WAS_JRPG_AbilitySet.h"

bool UWAS_JRPG_AbilitySystemComponent::GrantAbilitySet(const UWAS_JRPG_AbilitySet* AbilitySet, FWAS_JRPG_AbilitySetGrantedHandles* OutGrantedHandles, UObject* SourceObject)
{
	if (!AbilitySet)
	{
		return false;
	}

	return AbilitySet->GiveToAbilitySystem(this, OutGrantedHandles, SourceObject);
}

void UWAS_JRPG_AbilitySystemComponent::ClearAbilitySet(FWAS_JRPG_AbilitySetGrantedHandles& GrantedHandles)
{
	GrantedHandles.TakeFromAbilitySystem(this);
}

bool UWAS_JRPG_AbilitySystemComponent::TryActivateAbilitiesByInputTag(FGameplayTag InputTag, bool bAllowRemoteActivation)
{
	if (!InputTag.IsValid())
	{
		return false;
	}

	FGameplayTagContainer AbilityTags;
	AbilityTags.AddTag(InputTag);
	return TryActivateAbilitiesByInputTags(AbilityTags, bAllowRemoteActivation);
}

bool UWAS_JRPG_AbilitySystemComponent::TryActivateAbilitiesByInputTags(FGameplayTagContainer AbilityTags, bool bAllowRemoteActivation)
{
	if (AbilityTags.IsEmpty())
	{
		return false;
	}

	return TryActivateAbilitiesByTag(AbilityTags, bAllowRemoteActivation);
}
