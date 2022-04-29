#include "MissionFrameworkPrivatePCH.h"
#include "MissionGrantComponent.h"
#include "Mission.h"

UMissionGrantComponent::UMissionGrantComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Mission = ObjectInitializer.CreateDefaultSubobject<UMission>(this, TEXT("Mission"));
}

UMission* UMissionGrantComponent::GetMissionInstance(UObject* OwnerIfCreateCopy, bool bCreateCopy)
{
	FString Boolean = bCreateCopy ? "True" : "False";
	UE_LOG(LogTemp, Log, TEXT("Granting Mission %s (Copied: %s)"), *Mission->GetFullName(), *Boolean);
	if (bCreateCopy)
	{
		if (OwnerIfCreateCopy)
		{
			UMission* Copy = DuplicateObject<UMission>(Mission, OwnerIfCreateCopy);
			Copy->Activate();
			return Copy;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("bCreateCopy was True, but OwnerIfCreateCopy was nullptr, return nullptr. No Mission created."));
			return nullptr;
		}
	}
	else
	{
		return Mission;
	}
}