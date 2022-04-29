#include "MissionFrameworkEditorPrivatePCH.h"
#include "MissionFrameworkEditorToolbar.h"
#include "MissionFrameworkEditorCommands.h"
#include "LevelEditor.h"
#include "Editor/SceneOutliner/Public/SceneOutliner.h"
#include "MissionGrantComponent.h"
#include "Mission.h"
#include "MissionData.h"
#include "AssetRegistryModule.h"

#define LOCTEXT_NAMESPACE "MissionFrameworkEditorModule"

TSharedRef< SWidget > FMissionFrameworkEditorToolbar::GenerateMissionMenuContent(TSharedRef<FUICommandList> InCommandList)
{
	const bool bShouldCloseWindowAfterMenuSelection = true;
	FMenuBuilder MenuBuilder(bShouldCloseWindowAfterMenuSelection, InCommandList);

	SceneOutliner::FInitializationOptions InitOptions;
	{
		InitOptions.Mode = ESceneOutlinerMode::ActorPicker;
		InitOptions.bShowHeaderRow = false;
		InitOptions.bShowSearchBox = false;
		InitOptions.bShowCreateNewFolder = false;

		auto ActorFilter = [](const AActor* Actor) {
			return Actor->FindComponentByClass<UMissionGrantComponent>() != nullptr;
		};
		InitOptions.Filters->AddFilterPredicate(SceneOutliner::FActorFilterPredicate::CreateLambda(ActorFilter));
	}

	FSceneOutlinerModule& SceneOutlinerModule = FModuleManager::LoadModuleChecked<FSceneOutlinerModule>("SceneOutliner");
	TSharedRef< SWidget > MiniSceneOutliner =
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		.MaxHeight(400.0f)
		[
			SceneOutlinerModule.CreateSceneOutliner(
				InitOptions,
				FOnActorPicked::CreateStatic(&FMissionFrameworkEditorToolbar::OnMissionActorPicked))
		];

	static const FName DefaultForegroundName("DefaultForeground");

	// Give the scene outliner a border and background
	const FSlateBrush* BackgroundBrush = FEditorStyle::GetBrush("Menu.Background");
	TSharedRef< SBorder > RootBorder =
		SNew(SBorder)
		.Padding(3)
		.BorderImage(BackgroundBrush)
		.ForegroundColor(FEditorStyle::GetSlateColor(DefaultForegroundName))
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(5)
			.HAlign(HAlign_Center)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("SelectMissionActorToEdit", "Select an actor"))
			]

			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(2)
			[
				MiniSceneOutliner
			]
		];

	MenuBuilder.BeginSection("LevelEditorNewMission", LOCTEXT("MissionMenuCombo_NewHeading", "New Mission"));
	{
		MenuBuilder.AddMenuEntry(FMissionFrameworkEditorCommands::Get().AddMission, NAME_None, TAttribute<FText>(), TAttribute<FText>(), FSlateIcon(FEditorStyle::GetStyleSetName(), "ClassIcon.Actor"));
	}
	MenuBuilder.EndSection();

	MenuBuilder.BeginSection("LevelEditorExistingMission", LOCTEXT("MissionMenuCombo_ExistingHeading", "Edit Existing Mission"));
	{
		MenuBuilder.AddWidget(MiniSceneOutliner, FText::GetEmpty(), true);
	}
	MenuBuilder.EndSection();

	return MenuBuilder.MakeWidget();
}

void FMissionFrameworkEditorToolbar::OnMissionActorPicked(AActor* Actor)
{
	FSlateApplication::Get().DismissAllMenus();

	Actor->Modify();
	for (auto Component : Actor->GetComponentsByClass(UMissionGrantComponent::StaticClass()))
	{
		if (UMissionGrantComponent* MissionComponent = Cast<UMissionGrantComponent>(Component))
		{
			MissionComponent->Modify();
			UMission* Asset = MissionComponent->GetMissionInstance(nullptr, false);
			if (Asset != nullptr)
			{
				FGuid Search = Asset->GetUniqueMissionId();
				FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
				TArray<FAssetData> AssetData;
				if (AssetRegistryModule.Get().GetAssetsByClass(UMissionData::StaticClass()->GetFName(), AssetData, true))
				{
					if (FAssetData* Data = AssetData.FindByPredicate([&](auto Item) -> bool
					{
						if (UMissionData* MissionData = Cast<UMissionData>(Item.GetAsset()))
						{
							return MissionData->GetMissionReferenceId() == Search;
						}
						else
						{
							return false;
						}
					}))
					{
						Asset->Modify();
						FAssetEditorManager::Get().OpenEditorForAsset(Data->GetAsset());
					}
				}
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE