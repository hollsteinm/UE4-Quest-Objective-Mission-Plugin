// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "MissionFrameworkEditorPrivatePCH.h"
#include "MissionFrameworkEditorCommands.h"

#define LOCTEXT_NAMESPACE "FMissionFrameworkEditorModule"

void FMissionFrameworkEditorCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "Mission", "Bring up the Mission Framework Editor window", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(AddMission, "Add Mission", "Create a new AMissionLevelActor", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(ExpandPluginToolbar, "Mission", "Create a new AMissionLevelActor", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
