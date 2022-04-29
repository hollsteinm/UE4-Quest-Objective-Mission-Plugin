#include "MissionFrameworkEditorPrivatePCH.h"
#include "SlateBasics.h"
#include "Editor/PropertyEditor/Public/PropertyCustomizationHelpers.h"
#include "Editor/PropertyEditor/Public/DetailLayoutBuilder.h"
#include "Editor/PropertyEditor/Public/PropertyHandle.h"
#include "Editor/PropertyEditor/Public/IPropertyUtilities.h"
#include "ObjectEditorUtils.h"
#include "Customizations/MissionDynamicPropertyPathPropertyDetails.h"
#include "MissionTypes.h"

TSharedRef<IPropertyTypeCustomization> FMissionDynamicPropertyPathPropertyDetails::MakeInstance()
{
	return MakeShareable(new FMissionDynamicPropertyPathPropertyDetails());
}

void FMissionDynamicPropertyPathPropertyDetails::CustomizeHeader(TSharedRef<class IPropertyHandle> StructPropertyHandle, class FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	SelfPropertyHandle = &*StructPropertyHandle;
	PropertyUtils = StructCustomizationUtils.GetPropertyUtilities();
	RootTypePropertyHandle = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FMissionDynamicPropertyPath, TargetClass));
	ArrayHandle = SelfPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FMissionDynamicPropertyPath, Segments));

	TSharedPtr<SVerticalBox> VerticalBox;

	HeaderRow
		.NameContent()
		[
			StructPropertyHandle->CreatePropertyNameWidget()
		]
		.ValueContent()
		.MinDesiredWidth(250.0f)
		[
			SAssignNew(VerticalBox, SVerticalBox)
			+ SVerticalBox::Slot()
			.VAlign(VAlign_Fill)
			.HAlign(HAlign_Fill)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.VAlign(VAlign_Fill)
				.HAlign(HAlign_Fill)
				.FillWidth(1.5f)
				[
					SNew(SComboButton)
					.OnGetMenuContent(this, &FMissionDynamicPropertyPathPropertyDetails::MakePropertyMenuWidget)
					.ContentPadding(1)
					.ButtonContent()
					[
						SNew(STextBlock)
						.Text(this, &FMissionDynamicPropertyPathPropertyDetails::GetCurrentValueText)
						.ToolTipText(this, &FMissionDynamicPropertyPathPropertyDetails::GetCurrentValueText)
						.Font(IDetailLayoutBuilder::GetDetailFont())
					]
				]
				+ SHorizontalBox::Slot()
				.VAlign(VAlign_Fill)
				.HAlign(HAlign_Fill)
				.Padding(FMargin(0, 1, 0, 1))
				.FillWidth(0.5f)
				[
					SNew(SBorder)
					.Padding(FMargin(0.0f, 2.0f))
					.VAlign(VAlign_Center)
					[
						SNew(STextBlock)
						.Justification(ETextJustify::Center)
						.Text(this, &FMissionDynamicPropertyPathPropertyDetails::GetCurrentValueTypeText)
						.ToolTipText(this, &FMissionDynamicPropertyPathPropertyDetails::GetCurrentValueTypeText)
						.Font(IDetailLayoutBuilder::GetDetailFont())
					]
				]
			]
		];

		if (SelfPropertyHandle->GetBoolMetaData(TEXT("ClassPickerVisible")))
		{
			VerticalBox->AddSlot()
				.VAlign(VAlign_Fill)
				.HAlign(HAlign_Fill)
				[
					RootTypePropertyHandle->CreatePropertyValueWidget()
				];
		}
}

