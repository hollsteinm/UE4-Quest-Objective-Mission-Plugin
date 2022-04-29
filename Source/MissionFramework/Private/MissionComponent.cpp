#include "MissionFrameworkPrivatePCH.h"
#include "MissionComponent.h"
#include "Mission.h"

UMissionComponent::UMissionComponent(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer),
	bInEnabledState(false),
	bIsActive(true),
	bCanEverTick(false),
	Result(EMissionComponentResult::Pending),
	bResetOnStateEnter(true),
	bResetOnStateExit(true)
{
	DisableInStates.Add(FBuiltinStates::Invalid);
	EnableInStates.Add(FBuiltinStates::Start);
}

void UMissionComponent::Activate()
{
	OnActivate();
}

void UMissionComponent::Deactivate()
{
	OnDeactivate();
}

class UWorld* UMissionComponent::GetWorld() const
{
	UMissionState* OuterMissionState = GetOuterUMissionState();
	check(OuterMissionState);
	return OuterMissionState->GetWorld();
}

bool UMissionComponent::HasResult(EMissionComponentResult TestResult) const
{
	return Result == TestResult;
}

void UMissionComponent::ExitState(FName ExitedStateName, FName NextStateName)
{
	bInEnabledState = !DisableInStates.Contains(NextStateName);
	OnStateExit(ExitedStateName, NextStateName);
	if (bResetOnStateExit)
	{
		Reset();
	}
}

void UMissionComponent::EnterState(FName PreviousStateName, FName EnteredStateState)
{
	bInEnabledState = EnableInStates.Contains(EnteredStateState);
	OnStateEnter(PreviousStateName, EnteredStateState);
	if (bResetOnStateEnter)
	{
		Reset();
	}
}

bool UMissionComponent::CanEverTick() const
{
	return IsEnabled() && bCanEverTick;
}

bool UMissionComponent::IsEnabled() const
{
	return bIsActive && bInEnabledState && Result == EMissionComponentResult::Pending;
}

void UMissionComponent::TickComponent(float DeltaTime)
{
	if (CanEverTick())
	{
		OnTick(DeltaTime);
	}
}

void UMissionComponent::Reset()
{
	OnReset();
}

void UMissionComponent::Finish(bool bSatisfied)
{
	Result = bSatisfied ? EMissionComponentResult::Satisfied : EMissionComponentResult::NotSatisfied;
	OnFinishedEvent.Broadcast(this, bSatisfied);
}

void UMissionComponent::SetActive(bool bNewActive)
{
	bIsActive = bNewActive;
	ActiveChangedEvent.Broadcast(this);
}

void UMissionComponent::SetCanTick(bool bNewCanTick)
{
	bCanEverTick = bNewCanTick;
}

void UMissionComponent::OnStateExit_Implementation(FName ExitedStateName, FName NextStateName)
{
	UE_LOG(LogTemp, Warning, TEXT("OnStateExit, Native, Not Implemented"));
}

void UMissionComponent::OnStateEnter_Implementation(FName EnteredStateState, FName PreviousStateName)
{
	UE_LOG(LogTemp, Warning, TEXT("OnStateEnter, Native, Not Implemented"));
}

void UMissionComponent::OnReset_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("OnReset, Native, Not Implemented"));
}

void UMissionComponent::OnTick_Implementation(float DeltaTime)
{
}

void UMissionComponent::OnActivate_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("MissionComponent: Activated"));
}

void UMissionComponent::OnDeactivate_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("MissionComponent: Deactivated"));
}