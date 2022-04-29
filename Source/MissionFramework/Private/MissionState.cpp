// Fill out your copyright notice in the Description page of Project Settings.

#include "MissionFrameworkPrivatePCH.h"
#include "MissionState.h"
#include "MissionComponent.h"
#include "MissionReward.h"

UMissionState::UMissionState(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer),
	bTicks(true),
	bResetOnExit(true),
	bResetOnEnter(true)
{

}

UWorld* UMissionState::GetWorld() const
{
	if (UMission* OuterMission = GetOuterUMission())
	{
		return OuterMission->GetWorld();
	}
	else
	{
		return nullptr;
	}
}

void UMissionState::Activate()
{
	OnActivate();
	for (auto Component : Components)
	{
		if (Component)
		{
			Component->Activate();
		}
	}
	BindToAllComponents();
}

void UMissionState::Deactivate()
{
	OnDeactivate();
	for (auto Component : Components)
	{
		if (Component)
		{
			Component->Deactivate();
		}
	}
	UnBindFromAllComponents();
}

void UMissionState::PostInitProperties()
{
	Super::PostInitProperties();
	BindToAllComponents();
}

void UMissionState::BeginDestroy()
{
	Super::BeginDestroy();
	UnBindFromAllComponents();
}

void UMissionState::BindToAllComponents()
{
	for (auto Component : Components)
	{
		if (Component)
		{
			UnBindFromComponent(Component);
			BindToComponent(Component);
		}
	}
}

void UMissionState::UnBindFromAllComponents()
{
	for (auto Component : Components)
	{
		if (Component)
		{
			UnBindFromComponent(Component);
		}
	}
}

void UMissionState::BindToComponent(UMissionComponent* Component)
{
	if (Component)
	{
		Component->OnFinished().AddUObject(this, &UMissionState::ReceiveMissionComponentFinished);
	}
}

void UMissionState::UnBindFromComponent(UMissionComponent* Component)
{
	if (Component)
	{
		Component->OnFinished().RemoveAll(this);
	}
}

void UMissionState::Enter(FName PreviousStateName)
{
	OnEnter(PreviousStateName);
	for (auto Component : Components)
	{
		Component->EnterState(PreviousStateName, GetStateName());
	}
	if (bResetOnEnter)
	{
		Reset();
	}
}

void UMissionState::Exit(FName NextStateName)
{
	OnExit(NextStateName);
	for (auto Component : Components)
	{
		Component->ExitState(GetStateName(), NextStateName);
	}
	if (bResetOnExit)
	{
		Reset();
	}
	ApplyRewards();
}

void UMissionState::TickState(float DeltaTime)
{
	if (bTicks)
	{
		OnTick(DeltaTime);
		for (auto Component : Components)
		{
			Component->TickComponent(DeltaTime);
		}
	}
}

FName UMissionState::GetStateName() const
{
	return FBuiltinStates::Invalid;
}

 UMissionReward* UMissionState::GetRewardByClass(const TSubclassOf< UMissionReward>& ClassType) const
{
	for (auto Reward : Rewards)
	{
		if (Reward->IsA(*ClassType))
		{
			return Reward;
		}
	}
	return nullptr;
}

TArray< UMissionReward*> UMissionState::GetRewardsByClass(const TSubclassOf< UMissionReward>& ClassType) const
{
	return Rewards.FilterByPredicate([&](auto Reward) -> bool
	{
		return Reward->IsA(*ClassType);
	});
}

TArray<class UMissionReward*> UMissionState::GetRewards() const
{
	return Rewards;
}

UMissionReward* UMissionState::AddNewReward(const TSubclassOf<UMissionReward>& NewRewardClass)
{
	UMissionReward* Reward = NewObject<UMissionReward>(this, *NewRewardClass, NAME_None, RF_Transactional);
	if (Reward)
	{
		AddReward(Reward);
	}
	return Reward;
}


UMissionComponent* UMissionState::AddNewComponent(const TSubclassOf<UMissionComponent>& NewComponentClass)
{
	UMissionComponent* Component = NewObject<UMissionComponent>(this, *NewComponentClass, NAME_None, RF_Transactional);
	if (Component)
	{
		AddComponent(Component);
	}
	return Component;
}

UMissionComponent* UMissionState::GetComponentByClass(const TSubclassOf<UMissionComponent>& ClassType) const
{
	for (auto Component : Components)
	{
		if (Component->IsA(*ClassType))
		{
			return Component;
		}
	}
	return nullptr;
}

TArray<UMissionComponent*> UMissionState::GetComponentsByClass(const TSubclassOf<UMissionComponent>& ClassType) const
{
	return Components.FilterByPredicate([&](auto Component) -> bool
	{
		return Component->IsA(*ClassType);
	});
}

TArray<UMissionComponent*> UMissionState::GetComponents() const
{
	return Components;
}

void UMissionState::AddComponent(UMissionComponent* NewComponent)
{
	Components.AddUnique(NewComponent);
	BindToComponent(NewComponent);
}

void UMissionState::AddReward(UMissionReward* Reward)
{
	Rewards.AddUnique(Reward);
}

void UMissionState::ApplyRewards()
{
	for (auto Reward : Rewards)
	{
		if (Reward)
		{
			Reward->Apply(GetOuterUMission()->GetActingActor());
		}
	}
}

void UMissionState::Reset()
{
	OnReset();
	for (auto Component : Components)
	{
		if (Component)
		{
			Component->Reset();
		}
	}
}

void UMissionState::OnReset_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Not Implemented"));
}

void UMissionState::OnEnter_Implementation(FName PreviousStateName)
{
	UE_LOG(LogTemp, Warning, TEXT("Not Implemented"));
}

void UMissionState::OnExit_Implementation(FName NextStateName)
{
	UE_LOG(LogTemp, Warning, TEXT("Not Implemented"));
}

void UMissionState::ReceiveMissionComponentFinished(class UMissionComponent* Component, bool bResult)
{
	MissionComponentFinishedEvent.Broadcast(this, Component, bResult);
	OnMissionComponentFinish(Component, bResult);
}

void UMissionState::OnMissionComponentFinish_Implementation(UMissionComponent* Component, bool bResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Not Implemented"));
}