void FMissionDynamicPropertyPathPropertyDetails::CustomizeChildren(TSharedRef<class IPropertyHandle> StructPropertyHandle, class IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{

}

static FString MakeFriendly(const FName& Name)
{
	//TODO: Implement
	return Name.ToString();

}

FText FMissionDynamicPropertyPathPropertyDetails::GetCurrentValueTypeText() const
{
	TSharedPtr<IPropertyHandleArray> SegmentArray = ArrayHandle->AsArray();
	if (SegmentArray.IsValid())
	{
		uint32 NumElements = 0;
		if (SegmentArray->GetNumElements(NumElements) == FPropertyAccess::Success)
		{
			if (NumElements > 0)
			{
				TSharedRef<IPropertyHandle> SegmentHandle = SegmentArray->GetElement(NumElements - 1);
				TSharedPtr<IPropertyHandle> SegmentChild = SegmentHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FMissionPropertyPathSegment, CPPTypeName));
				if (SegmentChild.IsValid())
				{
					FName Value;
					FFormatNamedArguments Arguments;

					SegmentChild->GetValue(Value);
					Arguments.Add(TEXT("CPPType"), FText::FromName(Value));
					return  FText::Format(NSLOCTEXT("MissionFrameworkEditorModule", "CPPTypeNamePropertyDetail", "{CPPType}"), Arguments);
				}
			}
		}
	}
	return FText::FromName(FName(NAME_None));
}

FText FMissionDynamicPropertyPathPropertyDetails::GetCurrentValueText() const
{
	FText Result = NSLOCTEXT("MissionFramework", "PathSelect", "Click to select Property Path");

	TSharedPtr<IPropertyHandleArray> SegmentArray = ArrayHandle->AsArray();
	if (SegmentArray.IsValid())
	{
		uint32 NumElements = 0;
		if (SegmentArray->GetNumElements(NumElements) == FPropertyAccess::Success)
		{
			if (NumElements > 0)
			{
				FString PathText;
				for (uint32 Idx = 0; Idx < NumElements; Idx++)
				{
					TSharedRef<IPropertyHandle> SegmentHandle = SegmentArray->GetElement(Idx);
					TSharedPtr<IPropertyHandle> SegmentChild = SegmentHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FMissionPropertyPathSegment, Name));
					if (SegmentChild.IsValid())
					{
						FName Value;
						SegmentChild->GetValue(Value);
						PathText += MakeFriendly(Value) + ".";
					}
				}
				PathText.RemoveFromEnd(".");
				Result = FText::FromString(PathText);
			}
		}
	}
	return Result;
}

TSharedRef<SWidget> FMissionDynamicPropertyPathPropertyDetails::MakePropertyMenuWidget()
{
	FMenuBuilder MenuBuilder(true, nullptr);

	if (RootTypePropertyHandle.IsValid())
	{
		UObject* TypeObject = nullptr;
		if (RootTypePropertyHandle->GetValue(TypeObject) == FPropertyAccess::Success)
		{
			if (UStruct* RootType = Cast<UStruct>(TypeObject))
			{
				GetPropertyPaths(MenuBuilder, RootType, TArray<UProperty*>());
			}
		}
	}

	return MenuBuilder.MakeWidget();
}

//Filter Rules
bool IsAcceptableProperty(UProperty* Property)
{
	const TArray<UClass*> BlacklistedProperties =
	{
		UArrayProperty::StaticClass(),
		UDelegateProperty::StaticClass(),
		UInterfaceProperty::StaticClass(),
		UMapProperty::StaticClass(),
		UMulticastDelegateProperty::StaticClass(),
		USetProperty::StaticClass()
	};

	return BlacklistedProperties.FilterByPredicate([&](UClass* BlackProperty) -> bool
	{
		return Property->IsA(BlackProperty);
	}).Num() <= 0;
}

static bool IsComplexProperty(UProperty* Property)
{
	return (Property->IsA<UStructProperty>() || Property->IsA<UObjectProperty>() || Property->IsA<UWeakObjectProperty>()) &&
		!(Property->IsA<UStrProperty>() || 
			Property->IsA<UTextProperty>() || 
			Property->GetCPPType() == "FVector" ||
			Property->GetCPPType() == "FRotator" ||
			Property->GetCPPType() == "FTransform" ||
			Property->IsA<UNameProperty>());
}

static bool IsPropertyInGameBuild(UProperty* Property)
{
	return !Property->HasAnyPropertyFlags(CPF_AdvancedDisplay | CPF_EditorOnly);
}

