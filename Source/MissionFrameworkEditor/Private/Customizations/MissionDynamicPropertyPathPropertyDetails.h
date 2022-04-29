#pragma once

#include "Editor/PropertyEditor/Public/PropertyEditing.h"

class FMissionDynamicPropertyPathPropertyDetails : public IPropertyTypeCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

	/** IPropertyTypeCustomization interface */
	virtual void CustomizeHeader(TSharedRef<class IPropertyHandle> StructPropertyHandle, class FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;
	virtual void CustomizeChildren(TSharedRef<class IPropertyHandle> StructPropertyHandle, class IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;

private:
	TSharedPtr<IPropertyHandle> RootTypePropertyHandle;
	TSharedPtr<IPropertyHandle> ArrayHandle;
	TSharedPtr<IPropertyUtilities> PropertyUtils;

	IPropertyHandle* SelfPropertyHandle;

	TArray<FSimpleDelegate> DeferredActions;

	TSharedRef<SWidget> MakePropertyMenuWidget();

	void GetPropertyPaths(FMenuBuilder& MenuBuilder, class UStruct* Type, TArray<UProperty*> Paths);
	void ApplyBinding(UProperty* Property, TArray<UProperty*> Paths);

	FText GetCurrentValueText() const;
	FText GetCurrentValueTypeText() const;

	//This shit boy, this shit... let's just a recursive deferred chain of operations was necessary for some unholy reason.
	//Deferred Actions
	void EmptyArray(TArray<UProperty*> Paths);
	void AddItemsToArray(TArray<UProperty*> Paths);
	void AddItemToArray(TArray<UProperty*> Paths, uint32 Index);
	void SetItemInArray(TArray<UProperty*> Paths, uint32 Index);

	//Executors
	void OnEmptyArray(TArray<UProperty*> Paths);
	void OnAddItemsToArray(TArray<UProperty*> Paths);
	void OnAddItemToArray(TArray<UProperty*> Paths, uint32 Index);
	void OnSetItemInArray(TArray<UProperty*> Paths, uint32 Index);
};