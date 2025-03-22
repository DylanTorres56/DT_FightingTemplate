// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DT_FightingTemplate : ModuleRules
{
	public DT_FightingTemplate(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });
	}
}
