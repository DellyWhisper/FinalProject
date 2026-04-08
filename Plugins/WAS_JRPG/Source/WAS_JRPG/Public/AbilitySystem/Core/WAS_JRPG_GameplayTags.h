// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

class UGameplayTagsManager;

struct WAS_JRPG_API FWAS_JRPG_GameplayTags
{
public:
	static const FWAS_JRPG_GameplayTags& Get();
	static void InitializeNativeTags();

	FGameplayTag Input_Ability_Primary;
	FGameplayTag Input_Ability_Secondary;
	FGameplayTag Ability_Activate_OnSpawn;
	FGameplayTag Ability_Fail_Cooldown;
	FGameplayTag Ability_Fail_Cost;
	FGameplayTag State_Ready;
	FGameplayTag Status_Dead;
	FGameplayTag Status_Stunned;
	FGameplayTag Event_Death;

private:
	void AddAllTags(UGameplayTagsManager& Manager);
};