static bool IsPropertyVisible(UProperty* Property)
{
	return Property->HasAllPropertyFlags(CPF_BlueprintVisible);
}

static bool IsValidPrimitiveProperty(UProperty* Property)
{
	return IsAcceptableProperty(Property) &&
		!IsComplexProperty(Property) &&
		IsPropertyInGameBuild(Property) &&
		IsPropertyVisible(Property);
}

static bool IsValidComplexProperty(UProperty* Property)
{
	return IsAcceptableProperty(Property) &&
		IsComplexProperty(Property) &&
		IsPropertyInGameBuild(Property) &&
		IsPropertyVisible(Property);
}

static bool IsSupportedType(UStruct* Struct)
{
	const TArray<UClass*> BlacklistedClasses =
	{
		AActor::StaticClass(),
		UActorComponent::StaticClass()
	};

	return !Struct->IsA<UClass>() || BlacklistedClasses.FilterByPredicate([&](UClass* BlackClass) -> bool
	{
		if (UClass* AsClass = Cast<UClass>(Struct))
		{
			return AsClass->IsChildOf(BlackClass);
		}
		else
		{
			return false;
		}
	}).Num() <= 0;
}

static UStruct* ResolveStruct(UProperty* Property)
{
	UObjectProperty* ObjectProperty = Cast<UObjectProperty>(Property);
	UWeakObjectProperty* WeakObjectProperty = Cast<UWeakObjectProperty>(Property);
	UStructProperty* StructProperty = Cast<UStructProperty>(Property);

	UStruct* Struct = nullptr;

	if (ObjectProperty)
	{
		Struct = ObjectProperty->PropertyClass;
	}
	else if (WeakObjectProperty)
	{
		Struct = WeakObjectProperty->PropertyClass;
	}
	else if (StructProperty)
	{
		Struct = StructProperty->Struct;
	}

	return Struct;
}
//End Filter Rules

void FMissionDynamicPropertyPathPropertyDetails::GetPropertyPaths(FMenuBuilder& MenuBuilder, UStruct* Type, TArray<UProperty*> Paths)
{
	MenuBuilder.BeginSection("Properties", NSLOCTEXT("MissionFrameworkEditorModule", "PropertiesSection", "Properties"));
	{
		for (TFieldIterator<UProperty> Iter(Type); Iter; ++Iter)
		{
			if (UProperty* Property = *Iter)
			{
				if (IsValidPrimitiveProperty(Property))
				{
					MenuBuilder.AddMenuEntry(
						Property->GetDisplayNameText(),
						Property->GetToolTipText(),
						FSlateIcon(FEditorStyle::GetStyleSetName(), TEXT("Kismet.Tabs.Variables")),
						FUIAction(FExecuteAction::CreateSP(this, &FMissionDynamicPropertyPathPropertyDetails::ApplyBinding, Property, Paths))
						);
				}
			}
		}
	}
	MenuBuilder.EndSection();

	MenuBuilder.BeginSection("SubobjectProperties", NSLOCTEXT("MissionFrameworkEditorModule", "SubobjectPropertiesSection", "Subobject Properties"));
	{
		for (TFieldIterator<UProperty> Iter(Type); Iter; ++Iter)
		{
			if (UProperty* Property = *Iter)
			{
				if (IsValidComplexProperty(Property))
				{
					if (UStruct* Struct = ResolveStruct(Property))
					{
						if (IsSupportedType(Struct))
						{
							TArray<UProperty*> NewObjectPath( Paths );
							NewObjectPath.Add(Property);
							MenuBuilder.AddSubMenu(
								Property->GetDisplayNameText(),
								Property->GetToolTipText(),
								FNewMenuDelegate::CreateSP(this, &FMissionDynamicPropertyPathPropertyDetails::GetPropertyPaths, Struct, NewObjectPath)
								);
						}
					}
				}
			}
		}
	}
	MenuBuilder.EndSection();
}

