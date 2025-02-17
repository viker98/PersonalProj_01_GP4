// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PersonalProj01_GP4 : ModuleRules
{
	public PersonalProj01_GP4(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
