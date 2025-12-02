// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MgbDebugTool : ModuleRules
{
	public MgbDebugTool(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
			"Megabonky",
			"Megabonky/Core/AbilitySystem/AttributeSet"
            }
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "GameplayAbilities",
                "GameplayTags",
                "GameplayTasks",
                "UnrealEd",
                "PropertyEditor",
                "Megabonky",
            }
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
                "InputCore",
                "EditorSubsystem",
                "LevelEditor"
				// ... add private dependencies that you statically link with here ...	
            }
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
