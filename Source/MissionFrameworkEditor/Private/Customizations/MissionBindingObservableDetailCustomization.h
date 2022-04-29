#pragma once

#include "Editor/PropertyEditor/Public/PropertyEditing.h"

class FMissionBindingObservableDetailCustomization : public IDetailCustomization
{
public:
	/** Makes a new instance of this detail layout class for a specific detail view requesting it */
	static TSharedRef<IDetailCustomization> MakeInstance();

	/** IDetailCustomization interface */
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

private:
	void OnBindingsChanged(class UObject* EditedObject, TSharedPtr<IPropertyHandle> BindingHandle);
};