// Copyright Epic Games, Inc. All Rights Reserved.

#include "WAS_JRPG.h"

#define LOCTEXT_NAMESPACE "FWAS_JRPGModule"

void FWAS_JRPGModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FWAS_JRPGModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FWAS_JRPGModule, WAS_JRPG)