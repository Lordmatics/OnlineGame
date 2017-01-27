// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class OnlineGame : ModuleRules
{
	public OnlineGame(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG", "Slate", "SlateCore", "OnlineSubsystem" });

        DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");
    }
}
