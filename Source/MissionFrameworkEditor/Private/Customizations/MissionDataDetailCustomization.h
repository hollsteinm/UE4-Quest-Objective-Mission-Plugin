#pragma once

#include "Editor/PropertyEditor/Public/PropertyEditing.h"

class FMissionDataDetailCustomization : public IDetailCustomization
{
public:
	static TSharedRef<IDetailCustomization> MakeInstance();

	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

private:
	TSharedPtr<class IDetailsView> DetailsView;

};