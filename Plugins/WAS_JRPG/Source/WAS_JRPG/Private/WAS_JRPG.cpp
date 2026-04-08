// Copyright Epic Games, Inc. All Rights Reserved.

#include "WAS_JRPG.h"
#include "AbilitySystem/Core/WAS_JRPG_GameplayTags.h"

#define LOCTEXT_NAMESPACE "FWAS_JRPGModule"

void FWAS_JRPGModule::StartupModule()
{
	FWAS_JRPG_GameplayTags::InitializeNativeTags();
}

void FWAS_JRPGModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FWAS_JRPGModule, WAS_JRPG)
