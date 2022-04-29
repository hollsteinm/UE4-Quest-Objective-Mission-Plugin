#include "MissionFrameworkEditorPrivatePCH.h"
#include "SlateBasics.h"
#include "Editor/PropertyEditor/Public/PropertyCustomizationHelpers.h"
#include "Editor/PropertyEditor/Public/DetailLayoutBuilder.h"
#include "Editor/PropertyEditor/Public/PropertyHandle.h"
#include "Editor/PropertyEditor/Public/IPropertyUtilities.h"
#include "ObjectEditorUtils.h"
#include "MissionTypes.h"
#include "Customizations/MissionComponentNotificationPropertyDetails.h"
#include "ActorComponents/MissionNotifierComponent.h"
#include "GameFramework/Actor.h"


TSharedRef<IDetailCustomization> FMissionComponentNotificationPropertyDetails::MakeInstance()
{
	return MakeShareable(new FMissionComponentNotificationPropertyDetails);
}

void FMissionComponentNotificationPropertyDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
		TSharedPtr<IPropertyHandle> BoundActor = DetailBuilder.GetProperty("BoundActor");
		BoundActor->MarkHiddenByCustomization();
		DetailBuilder.EditCategory("Component")
			.AddCustomRow(BoundActor->GetPropertyDisplayName())
			.NameContent()
			[
				BoundActor->CreatePropertyNameWidget()
			]
			.ValueContent()
			[
				PropertyCustomizationHelpers::MakeActorPickerAnchorButton(
					FOnGetActorFilters::CreateLambda([](auto Filter)
					{
						Filter->AddFilterPredicate(SceneOutliner::FActorFilterPredicate::CreateLambda([](const AActor* Actor) -> bool
						{
							return Actor->FindComponentByClass<UMissionNotifierComponent>() != nullptr;
						}));
					}),
					FOnActorSelected::CreateSP(this, &FMissionComponentNotificationPropertyDetails::SetBoundActor, BoundActor))
			];
}

void FMissionComponentNotificationPropertyDetails::SetBoundActor(AActor* PickedActor, TSharedPtr<IPropertyHandle> Handle)
{
	Handle->SetValue(PickedActor, EPropertyValueSetFlags::DefaultFlags);
}