void FMissionDynamicPropertyPathPropertyDetails::ApplyBinding(UProperty* Property, TArray<UProperty*> Paths)
{
	if (Property != nullptr)
	{
		Paths.Add(Property);
	}
	EmptyArray(Paths);
}

void FMissionDynamicPropertyPathPropertyDetails::EmptyArray(TArray<UProperty*> Paths)
{
	PropertyUtils->EnqueueDeferredAction(FSimpleDelegate::CreateSP(this, &FMissionDynamicPropertyPathPropertyDetails::OnEmptyArray, Paths));
}

void FMissionDynamicPropertyPathPropertyDetails::AddItemToArray(TArray<UProperty*> Paths, uint32 Index)
{
	PropertyUtils->EnqueueDeferredAction(FSimpleDelegate::CreateSP(this, &FMissionDynamicPropertyPathPropertyDetails::OnAddItemToArray, Paths, Index));
}

void FMissionDynamicPropertyPathPropertyDetails::SetItemInArray(TArray<UProperty*> Paths, uint32 Index)
{
	OnSetItemInArray(Paths, Index);
}

void FMissionDynamicPropertyPathPropertyDetails::AddItemsToArray(TArray<UProperty*> Paths)
{
	PropertyUtils->EnqueueDeferredAction(FSimpleDelegate::CreateSP(this, &FMissionDynamicPropertyPathPropertyDetails::OnAddItemsToArray, Paths));
}

void FMissionDynamicPropertyPathPropertyDetails::OnEmptyArray(TArray<UProperty*> Paths)
{
	TSharedPtr<IPropertyHandleArray> AsArray = ArrayHandle->AsArray();
	check(AsArray.IsValid());

	if (AsArray->EmptyArray() == FPropertyAccess::Success && Paths.Num() > 0)
	{
		AddItemsToArray(Paths);
	}
	else
	{
		PropertyUtils->RequestRefresh();
	}
}

void FMissionDynamicPropertyPathPropertyDetails::OnAddItemsToArray(TArray<UProperty*> Paths)
{
	AddItemToArray(Paths, 0);
}

void FMissionDynamicPropertyPathPropertyDetails::OnAddItemToArray(TArray<UProperty*> Paths, uint32 Index)
{
	TSharedPtr<IPropertyHandleArray> AsArray = ArrayHandle->AsArray();
	check(AsArray.IsValid());
	
	if (Paths.Num() > 0)
	{
		FSimpleDelegate Delegate = FSimpleDelegate::CreateSP(this, &FMissionDynamicPropertyPathPropertyDetails::SetItemInArray, Paths, Index);
		AsArray->SetOnNumElementsChanged(Delegate);
		AsArray->AddItem();
	}
	else
	{
		PropertyUtils->RequestRefresh();
	}
}

void FMissionDynamicPropertyPathPropertyDetails::OnSetItemInArray(TArray<UProperty*> Paths, uint32 Index)
{
	TSharedPtr<IPropertyHandleArray> AsArray = ArrayHandle->AsArray();

	UProperty* Property = Paths[0];
	check(AsArray.IsValid());

	uint32 CurrentNum = 0;
	AsArray->GetNumElements(CurrentNum);

	if (Index < CurrentNum)
	{
		TSharedPtr<IPropertyHandle> Element = AsArray->GetElement(Index);
		check(Element.IsValid());

		TSharedPtr<IPropertyHandle> ElementChildName = Element->GetChildHandle(GET_MEMBER_NAME_CHECKED(FMissionPropertyPathSegment, Name));
		check(ElementChildName.IsValid());

		TSharedPtr<IPropertyHandle> ElementChildType = Element->GetChildHandle(GET_MEMBER_NAME_CHECKED(FMissionPropertyPathSegment, CPPTypeName));
		check(ElementChildType.IsValid());

		ElementChildName->SetValue(Property->GetName(), EPropertyValueSetFlags::DefaultFlags);
		ElementChildType->SetValue(Property->GetCPPType(), EPropertyValueSetFlags::DefaultFlags);

		Paths.RemoveAt(0);
		AddItemToArray(Paths, ++Index);
	}
}