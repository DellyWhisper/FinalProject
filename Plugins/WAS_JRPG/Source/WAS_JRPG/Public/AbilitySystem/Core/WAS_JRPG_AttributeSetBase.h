// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "WAS_JRPG_AttributeSetBase.generated.h"

#define WAS_JRPG_ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS(BlueprintType)
class WAS_JRPG_API UWAS_JRPG_AttributeSetBase : public UAttributeSet
{
	GENERATED_BODY()

public:
	UWAS_JRPG_AttributeSetBase();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "WAS|Attributes")
	FGameplayAttributeData Health;
	WAS_JRPG_ATTRIBUTE_ACCESSORS(UWAS_JRPG_AttributeSetBase, Health)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "WAS|Attributes")
	FGameplayAttributeData MaxHealth;
	WAS_JRPG_ATTRIBUTE_ACCESSORS(UWAS_JRPG_AttributeSetBase, MaxHealth)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "WAS|Attributes")
	FGameplayAttributeData Mana;
	WAS_JRPG_ATTRIBUTE_ACCESSORS(UWAS_JRPG_AttributeSetBase, Mana)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "WAS|Attributes")
	FGameplayAttributeData MaxMana;
	WAS_JRPG_ATTRIBUTE_ACCESSORS(UWAS_JRPG_AttributeSetBase, MaxMana)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Stamina, Category = "WAS|Attributes")
	FGameplayAttributeData Stamina;
	WAS_JRPG_ATTRIBUTE_ACCESSORS(UWAS_JRPG_AttributeSetBase, Stamina)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxStamina, Category = "WAS|Attributes")
	FGameplayAttributeData MaxStamina;
	WAS_JRPG_ATTRIBUTE_ACCESSORS(UWAS_JRPG_AttributeSetBase, MaxStamina)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MoveSpeed, Category = "WAS|Attributes")
	FGameplayAttributeData MoveSpeed;
	WAS_JRPG_ATTRIBUTE_ACCESSORS(UWAS_JRPG_AttributeSetBase, MoveSpeed)

protected:
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_Stamina(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxStamina(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MoveSpeed(const FGameplayAttributeData& OldValue);

private:
	float ClampAttributeValue(const FGameplayAttribute& Attribute, float Value) const;
};
