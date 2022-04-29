// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

namespace UnrealBuildTool.Rules
{
	public class MissionFramework : ModuleRules
	{
		public MissionFramework(TargetInfo Target)
		{
			PublicIncludePaths.AddRange(
				new string[] {
					"MissionFramework/Public"
				}
				);

			PrivateIncludePaths.AddRange(
				new string[] {
					"MissionFramework/Private",
				}
				);

			PublicDependencyModuleNames.AddRange(
				new string[]
				{
					"Core",
					"CoreUObject",
					"Engine",
                    "InputCore"
                }
				);

			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
				}
				);

			DynamicallyLoadedModuleNames.AddRange(
				new string[]
				{
				}
				);
		}
	}
}