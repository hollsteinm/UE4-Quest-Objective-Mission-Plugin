// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "SlateBasics.h"
#include "MissionFrameworkEditorStyle.h"

class FMissionFrameworkEditorCommands : public TCommands<FMissionFrameworkEditorCommands>
{
public:

	FMissionFrameworkEditorCommands()
		: TCommands<FMissionFrameworkEditorCommands>(TEXT("MissionFrameworkEditorModule"), NSLOCTEXT("Contexts", "MissionFrameworkEditorModule", "Mission Framework Editor Plugin"), NAME_None, FMissionFrameworkEditorStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
	TSharedPtr< FUICommandInfo > AddMission;
	TSharedPtr< FUICommandInfo > ExpandPluginToolbar;
};