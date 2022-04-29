// Fill out your copyright notice in the Description page of Project Settings.

#include "MissionFrameworkPrivatePCH.h"
#include "Mission.h"
#include "MissionComponent.h"
#include "MissionState.h"
#include "States/MissionStateActive.h"
#include "States/MissionStateTransition.h"
#include "MissionTypes.h"
#include "GameFramework/Actor.h"

UMission::UMission(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer),
	UniqueMissionId(FGuid::NewGuid())
{
	Invalid = ObjectInitializer.CreateDefaultSubobject<UMissionState>(this, FBuiltinStates::Invalid);
	Complete = ObjectInitializer.CreateDefaultSubobject<UMissionState>(this, FBuiltinStates::Pass);
	Fail = ObjectInitializer.CreateDefaultSubobject<UMissionState>(this, FBuiltinStates::Fail);
	Active = ObjectInitializer.CreateDefaultSubobject<UMissionStateActive>(this, FBuiltinStates::Start);
	CurrentState = Invalid;
}

void UMission::Activate()
{
	UE_LOG(LogTemp, Log, TEXT("Mission Activated (%s)"), *GetFullName());
	Invalid->Activate();
	Complete->Activate();
	Fail->Activate();
	Active->Activate();
	if (CurrentState && !(CurrentState == Invalid || CurrentState == Complete || CurrentState == Fail || CurrentState == Active))
	{
		CurrentState->Activate();
		if (!CurrentState->OnMissionComponentFinished().IsBoundToObject(this))
		{
			CurrentState->OnMissionComponentFinished().AddUObject(this, &UMission::ReceiveMissionStateMissionComponentFinished);
		}
	}
}

void UMission::Deactivate()
{
	UE_LOG(LogTemp, Log, TEXT("Mission Deactivated (%s)"), *GetFullName());
	Invalid->Deactivate();
	Complete->Deactivate();
	Fail->Deactivate();
	Active->Deactivate();
	if (CurrentState && !(CurrentState == Invalid || CurrentState == Complete || CurrentState == Fail || CurrentState == Active))
	{
		CurrentState->Deactivate();
		if (CurrentState->OnMissionComponentFinished().IsBoundToObject(this))
		{
			CurrentState->OnMissionComponentFinished().RemoveAll(this);
		}
	}
}

void UMission::PostInitProperties()
{
	Super::PostInitProperties();
	if (CurrentState && !CurrentState->OnMissionComponentFinished().IsBoundToObject(this))
	{
		CurrentState->OnMissionComponentFinished().AddUObject(this, &UMission::ReceiveMissionStateMissionComponentFinished);
	}
}

void UMission::BeginDestroy()
{
	Super::BeginDestroy();
	if (CurrentState && CurrentState->OnMissionComponentFinished().IsBoundToObject(this))
	{
		CurrentState->OnMissionComponentFinished().RemoveAll(this);
	}
}

FName UMission::GetCurrentStateName() const
{
	if (CurrentState)
	{
		return CurrentState->GetStateName();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("CurrentState was nullptr"));
		return FBuiltinStates::Invalid;
	}
}

UWorld* UMission::GetWorld() const
{
	UWorld* World = GetOuter()->GetWorld();
	if (!World && ActorContext)
	{
		World = ActorContext->GetWorld();
	}
	return World;
}

void UMission::TickMission(float DeltaTime)
{
	if (CurrentState)
	{
		CurrentState->TickState(DeltaTime);
	}
}

bool UMission::GoToState(UMissionState* NewState, bool bForceUpdate)
{
	if (NewState)
	{
		if (bForceUpdate || NewState != CurrentState)
		{
			UE_LOG(LogTemp, Log, TEXT("Transitioning States: %s -> %s"), *CurrentState->GetStateName().ToString(), *NewState->GetStateName().ToString());
			OnPreStateTransition.Broadcast(this, CurrentState->GetStateName(), NewState->GetStateName());
			UMissionState* PastState = CurrentState;
			UE_LOG(LogTemp, Log, TEXT("Exiting State: %s"), *CurrentState->GetStateName().ToString());
			CurrentState->Exit(NewState->GetStateName());
			if (PastState)
			{
				CurrentState->OnMissionComponentFinished().RemoveAll(this);
			}
			if (PastState == CurrentState)
			{
				CurrentState = NewState;
				if (CurrentState)
				{
					CurrentState->OnMissionComponentFinished().AddUObject(this, &UMission::ReceiveMissionStateMissionComponentFinished);
				}
				UE_LOG(LogTemp, Log, TEXT("Entering State: %s"), *CurrentState->GetStateName().ToString());
				CurrentState->Enter(PastState->GetStateName());
				OnPostStateTransition.Broadcast(this, CurrentState->GetStateName(), PastState->GetStateName());
			}
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NewState in GoToState was nullptr, consider using Invalid state (Newed up UMissionState) if this was intentional."));
		return false;
	}
}

bool UMission::GoToCompletedState()
{
	return GoToState(Complete);
}

bool UMission::GoToFailedState()
{
	return GoToState(Fail);
}

bool UMission::GoToActiveState()
{
	return GoToState(Active);
}

class AActor* UMission::GetActingActor() const
{
	return ActorContext;
}

void UMission::SetOwningActor(AActor* Actor)
{
	ActorContext = Actor;
}

void UMission::RemoveOwningActor()
{
	ActorContext = nullptr;
}

bool UMission::IsActingObjectActor() const
{
	return GetActingActor() != nullptr;
}

class AGameMode* UMission::GetActingGameMode() const
{
	return GetWorld()->GetAuthGameMode();
}

bool UMission::DoesActingActorHaveController() const
{
	return GetActingController() != nullptr;
}

class AController* UMission::GetActingController() const
{
	if (ActorContext)
	{
		return ActorContext->GetInstigatorController();
	}
	else
	{
		return nullptr;
	}
}

FName UMission::GetActiveStateName() const
{
	if (Active)
	{
		return Active->GetStateName();
	}
	else
	{
		return FBuiltinStates::Invalid;
	}
}

FName UMission::GetFailedStateName() const
{
	if (Fail)
	{
		return Fail->GetStateName();
	}
	else
	{
		return FBuiltinStates::Invalid;
	}
}

FName UMission::GetCompletedStateName() const
{
	if (Complete)
	{
		return Complete->GetStateName();
	}
	else
	{
		return FBuiltinStates::Invalid;
	}
}

void UMission::ReceiveMissionStateMissionComponentFinished(UMissionState* MissionState, UMissionComponent* Component, bool bResult)
{
	OnStateComponentFinished.Broadcast(this, MissionState, Component, bResult);
}

const FGuid UMission::GetUniqueMissionId() const
{
	return UniqueMissionId;
}