// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Types/WAS_JRPG_GrantedAbility.h"
#include "WAS_JRPG_AbilitySet.generated.h"

class UGameplayEffect;

USTRUCT(BlueprintType)
struct WAS_JRPG_API FWAS_JRPG_GrantedGameplayEffect
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilitySet")
	TSubclassOf<UGameplayEffect> EffectClass = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilitySet", meta = (ClampMin = "0.0"))
	float EffectLevel = 1.0f;

	bool IsValid() const
	{
		return EffectClass != nullptr;
	}
};

USTRUCT()
struct WAS_JRPG_API FWAS_JRPG_AbilitySetGrantedHandles
{
	GENERATED_BODY()

public:
	void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);
	void AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle);
	void TakeFromAbilitySystem(UAbilitySystemComponent* AbilitySystemComponent);
	bool IsEmpty() const;
	void Reset();

private:
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;

	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> GameplayEffectHandles;
};

UCLASS(BlueprintType)
class WAS_JRPG_API UWAS_JRPG_AbilitySet : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	bool GiveToAbilitySystem(UAbilitySystemComponent* AbilitySystemComponent, FWAS_JRPG_AbilitySetGrantedHandles* OutGrantedHandles, UObject* SourceObject = nullptr) const;

	const TArray<FWAS_JRPG_GrantedAbility>& GetGrantedAbilities() const;
	const TArray<FWAS_JRPG_GrantedGameplayEffect>& GetGrantedEffects() const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilitySet")
	TArray<FWAS_JRPG_GrantedAbility> GrantedAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilitySet")
	TArray<FWAS_JRPG_GrantedGameplayEffect> GrantedEffects;
};
