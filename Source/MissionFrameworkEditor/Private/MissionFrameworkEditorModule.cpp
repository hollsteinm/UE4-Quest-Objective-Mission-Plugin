// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "MissionFrameworkEditorPrivatePCH.h"

#include "Editor/DetailCustomizations/Private/DetailCustomizationsPrivatePCH.h"
#include "Editor/PropertyEditor/Public/PropertyEditorDelegates.h"

#include "Developer/AssetTools/Public/AssetToolsModule.h"

#include "AssetRegistryModule.h"
#include "PackageTools.h"
#include "Mission.h"
#include "MissionData.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "MissionFrameworkEditorStyle.h"
#include "MissionFrameworkEditorCommands.h"
#include "MissionFrameworkEditorToolbar.h"
#include "ObjectEditorUtils.h"
#include "MissionGrantComponent.h"
#include "LevelEditor.h"

#include "Customizations/MissionBindingObservableDetailCustomization.h"
#include "Customizations/MissionDynamicPropertyPathPropertyDetails.h"
#include "Customizations/MissionComponentBindingObserverPropertyDetails.h"
#include "Customizations/MissionComponentNotificationPropertyDetails.h"
#include "Customizations/MissionDataDetailCustomization.h"

static const FName MissionFrameworkEditorTabName("MissionFrameworkEditorModule");

static const FName PropertyEditor("PropertyEditor");
static const FName LevelEditor("LevelEditor");
static const FName Settings("Settings");

#define LOCTEXT_NAMESPACE "FMissionFrameworkEditorModule"

void FMissionFrameworkEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FMissionFrameworkEditorStyle::Initialize();
	FMissionFrameworkEditorStyle::ReloadTextures();
		
	BindCommands();
	HookIntoLevelEditor();
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(MissionFrameworkEditorTabName, FOnSpawnTab::CreateRaw(this, &FMissionFrameworkEditorModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FMissionFrameworkEditorTabTitle", "Missions"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
	
	RegisterCustomLayouts();
}

void FMissionFrameworkEditorModule::ShutdownModule()
{
	if (FModuleManager::Get().IsModuleLoaded(LevelEditor))
	{
		FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>(LevelEditor);
		LevelEditorModule.OnMapChanged().RemoveAll(this);
	}

	FMissionFrameworkEditorStyle::Shutdown();
	FMissionFrameworkEditorCommands::Unregister();
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(MissionFrameworkEditorTabName);

	UnregisterCustomLayouts();
}

void FMissionFrameworkEditorModule::BindCommands()
{
	FMissionFrameworkEditorCommands::Register();

	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FMissionFrameworkEditorCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FMissionFrameworkEditorModule::PluginButtonClicked),
		FCanExecuteAction());

	PluginCommands->MapAction(
		FMissionFrameworkEditorCommands::Get().ExpandPluginToolbar,
		FExecuteAction::CreateRaw(this, &FMissionFrameworkEditorModule::PluginButtonClicked),
		FCanExecuteAction());

	PluginCommands->MapAction(
		FMissionFrameworkEditorCommands::Get().AddMission,
		FExecuteAction::CreateRaw(this, &FMissionFrameworkEditorModule::OnAddMissionClicked),
		FCanExecuteAction());
}

void FMissionFrameworkEditorModule::HookIntoLevelEditor()
{
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>(LevelEditor);

	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender);
		MenuExtender->AddMenuExtension(LevelEditor, EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FMissionFrameworkEditorModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}

	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension(Settings, EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FMissionFrameworkEditorModule::AddToolbarExtension));

		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}

	LevelEditorModule.OnMapChanged().AddRaw(this, &FMissionFrameworkEditorModule::SetActiveWorld);
}

void FMissionFrameworkEditorModule::SetActiveWorld(UWorld* NewWorld, EMapChangeType Reason)
{
	if (NewWorld != MyWorld)
	{
		switch (Reason)
		{
		case EMapChangeType::LoadMap:
		case EMapChangeType::NewMap:
			MyWorld = NewWorld;
			break;
		default:
			break;
		}
	}
}


TSharedRef<SDockTab> FMissionFrameworkEditorModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("MissionEditorDefaultText", "You have no missions."))
			]
		];
}

