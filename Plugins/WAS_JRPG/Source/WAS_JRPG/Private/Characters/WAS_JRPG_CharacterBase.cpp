// Copyright Epic Games, Inc. All Rights Reserved.

#include "Characters/WAS_JRPG_CharacterBase.h"

#include "AbilitySystem/Core/WAS_JRPG_AbilitySystemComponent.h"
#include "AbilitySystem/Core/WAS_JRPG_AttributeSetBase.h"
#include "GameplayEffect.h"

AWAS_JRPG_CharacterBase::AWAS_JRPG_CharacterBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	AbilitySystemComponent = CreateDefaultSubobject<UWAS_JRPG_AbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSetBase = CreateDefaultSubobject<UWAS_JRPG_AttributeSetBase>(TEXT("AttributeSetBase"));
}

UAbilitySystemComponent* AWAS_JRPG_CharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AWAS_JRPG_CharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitializeAbilitySystem();
}

void AWAS_JRPG_CharacterBase::OnRep_Controller()
{
	Super::OnRep_Controller();

	InitializeAbilitySystem();
}

void AWAS_JRPG_CharacterBase::BeginPlay()
{
	Super::BeginPlay();

	InitializeAbilitySystem();
}

void AWAS_JRPG_CharacterBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (AbilitySystemComponent && HealthChangedDelegateHandle.IsValid())
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UWAS_JRPG_AttributeSetBase::GetHealthAttribute()).Remove(HealthChangedDelegateHandle);
		HealthChangedDelegateHandle.Reset();
	}

	Super::EndPlay(EndPlayReason);
}

UWAS_JRPG_AbilitySystemComponent* AWAS_JRPG_CharacterBase::GetWASAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UWAS_JRPG_AttributeSetBase* AWAS_JRPG_CharacterBase::GetAttributeSetBase() const
{
	return AttributeSetBase;
}

float AWAS_JRPG_CharacterBase::GetHealth() const
{
	return AttributeSetBase ? AttributeSetBase->GetHealth() : 0.0f;
}

float AWAS_JRPG_CharacterBase::GetMaxHealth() const
{
	return AttributeSetBase ? AttributeSetBase->GetMaxHealth() : 0.0f;
}

void AWAS_JRPG_CharacterBase::InitializeAbilitySystem()
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	InitializeAbilityActorInfo();

	if (!HealthChangedDelegateHandle.IsValid())
	{
		HealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UWAS_JRPG_AttributeSetBase::GetHealthAttribute()).AddUObject(this, &AWAS_JRPG_CharacterBase::HandleHealthChanged);
	}

	if (HasAuthority())
	{
		GrantStartupAbilitySets();
		ApplyStartupEffects();
	}
}

void AWAS_JRPG_CharacterBase::InitializeAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	if (!bAbilitySystemInitialized)
	{
		bAbilitySystemInitialized = true;
		OnAbilitySystemInitialized();
	}
}

void AWAS_JRPG_CharacterBase::GrantStartupAbilitySets()
{
	if (!AbilitySystemComponent || !HasAuthority() || bStartupAbilitiesGranted)
	{
		return;
	}

	for (UWAS_JRPG_AbilitySet* AbilitySet : StartupAbilitySets)
	{
		if (AbilitySet)
		{
			AbilitySystemComponent->GrantAbilitySet(AbilitySet, &StartupGrantedHandles, this);
		}
	}

	bStartupAbilitiesGranted = true;
}

void AWAS_JRPG_CharacterBase::ApplyStartupEffects()
{
	if (!AbilitySystemComponent || !HasAuthority() || bStartupEffectsApplied)
	{
		return;
	}

	for (const TSubclassOf<UGameplayEffect>& GameplayEffectClass : StartupGameplayEffects)
	{
		if (!GameplayEffectClass)
		{
			continue;
		}

		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		const FGameplayEffectSpecHandle EffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffectClass, StartupGameplayEffectLevel, EffectContext);
		if (!EffectSpecHandle.IsValid())
		{
			continue;
		}

		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	}

	bStartupEffectsApplied = true;
}

void AWAS_JRPG_CharacterBase::HandleOutOfHealth()
{
	OnDeath();
}

void AWAS_JRPG_CharacterBase::HandleHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	if (bDeathHandled)
	{
		return;
	}

	if (ChangeData.OldValue > 0.0f && ChangeData.NewValue <= 0.0f)
	{
		bDeathHandled = true;
		HandleOutOfHealth();
	}
}

void AWAS_JRPG_CharacterBase::OnAbilitySystemInitialized_Implementation()
{
}

void AWAS_JRPG_CharacterBase::OnDeath_Implementation()
{
}
