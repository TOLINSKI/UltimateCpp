// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UltimateCpp : ModuleRules
{
	public UltimateCpp(ReadOnlyTargetRules Target) : base(Target)
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
			"Slate",
			"AnimGraphRuntime",
			"BaseCraft"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"UltimateCpp",
			"UltimateCpp/Variant_Platforming",
			"UltimateCpp/Variant_Platforming/Animation",
			"UltimateCpp/Variant_Combat",
			"UltimateCpp/Variant_Combat/AI",
			"UltimateCpp/Variant_Combat/Animation",
			"UltimateCpp/Variant_Combat/Gameplay",
			"UltimateCpp/Variant_Combat/Interfaces",
			"UltimateCpp/Variant_Combat/UI",
			"UltimateCpp/Variant_SideScrolling",
			"UltimateCpp/Variant_SideScrolling/AI",
			"UltimateCpp/Variant_SideScrolling/Gameplay",
			"UltimateCpp/Variant_SideScrolling/Interfaces",
			"UltimateCpp/Variant_SideScrolling/UI",
			"UltimateCpp/Slash"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
