// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Merchants : ModuleRules
{
	public Merchants(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "HeadMountedDisplay", "EnhancedInput", "AIModule", "NavigationSystem", "AnimGraphRuntime" });

        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
    }
}
