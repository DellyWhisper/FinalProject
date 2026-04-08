// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class WAS_JRPG : ModuleRules
{
	public WAS_JRPG(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"GameplayAbilities",
				"GameplayTags",
				"GameplayTasks",
				"PaperZD"
			}
			);

		PrivateDependencyModuleNames.AddRange(new string[] { });
	}
}
