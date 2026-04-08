// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/WAS_JRPG_CharacterBase.h"
#include "WAS_Character_Base.generated.h"

UCLASS()
class FINALPROJECT_API AWAS_Character_Base : public AWAS_JRPG_CharacterBase
{
	GENERATED_BODY()
	
public:	
	AWAS_Character_Base(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
