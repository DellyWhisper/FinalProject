// Copyright Epic Games, Inc. All Rights Reserved.

#include "AbilitySystem/Core/WAS_JRPG_AttributeSetBase.h"

#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

UWAS_JRPG_AttributeSetBase::UWAS_JRPG_AttributeSetBase()
{
	InitHealth(100.0f);
	InitMaxHealth(100.0f);
	InitMana(50.0f);
	InitMaxMana(50.0f);
	InitStamina(100.0f);
	InitMaxStamina(100.0f);
	InitMoveSpeed(600.0f);
}

void UWAS_JRPG_AttributeSetBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UWAS_JRPG_AttributeSetBase, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWAS_JRPG_AttributeSetBase, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWAS_JRPG_AttributeSetBase, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWAS_JRPG_AttributeSetBase, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWAS_JRPG_AttributeSetBase, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWAS_JRPG_AttributeSetBase, MaxStamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWAS_JRPG_AttributeSetBase, MoveSpeed, COND_None, REPNOTIFY_Always);
}

void UWAS_JRPG_AttributeSetBase::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	NewValue = ClampAttributeValue(Attribute, NewValue);
}

void UWAS_JRPG_AttributeSetBase::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(ClampAttributeValue(GetHealthAttribute(), GetHealth()));
	}
	else if (Data.EvaluatedData.Attribute == GetMaxHealthAttribute())
	{
		SetMaxHealth(ClampAttributeValue(GetMaxHealthAttribute(), GetMaxHealth()));
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	}
	else if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(ClampAttributeValue(GetManaAttribute(), GetMana()));
	}
	else if (Data.EvaluatedData.Attribute == GetMaxManaAttribute())
	{
		SetMaxMana(ClampAttributeValue(GetMaxManaAttribute(), GetMaxMana()));
		SetMana(FMath::Clamp(GetMana(), 0.0f, GetMaxMana()));
	}
	else if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		SetStamina(ClampAttributeValue(GetStaminaAttribute(), GetStamina()));
	}
	else if (Data.EvaluatedData.Attribute == GetMaxStaminaAttribute())
	{
		SetMaxStamina(ClampAttributeValue(GetMaxStaminaAttribute(), GetMaxStamina()));
		SetStamina(FMath::Clamp(GetStamina(), 0.0f, GetMaxStamina()));
	}
	else if (Data.EvaluatedData.Attribute == GetMoveSpeedAttribute())
	{
		SetMoveSpeed(ClampAttributeValue(GetMoveSpeedAttribute(), GetMoveSpeed()));
	}
}

void UWAS_JRPG_AttributeSetBase::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWAS_JRPG_AttributeSetBase, Health, OldValue);
}

void UWAS_JRPG_AttributeSetBase::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWAS_JRPG_AttributeSetBase, MaxHealth, OldValue);
}

void UWAS_JRPG_AttributeSetBase::OnRep_Mana(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWAS_JRPG_AttributeSetBase, Mana, OldValue);
}

void UWAS_JRPG_AttributeSetBase::OnRep_MaxMana(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWAS_JRPG_AttributeSetBase, MaxMana, OldValue);
}

void UWAS_JRPG_AttributeSetBase::OnRep_Stamina(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWAS_JRPG_AttributeSetBase, Stamina, OldValue);
}

void UWAS_JRPG_AttributeSetBase::OnRep_MaxStamina(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWAS_JRPG_AttributeSetBase, MaxStamina, OldValue);
}

void UWAS_JRPG_AttributeSetBase::OnRep_MoveSpeed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWAS_JRPG_AttributeSetBase, MoveSpeed, OldValue);
}

float UWAS_JRPG_AttributeSetBase::ClampAttributeValue(const FGameplayAttribute& Attribute, float Value) const
{
	if (Attribute == GetHealthAttribute())
	{
		return FMath::Clamp(Value, 0.0f, GetMaxHealth());
	}

	if (Attribute == GetMaxHealthAttribute())
	{
		return FMath::Max(Value, 1.0f);
	}

	if (Attribute == GetManaAttribute())
	{
		return FMath::Clamp(Value, 0.0f, GetMaxMana());
	}

	if (Attribute == GetMaxManaAttribute())
	{
		return FMath::Max(Value, 0.0f);
	}

	if (Attribute == GetStaminaAttribute())
	{
		return FMath::Clamp(Value, 0.0f, GetMaxStamina());
	}

	if (Attribute == GetMaxStaminaAttribute())
	{
		return FMath::Max(Value, 0.0f);
	}

	if (Attribute == GetMoveSpeedAttribute())
	{
		return FMath::Max(Value, 0.0f);
	}

	return Value;
}