static AActor* CreateMissionActor(UWorld* MyWorld)
{
	if (MyWorld)
	{
		AActor* NewActor = MyWorld->SpawnActor<AActor>();
		if (NewActor)
		{
			UClass* ComponentClass = UMissionGrantComponent::StaticClass();
			NewActor->Modify();

			// Create an appropriate name for the new component
			FName NewComponentName = *FComponentEditorUtils::GenerateValidVariableName(ComponentClass, NewActor);

			// Construct the new component and attach as needed
			UMissionGrantComponent* NewInstanceComponent = NewObject<UMissionGrantComponent>(NewActor, ComponentClass, NewComponentName, RF_Transactional);
			check(NewInstanceComponent);

			// Add to SerializedComponents array so it gets saved
			NewActor->AddInstanceComponent(NewInstanceComponent);
			NewInstanceComponent->OnComponentCreated();
			NewInstanceComponent->RegisterComponent();

			// Rerun construction scripts
			NewActor->RerunConstructionScripts();

			FAssetToolsModule& AssetToolsModule = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools");

			// Create a unique package name and asset name for the frame
			FFormatNamedArguments Args;
			Args.Add(TEXT("MapName"), FText::FromString(MyWorld->GetName()));
			Args.Add(TEXT("ActorName"), FText::FromString(NewActor->GetName()));
			Args.Add(TEXT("ComponentName"), FText::FromString(NewInstanceComponent->GetName()));
			FText FormattedName = FText::Format(NSLOCTEXT("MissionFrameworkEditor", "DefaultPath", "/Game/Missions/{MapName}/MissionData_{ActorName}_{ComponentName}"), Args);
			const FString TentativePackagePath = PackageTools::SanitizePackageName(FormattedName.ToString());
			FString DefaultSuffix;
			FString AssetName;
			FString PackageName;
			AssetToolsModule.Get().CreateUniqueAssetName(TentativePackagePath, DefaultSuffix, /*out*/ PackageName, /*out*/ AssetName);

			// Create a package for the asset
			UObject* OuterForAsset = CreatePackage(nullptr, *PackageName);
			check(OuterForAsset);

			// Create a frame in the package
			UMissionData* NewAsset = NewObject<UMissionData>(OuterForAsset, UMissionData::StaticClass(), *AssetName, RF_Transactional | RF_Standalone);
			check(NewAsset);

			NewAsset->LinkToMission(NewInstanceComponent->GetMissionInstance(nullptr, false));
			FAssetRegistryModule::AssetCreated(NewAsset);
			return NewActor;
		}
		else
		{
			return nullptr;
		}
	}
	else
	{
		return nullptr;
	}
}

void FMissionFrameworkEditorModule::OnAddMissionClicked()
{
	FMissionFrameworkEditorToolbar::OnMissionActorPicked(CreateMissionActor(MyWorld));
}

void FMissionFrameworkEditorModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->InvokeTab(MissionFrameworkEditorTabName);
}

void FMissionFrameworkEditorModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FMissionFrameworkEditorCommands::Get().OpenPluginWindow);
}

void FMissionFrameworkEditorModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddComboButton(
		FUIAction(),
		FOnGetContent::CreateStatic(&FMissionFrameworkEditorToolbar::GenerateMissionMenuContent, PluginCommands.ToSharedRef()),
		LOCTEXT("EditMission_Label", "Mission"),
		LOCTEXT("EditMission_Tooltip", "Displays a list of Mission objects to open in their respective editors"),
		FSlateIcon(FMissionFrameworkEditorStyle::GetStyleSetName(), "MissionFrameworkEditorModule.OpenPluginWindow")
		);
}


void FMissionFrameworkEditorModule::RegisterCustomLayouts()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>(PropertyEditor);
	PropertyModule.RegisterCustomClassLayout("MissionBindingObservable", FOnGetDetailCustomizationInstance::CreateStatic(&FMissionBindingObservableDetailCustomization::MakeInstance));
	PropertyModule.RegisterCustomPropertyTypeLayout("MissionDynamicPropertyPath", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FMissionDynamicPropertyPathPropertyDetails::MakeInstance));
	PropertyModule.RegisterCustomPropertyTypeLayout("MissionComponentBindingObserver", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FMissionComponentBindingObserverPropertyDetails::MakeInstance));
	PropertyModule.RegisterCustomClassLayout("MissionComponentNotification", FOnGetDetailCustomizationInstance::CreateStatic(&FMissionComponentNotificationPropertyDetails::MakeInstance));
	PropertyModule.RegisterCustomClassLayout("MissionData", FOnGetDetailCustomizationInstance::CreateStatic(&FMissionDataDetailCustomization::MakeInstance));
}

void FMissionFrameworkEditorModule::UnregisterCustomLayouts()
{
	if (FModuleManager::Get().IsModuleLoaded(PropertyEditor))
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>(PropertyEditor);
		PropertyModule.UnregisterCustomClassLayout("MissionBindingObservable");
		PropertyModule.UnregisterCustomPropertyTypeLayout("MissionDynamicPropertyPath");
		PropertyModule.UnregisterCustomPropertyTypeLayout("MissionComponentBindingObserver");
		PropertyModule.UnregisterCustomClassLayout("MissionComponentNotification");
		PropertyModule.UnregisterCustomClassLayout("MissionData");
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMissionFrameworkEditorModule, MissionFrameworkEditorModule)