// Copyright Epic Games, Inc. All Rights Reserved.

#include "AbilitySystem/Core/WAS_JRPG_AbilitySet.h"

#include "AbilitySystem/Core/WAS_JRPG_GameplayAbility.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayEffect.h"
#include "GameFramework/Actor.h"

void FWAS_JRPG_AbilitySetGrantedHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
	if (Handle.IsValid())
	{
		AbilitySpecHandles.Add(Handle);
	}
}

void FWAS_JRPG_AbilitySetGrantedHandles::AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle)
{
	if (Handle.IsValid())
	{
		GameplayEffectHandles.Add(Handle);
	}
}

void FWAS_JRPG_AbilitySetGrantedHandles::TakeFromAbilitySystem(UAbilitySystemComponent* AbilitySystemComponent)
{
	if (!AbilitySystemComponent)
	{
		Reset();
		return;
	}

	for (const FGameplayAbilitySpecHandle& Handle : AbilitySpecHandles)
	{
		if (Handle.IsValid())
		{
			AbilitySystemComponent->ClearAbility(Handle);
		}
	}

	for (const FActiveGameplayEffectHandle& Handle : GameplayEffectHandles)
	{
		if (Handle.IsValid())
		{
			AbilitySystemComponent->RemoveActiveGameplayEffect(Handle);
		}
	}

	Reset();
}

bool FWAS_JRPG_AbilitySetGrantedHandles::IsEmpty() const
{
	return AbilitySpecHandles.IsEmpty() && GameplayEffectHandles.IsEmpty();
}

void FWAS_JRPG_AbilitySetGrantedHandles::Reset()
{
	AbilitySpecHandles.Reset();
	GameplayEffectHandles.Reset();
}

bool UWAS_JRPG_AbilitySet::GiveToAbilitySystem(UAbilitySystemComponent* AbilitySystemComponent, FWAS_JRPG_AbilitySetGrantedHandles* OutGrantedHandles, UObject* SourceObject) const
{
	if (!AbilitySystemComponent)
	{
		return false;
	}

	AActor* OwnerActor = AbilitySystemComponent->GetOwnerActor();
	if (!OwnerActor || !OwnerActor->HasAuthority())
	{
		return false;
	}

	bool bGrantedAnything = false;

	for (const FWAS_JRPG_GrantedAbility& GrantedAbility : GrantedAbilities)
	{
		if (!GrantedAbility.bGrantOnSpawn || !GrantedAbility.IsValid())
		{
			continue;
		}

		FGameplayAbilitySpec AbilitySpec(GrantedAbility.AbilityClass, GrantedAbility.AbilityLevel);
		AbilitySpec.SourceObject = SourceObject;

		if (GrantedAbility.InputTag.IsValid())
		{
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(GrantedAbility.InputTag);
		}

		const FGameplayAbilitySpecHandle AbilityHandle = AbilitySystemComponent->GiveAbility(AbilitySpec);
		bGrantedAnything = bGrantedAnything || AbilityHandle.IsValid();

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddAbilitySpecHandle(AbilityHandle);
		}

		const UGameplayAbility* AbilityCDO = GrantedAbility.AbilityClass.GetDefaultObject();
		const UWAS_JRPG_GameplayAbility* JRPGAbilityCDO = Cast<UWAS_JRPG_GameplayAbility>(AbilityCDO);
		if (JRPGAbilityCDO && JRPGAbilityCDO->ShouldActivateOnSpawn())
		{
			AbilitySystemComponent->TryActivateAbility(AbilityHandle);
		}
	}

	for (const FWAS_JRPG_GrantedGameplayEffect& GrantedEffect : GrantedEffects)
	{
		if (!GrantedEffect.IsValid())
		{
			continue;
		}

		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(SourceObject ? SourceObject : OwnerActor);

		const FGameplayEffectSpecHandle EffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(GrantedEffect.EffectClass, GrantedEffect.EffectLevel, EffectContext);
		if (!EffectSpecHandle.IsValid())
		{
			continue;
		}

		const FActiveGameplayEffectHandle ActiveEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
		bGrantedAnything = bGrantedAnything || ActiveEffectHandle.IsValid();

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddGameplayEffectHandle(ActiveEffectHandle);
		}
	}

	return bGrantedAnything;
}

void UWAS_JRPG_AbilitySet::ConfigureDefaultInitEffect(TSubclassOf<UGameplayEffect> EffectClass, float EffectLevel)
{
	GrantedAbilities.Reset();
	GrantedEffects.Reset();

	if (!EffectClass)
	{
		return;
	}

	FWAS_JRPG_GrantedGameplayEffect& GrantedEffect = GrantedEffects.AddDefaulted_GetRef();
	GrantedEffect.EffectClass = EffectClass;
	GrantedEffect.EffectLevel = EffectLevel;
}

const TArray<FWAS_JRPG_GrantedAbility>& UWAS_JRPG_AbilitySet::GetGrantedAbilities() const
{
	return GrantedAbilities;
}

const TArray<FWAS_JRPG_GrantedGameplayEffect>& UWAS_JRPG_AbilitySet::GetGrantedEffects() const
{
	return GrantedEffects;
}
