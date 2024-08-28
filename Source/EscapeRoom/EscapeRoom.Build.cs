// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class EscapeRoom : ModuleRules
{
	public EscapeRoom(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "NavigationSystem", "AIModule", "Core", "CoreUObject", "Engine", "InputCore", "GameplayTags", "EnhancedInput" });
	}
}
