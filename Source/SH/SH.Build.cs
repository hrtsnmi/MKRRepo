// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SH : ModuleRules
{
	public SH(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "UMG", "MotionWarping" });

        // Uncomment if you are using Slate UI
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
        PublicDependencyModuleNames.Add("AIModule");
        PublicDependencyModuleNames.Add("GameplayTasks");

        PublicDependencyModuleNames.Add("NavigationSystem");
    }
}
