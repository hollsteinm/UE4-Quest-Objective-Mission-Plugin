// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MissionFrameworkEditor : ModuleRules
{
	public MissionFrameworkEditor(TargetInfo Target)
	{
		
		PublicIncludePaths.AddRange(
			new string[] {
				"MissionFrameworkEditor/Public"
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				"MissionFrameworkEditor/Private",
                "AssetTools",
                "AssetRegistry"
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"MissionFramework",
                "Json",
                "JsonUtilities"
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Projects",
				"InputCore",
				"UnrealEd",
                "LevelEditor",
                "SceneOutliner",
                "CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
                "EditorStyle",
                "PropertyEditor",
                "DetailCustomizations"
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
                "AssetTools",
                "AssetRegistry"
            }
			);
	}
}
