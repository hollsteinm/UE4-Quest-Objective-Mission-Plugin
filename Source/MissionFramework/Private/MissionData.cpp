// Fill out your copyright notice in the Description page of Project Settings.

#include "MissionFrameworkPrivatePCH.h"
#include "Mission.h"
#include "MissionData.h"

UMissionData::UMissionData(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer),
	MissionReference(nullptr)
{

}

const FGuid UMissionData::GetMissionReferenceId() const
{
	if (MissionReference.IsValid())
	{
		if (UMission* Mission = Cast<UMission>(MissionReference.ResolveObject()))
		{
			return Mission->GetUniqueMissionId();
		}
		else
		{
			return FGuid();
		}
	}
	else
	{
		return FGuid();
	}
}

FMissionCosmetics UMissionData::GetMissionCosmeticData() const
{
	return CosmeticData;
}

class UMission* UMissionData::GetMission() const
{
	if (MissionReference.IsValid())
	{
		return Cast<UMission>(MissionReference.ResolveObject());
	}
	else
	{
		return nullptr;
	}
}

void UMissionData::LinkToMission(UMission* NewMission)
{
	MissionReference = FStringAssetReference(NewMission);
}