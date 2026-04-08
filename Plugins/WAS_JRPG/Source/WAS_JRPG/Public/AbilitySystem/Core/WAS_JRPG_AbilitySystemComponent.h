// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "WAS_JRPG_AbilitySystemComponent.generated.h"

class UWAS_JRPG_AbilitySet;
struct FWAS_JRPG_AbilitySetGrantedHandles;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class WAS_JRPG_API UWAS_JRPG_AbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	bool GrantAbilitySet(const UWAS_JRPG_AbilitySet* AbilitySet, FWAS_JRPG_AbilitySetGrantedHandles* OutGrantedHandles, UObject* SourceObject = nullptr);
	void ClearAbilitySet(FWAS_JRPG_AbilitySetGrantedHandles& GrantedHandles);

	UFUNCTION(BlueprintCallable, Category = "WAS|AbilitySystem")
	bool TryActivateAbilitiesByInputTag(FGameplayTag InputTag, bool bAllowRemoteActivation = true);

	UFUNCTION(BlueprintCallable, Category = "WAS|AbilitySystem")
	bool TryActivateAbilitiesByInputTags(FGameplayTagContainer AbilityTags, bool bAllowRemoteActivation = true);
};
