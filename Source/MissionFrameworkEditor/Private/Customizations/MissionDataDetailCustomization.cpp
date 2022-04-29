#include "MissionFrameworkEditorPrivatePCH.h"
#include "SlateBasics.h"
#include "Editor/PropertyEditor/Public/PropertyCustomizationHelpers.h"
#include "Editor/PropertyEditor/Public/DetailLayoutBuilder.h"
#include "Editor/PropertyEditor/Public/PropertyHandle.h"
#include "Editor/PropertyEditor/Public/IPropertyUtilities.h"
#include "ObjectEditorUtils.h"
#include "MissionTypes.h"
#include "Customizations/MissionDataDetailCustomization.h"
#include "AssetRegistryModule.h"


TSharedRef<IDetailCustomization> FMissionDataDetailCustomization::MakeInstance()
{
	return MakeShareable(new FMissionDataDetailCustomization);
}

void FMissionDataDetailCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	TSharedPtr<IPropertyHandle> MissionProperty = DetailBuilder.GetProperty("MissionReference");
	DetailBuilder.HideProperty(MissionProperty);

	const bool bIsUpdatable = false;
	const bool bIsLockable = false;

	FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	const FDetailsViewArgs DetailsViewArgs(bIsUpdatable, bIsLockable, true, FDetailsViewArgs::ObjectsUseNameArea, false);

	DetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	FString StringReference;
	MissionProperty->GetValueAsFormattedString(StringReference);
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::Get().LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	FAssetData AssetData = AssetRegistryModule.Get().GetAssetByObjectPath(*StringReference);

	DetailsView->SetObject(AssetData.GetAsset());

	DetailBuilder.EditCategory("Mission")
		.AddCustomRow(NSLOCTEXT("MissionFrameworkEditor", "MissionCategory", "Mission"))
		.NameContent()
		[
			MissionProperty->CreatePropertyNameWidget()
		]
		.ValueContent()
		.MinDesiredWidth(1920.0f)
		[
			SNew(SScrollBox)
			+ SScrollBox::Slot()
			.VAlign(VAlign_Fill)
			.HAlign(HAlign_Fill)
			.Padding(10.0f, 5.0f)
			[
				DetailsView.ToSharedRef()
			]
		];
}