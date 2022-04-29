#include "MissionFrameworkEditorPrivatePCH.h"
#include "SlateBasics.h"
#include "Editor/PropertyEditor/Public/PropertyCustomizationHelpers.h"
#include "Editor/PropertyEditor/Public/DetailLayoutBuilder.h"
#include "Editor/PropertyEditor/Public/PropertyHandle.h"
#include "Editor/PropertyEditor/Public/IPropertyUtilities.h"
#include "ObjectEditorUtils.h"
#include "MissionTypes.h"
#include "Customizations/MissionComponentBindingObserverPropertyDetails.h"
#include "ActorComponents/MissionBindingObservable.h"
#include "GameFramework/Actor.h"


TSharedRef<IPropertyTypeCustomization> FMissionComponentBindingObserverPropertyDetails::MakeInstance()
{
	return MakeShareable(new FMissionComponentBindingObserverPropertyDetails);
}

void FMissionComponentBindingObserverPropertyDetails::CustomizeHeader(TSharedRef<class IPropertyHandle> StructPropertyHandle, class FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	HeaderRow.NameContent()
		[
			StructPropertyHandle->CreatePropertyNameWidget()
		];
}

void FMissionComponentBindingObserverPropertyDetails::CustomizeChildren(TSharedRef<class IPropertyHandle> StructPropertyHandle, class IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	uint32 NumChildren = 0;
	StructPropertyHandle->GetNumChildren(NumChildren);
	for (uint32 Index = 0; Index < NumChildren; ++Index)
	{
		TSharedPtr<IPropertyHandle> Child = StructPropertyHandle->GetChildHandle(Index);
		if (Child->GetProperty()->GetName() != TEXT("BoundActor"))
		{
			StructBuilder.AddChildProperty(Child.ToSharedRef());
		}
		else
		{
			StructBuilder.AddChildProperty(Child.ToSharedRef())
				.CustomWidget()
				.NameContent()
				[
					Child->CreatePropertyNameWidget()
				]
				.ValueContent()
				[
					PropertyCustomizationHelpers::MakeActorPickerAnchorButton(
						FOnGetActorFilters::CreateLambda([](auto Filter)
						{
							Filter->AddFilterPredicate(SceneOutliner::FActorFilterPredicate::CreateLambda([](const AActor* Actor) -> bool
							{
								return Actor->FindComponentByClass<UMissionBindingObservable>() != nullptr;
							}));
						}),
						FOnActorSelected::CreateSP(this, &FMissionComponentBindingObserverPropertyDetails::SetBoundActor, Child))
				];
		}
	}
}

void FMissionComponentBindingObserverPropertyDetails::SetBoundActor(AActor* PickedActor, TSharedPtr<IPropertyHandle> Handle)
{
	Handle->SetValue(PickedActor, EPropertyValueSetFlags::DefaultFlags);
}