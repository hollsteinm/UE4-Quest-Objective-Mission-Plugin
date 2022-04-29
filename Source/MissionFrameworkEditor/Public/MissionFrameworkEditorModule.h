// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ModuleManager.h"
#include "LevelEditor.h"

class FToolBarBuilder;
class FMenuBuilder;

class FMissionFrameworkEditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command (by default it will bring up plugin window) */
	void PluginButtonClicked();
	void OnAddMissionClicked();
	
private:

	void AddToolbarExtension(FToolBarBuilder& Builder);
	void AddMenuExtension(FMenuBuilder& Builder);

	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);

	virtual void BindCommands();
	virtual void HookIntoLevelEditor();

	virtual void SetActiveWorld(class UWorld* NewWorld, EMapChangeType Reason);

private:
	TSharedPtr<class FUICommandList> PluginCommands;
	class UWorld* MyWorld;

	void RegisterCustomLayouts();
	void UnregisterCustomLayouts();
};