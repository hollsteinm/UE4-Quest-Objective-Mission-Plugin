// Fill out your copyright notice in the Description page of Project Settings.

#include "MissionFrameworkPrivatePCH.h"
#include "Components/MissionComponentChildMission.h"
#include "Mission.h"


UMissionComponentChildMission::UMissionComponentChildMission(const FObjectInitializer& ObjectInitializer) : 
	Super(ObjectInitializer)
{
	SetCanTick(true);
	ChildMission = ObjectInitializer.CreateDefaultSubobject<UMission>(this, TEXT("ChildMission"));
}

void UMissionComponentChildMission::OnTick_Implementation(float DeltaTime)
{
	if (ChildMission)
	{
		ChildMission->TickMission(DeltaTime);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ChildMission was nullptr"));
	}
}

void UMissionComponentChildMission::PostInitProperties()
{
	Super::PostInitProperties();
	if (UMissionState* State = GetOuterUMissionState())
	{
		if (State && ChildMission)
		{
			if (UMission* Mission = State->GetOuterUMission())
			{
				ChildMission->SetOwningActor(Mission->GetActingActor());
			}
		}
	}
}

void UMissionComponentChildMission::OnStateEnter_Implementation(FName EnteredStateState, FName PreviousStateName)
{
	if (IsEnabled() && ChildMission)
	{
		if (ChildMission->GetActiveStateName() == EnteredStateState)
		{
			ChildMission->GoToActiveState();
		}
		else if (ChildMission->GetCompletedStateName() == EnteredStateState)
		{
			ChildMission->GoToCompletedState();
		}
		else if (ChildMission->GetFailedStateName() == EnteredStateState)
		{
			ChildMission->GoToFailedState();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No matching state"))
		}
	}
}

bool UMissionComponentChildMission::GoToCompletedState()
{
	if (ChildMission)
	{
		return ChildMission->GoToCompletedState();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ChildMission was nullptr"));
		return false;
	}
}

bool UMissionComponentChildMission::GoToFailedState()
{
	if (ChildMission)
	{
		return ChildMission->GoToFailedState();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ChildMission was nullptr"));
		return false;
	}
}

bool UMissionComponentChildMission::GoToActiveState()
{
	if (ChildMission)
	{
		return ChildMission->GoToActiveState();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ChildMission was nullptr"));
		return false;
	}
}