// Fill out your copyright notice in the Description page of Project Settings.

#include "MissionFrameworkPrivatePCH.h"
#include "MissionState.h"
#include "Mission.h"
#include "States/MissionStateTransition.h"

UMissionStateTransition::UMissionStateTransition(const FObjectInitializer& ObjectInitializer) : 
	Super(ObjectInitializer),
	bDestroyLeadingMission(true)
{

}

void UMissionStateTransition::OnEnter_Implementation(FName ExtStateName)
{
	if (TransitionMission)
	{
		UMission* TransitionedMission = NewObject<UMission>(GetOuterUMission()->GetOuter(), TransitionMission);
		if (TransitionedMission)
		{
			if (!TransitionedMission->GoToActiveState())
			{
				UE_LOG(LogTemp, Warning, TEXT("%s did not GoToActiveState()"), *TransitionedMission->GetName());
			}
			else
			{
				OnTransition.Broadcast(TransitionedMission);
				if (bDestroyLeadingMission)
				{
					GetOuterUMission()->ConditionalBeginDestroy();
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("TransitionedMission was nullptr after creation"));
		}
	}
}

FName UMissionStateTransition::GetStateName() const
{
	return FName(TEXT("Transition"));
}