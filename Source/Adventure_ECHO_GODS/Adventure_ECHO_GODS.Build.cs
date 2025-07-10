// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Adventure_ECHO_GODS : ModuleRules
{
	public Adventure_ECHO_GODS(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(new string[] {
			"Core", "CoreUObject", "Engine", "InputCore"
		});

		PrivateDependencyModuleNames.AddRange(new string[] {
			"Slate", "SlateCore"
		});

		// Only include editor modules when building the editor
		if (Target.bBuildEditor)
		{
			PrivateDependencyModuleNames.AddRange(new string[] {
				"EditorStyle", "UnrealEd", "EditorSubsystem", "Foliage", "FoliageEdit"
			});
		}

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
