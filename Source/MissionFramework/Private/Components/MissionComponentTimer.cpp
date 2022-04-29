#include "MissionFrameworkPrivatePCH.h"
#include "Components/MissionComponentTimer.h"

UMissionComponentTimer::UMissionComponentTimer(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer),
	TimeSpan(FTimespan::FromMinutes(1.0)),
	bSuccessOnExpire(false)
{
	bResetOnStateEnter = true;
	bResetOnStateExit = true;
}

void UMissionComponentTimer::BeginDestroy()
{
	Super::BeginDestroy();
	StopTimer();
}

void UMissionComponentTimer::OnReset_Implementation()
{
	StopTimer();
	if (IsEnabled())
	{
		PlayTimer();
	}
}

void UMissionComponentTimer::OnStateEnter_Implementation(FName EnteredStateName, FName PreviousStateName)
{
	if (!bResetOnStateEnter)
	{
		if (IsEnabled())
		{
			PlayTimer();
		}
	}
}

void UMissionComponentTimer::OnStateExit_Implementation(FName ExitedStateName, FName NextStateName)
{
	if (!bResetOnStateExit)
	{
		if (!IsEnabled())
		{
			StopTimer();
		}
	}
}

void UMissionComponentTimer::ResetTimer()
{
	StopTimer();
	PlayTimer();
}

void UMissionComponentTimer::PauseTimer()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().PauseTimer(TimerHandle);
	}
}

void UMissionComponentTimer::ContinueTimer()
{
	if (UWorld* World = GetWorld())
	{
		if (World->GetTimerManager().IsTimerPaused(TimerHandle))
		{
			World->GetTimerManager().UnPauseTimer(TimerHandle);
		}
	}
}

void UMissionComponentTimer::StopTimer()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(TimerHandle);
	}
}

void UMissionComponentTimer::PlayTimer()
{
	if (UWorld* World = GetWorld())
	{
		if (!World->GetTimerManager().TimerExists(TimerHandle))
		{
			World->GetTimerManager().SetTimer(TimerHandle, this, &UMissionComponentTimer::FinishTimer, TimeSpan.GetTotalSeconds());
		}
	}
}

float UMissionComponentTimer::GetSecondsRemaining() const
{
	if (UWorld* World = GetWorld())
	{
		if (World->GetTimerManager().TimerExists(TimerHandle))
		{
			return World->GetTimerManager().GetTimerRemaining(TimerHandle);
		}
		else
		{
			return 0.0f;
		}
	}
	else
	{
		return 0.0f;
	}
}

float UMissionComponentTimer::GetInitialSeconds() const
{
	return TimeSpan.GetTotalSeconds();
}

void UMissionComponentTimer::FinishTimer()
{
	Finish(bSuccessOnExpire);
}