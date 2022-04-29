// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "MissionFrameworkEditorPrivatePCH.h"
#include "Customizations/MissionBindingObservableDetailCustomization.h"
#include "SlateBasics.h"
#include "Editor/PropertyEditor/Public/PropertyCustomizationHelpers.h"
#include "Editor/PropertyEditor/Public/DetailLayoutBuilder.h"
#include "Editor/PropertyEditor/Public/PropertyHandle.h"
#include "Editor/PropertyEditor/Public/IPropertyUtilities.h"
#include "ObjectEditorUtils.h"
#include "GameFramework/Actor.h"
#include "ActorComponents/MissionBindingObservable.h"

TSharedRef<IDetailCustomization> FMissionBindingObservableDetailCustomization::MakeInstance()
{
	return MakeShareable(new FMissionBindingObservableDetailCustomization);
}

void FMissionBindingObservableDetailCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	TArray<TWeakObjectPtr<UObject>> CustomizedObjects;
	DetailBuilder.GetObjectsBeingCustomized(CustomizedObjects);
	if(CustomizedObjects.Num() == 1)
	{
		TSharedPtr<IPropertyHandle> BindingHandle = DetailBuilder.GetProperty("Bindings");
		if (BindingHandle.IsValid())
		{
			OnBindingsChanged(CustomizedObjects[0].Get(), BindingHandle);
			TSharedPtr<IPropertyHandleArray> AsArray = BindingHandle->AsArray();
			if (AsArray.IsValid())
			{
				FSimpleDelegate OnNumChanged = FSimpleDelegate::CreateSP(this, &FMissionBindingObservableDetailCustomization::OnBindingsChanged, CustomizedObjects[0].Get(), BindingHandle);
				AsArray->SetOnNumElementsChanged(OnNumChanged);
			}
		}
	}
}

void FMissionBindingObservableDetailCustomization::OnBindingsChanged(UObject* EditedObject, TSharedPtr<IPropertyHandle> BindingHandle)
{
	if (EditedObject != nullptr && BindingHandle.IsValid())
	{
		TSharedPtr<IPropertyHandleArray> AsArray = BindingHandle->AsArray();
		if (AsArray.IsValid())
		{
			uint32 NumElements = 0;
			if (AsArray->GetNumElements(NumElements) == FPropertyAccess::Success)
			{
				for (uint32 Index = 0; Index < NumElements; ++Index)
				{
					TSharedRef<IPropertyHandle> Element = AsArray->GetElement(Index);
					TSharedPtr<IPropertyHandle> ClassHandle = Element->GetChildHandle(GET_MEMBER_NAME_CHECKED(FMissionDynamicPropertyPath, TargetClass));
					if (ClassHandle.IsValid())
					{
						UClass* TargetClass = nullptr;
						UObject* Outer = EditedObject->GetOuter();
						if (!Outer->IsA<UClass>())
						{
							TargetClass = Outer->GetClass();
						}
						else
						{
							TargetClass = Cast<UClass>(Outer);
							if (TargetClass->IsChildOf(UBlueprintGeneratedClass::StaticClass()))
							{
								TargetClass = Cast<UBlueprintGeneratedClass>(TargetClass)->GetArchetypeForCDO()->GetClass();
							}
						}

						if (TargetClass)
						{
							ClassHandle->SetValue(TargetClass, EPropertyValueSetFlags::DefaultFlags);
						}
					}
				}
			}
		}
	}
}