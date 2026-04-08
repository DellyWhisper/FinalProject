// Copyright Epic Games, Inc. All Rights Reserved.

#include "AbilitySystem/Core/WAS_JRPG_GameplayTags.h"

#include "GameplayTagsManager.h"

namespace WASJRPG::GameplayTags
{
	static FWAS_JRPG_GameplayTags RegisteredTags;
	static bool bInitialized = false;
}

const FWAS_JRPG_GameplayTags& FWAS_JRPG_GameplayTags::Get()
{
	if (!WASJRPG::GameplayTags::bInitialized)
	{
		InitializeNativeTags();
	}

	return WASJRPG::GameplayTags::RegisteredTags;
}

void FWAS_JRPG_GameplayTags::InitializeNativeTags()
{
	if (WASJRPG::GameplayTags::bInitialized)
	{
		return;
	}

	WASJRPG::GameplayTags::bInitialized = true;

	UGameplayTagsManager& Manager = UGameplayTagsManager::Get();
	WASJRPG::GameplayTags::RegisteredTags.AddAllTags(Manager);
}

void FWAS_JRPG_GameplayTags::AddAllTags(UGameplayTagsManager& Manager)
{
	Input_Ability_Primary = Manager.AddNativeGameplayTag(TEXT("Input.Ability.Primary"), TEXT("Primary ability input tag."));
	Input_Ability_Secondary = Manager.AddNativeGameplayTag(TEXT("Input.Ability.Secondary"), TEXT("Secondary ability input tag."));
	Ability_Activate_OnSpawn = Manager.AddNativeGameplayTag(TEXT("Ability.Activate.OnSpawn"), TEXT("Ability should activate when granted at startup."));
	Ability_Fail_Cooldown = Manager.AddNativeGameplayTag(TEXT("Ability.Fail.Cooldown"), TEXT("Ability activation failed because the cooldown is active."));
	Ability_Fail_Cost = Manager.AddNativeGameplayTag(TEXT("Ability.Fail.Cost"), TEXT("Ability activation failed because the cost could not be paid."));
	State_Ready = Manager.AddNativeGameplayTag(TEXT("State.Ready"), TEXT("Character is ready for ability input."));
	Status_Dead = Manager.AddNativeGameplayTag(TEXT("Status.Dead"), TEXT("Character has no remaining health."));
	Status_Stunned = Manager.AddNativeGameplayTag(TEXT("Status.Stunned"), TEXT("Character is unable to act."));
	Event_Death = Manager.AddNativeGameplayTag(TEXT("Event.Death"), TEXT("Gameplay event fired when a character dies."));
}
