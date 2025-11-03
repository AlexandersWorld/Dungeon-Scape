// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DungeoScape : ModuleRules
{
	public DungeoScape(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"DungeoScape",
			"DungeoScape/Variant_Horror",
			"DungeoScape/Variant_Horror/UI",
			"DungeoScape/Variant_Shooter",
			"DungeoScape/Variant_Shooter/AI",
			"DungeoScape/Variant_Shooter/UI",
			"DungeoScape/Variant_Shooter/Weapons"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
