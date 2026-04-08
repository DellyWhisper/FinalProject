// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/Core/WAS_JRPG_AbilitySet.h"
#include "WAS_JRPG_CharacterBase.generated.h"

class UGameplayEffect;
class UAbilitySystemComponent;
class UWAS_JRPG_AbilitySystemComponent;
class UWAS_JRPG_AttributeSetBase;
struct FOnAttributeChangeData;

UCLASS(Abstract, Blueprintable)
class WAS_JRPG_API AWAS_JRPG_CharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AWAS_JRPG_CharacterBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_Controller() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintPure, Category = "WAS|AbilitySystem")
	UWAS_JRPG_AbilitySystemComponent* GetWASAbilitySystemComponent() const;

	UFUNCTION(BlueprintPure, Category = "WAS|AbilitySystem")
	UWAS_JRPG_AttributeSetBase* GetAttributeSetBase() const;

	UFUNCTION(BlueprintPure, Category = "WAS|Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintPure, Category = "WAS|Attributes")
	float GetMaxHealth() const;

protected:
	virtual void InitializeAbilitySystem();
	virtual void InitializeAbilityActorInfo();
	virtual void GrantStartupAbilitySets();
	virtual void ApplyStartupEffects();
	virtual void HandleOutOfHealth();
	virtual void HandleHealthChanged(const FOnAttributeChangeData& ChangeData);

	UFUNCTION(BlueprintNativeEvent, Category = "WAS|AbilitySystem")
	void OnAbilitySystemInitialized();
	virtual void OnAbilitySystemInitialized_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "WAS|AbilitySystem")
	void OnDeath();
	virtual void OnDeath_Implementation();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WAS|AbilitySystem", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWAS_JRPG_AbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WAS|AbilitySystem", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWAS_JRPG_AttributeSetBase> AttributeSetBase;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WAS|AbilitySystem")
	TArray<TObjectPtr<UWAS_JRPG_AbilitySet>> StartupAbilitySets;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WAS|AbilitySystem")
	TArray<TSubclassOf<UGameplayEffect>> StartupGameplayEffects;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WAS|AbilitySystem", meta = (ClampMin = "0.0"))
	float StartupGameplayEffectLevel = 1.0f;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "WAS|AbilitySystem")
	bool bStartupAbilitiesGranted = false;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "WAS|AbilitySystem")
	bool bAbilitySystemInitialized = false;

private:
	FWAS_JRPG_AbilitySetGrantedHandles StartupGrantedHandles;
	FDelegateHandle HealthChangedDelegateHandle;
	bool bStartupEffectsApplied = false;
	bool bDeathHandled = false;
};
