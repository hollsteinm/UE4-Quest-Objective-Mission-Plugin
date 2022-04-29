// Fill out your copyright notice in the Description page of Project Settings.

#include "MissionFrameworkPrivatePCH.h"
#include "States/MissionStateActive.h"
#include "Mission.h"
#include "MissionComponent.h"


UMissionStateActive::UMissionStateActive(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
}

FName UMissionStateActive::GetStateName() const
{
	return FBuiltinStates::Start;
}

void UMissionStateActive::OnMissionComponentFinish_Implementation(UMissionComponent* Component, bool bResult)
{
	if (bResult)
	{
		TArray<UMissionComponent*> CurrentComponents = GetComponents();
		int32 Total = CurrentComponents.Num();
		int32 Complete = CurrentComponents.FilterByPredicate([](auto Component) -> bool
		{
			return Component->HasResult(EMissionComponentResult::Satisfied);
		}).Num();
		if (Total <= Complete)
		{
			if (UMission* OuterMission = GetOuterUMission())
			{
				OuterMission->GoToCompletedState();
			}
		}
	}
	else
	{
		if (UMission* OuterMission = GetOuterUMission())
		{
			OuterMission->GoToFailedState();
		}
	}
}