// Copyright Epic Games, Inc. All Rights Reserved.

#include "AbilitySystem/Effects/WAS_JRPG_InitAttributesEffect.h"

#include "AbilitySystem/Core/WAS_JRPG_AttributeSetBase.h"

UWAS_JRPG_InitAttributesEffect::UWAS_JRPG_InitAttributesEffect()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	auto AddOverrideModifier = [this](const FGameplayAttribute& Attribute, float MagnitudeValue)
	{
		FGameplayModifierInfo& Modifier = Modifiers.AddDefaulted_GetRef();
		Modifier.Attribute = Attribute;
		Modifier.ModifierOp = EGameplayModOp::Override;
		Modifier.ModifierMagnitude = FScalableFloat(MagnitudeValue);
	};

	AddOverrideModifier(UWAS_JRPG_AttributeSetBase::GetMaxHealthAttribute(), 100.0f);
	AddOverrideModifier(UWAS_JRPG_AttributeSetBase::GetHealthAttribute(), 100.0f);
	AddOverrideModifier(UWAS_JRPG_AttributeSetBase::GetMaxManaAttribute(), 50.0f);
	AddOverrideModifier(UWAS_JRPG_AttributeSetBase::GetManaAttribute(), 50.0f);
	AddOverrideModifier(UWAS_JRPG_AttributeSetBase::GetMaxStaminaAttribute(), 100.0f);
	AddOverrideModifier(UWAS_JRPG_AttributeSetBase::GetStaminaAttribute(), 100.0f);
	AddOverrideModifier(UWAS_JRPG_AttributeSetBase::GetMoveSpeedAttribute(), 600.0f);
}
