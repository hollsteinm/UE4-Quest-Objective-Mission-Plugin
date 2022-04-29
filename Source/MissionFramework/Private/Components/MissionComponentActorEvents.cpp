// Fill out your copyright notice in the Description page of Project Settings.

#include "MissionFrameworkPrivatePCH.h"
#include "Components/MissionComponentActorEvents.h"


void UMissionComponentActorEvents::Bind()
{
	BoundActor->OnActorBeginOverlap.AddDynamic(this, &UMissionComponentActorEvents::OnActorBeginOverlap);
	BoundActor->OnActorEndOverlap.AddDynamic(this, &UMissionComponentActorEvents::OnActorEndOverlap);
	BoundActor->OnActorHit.AddDynamic(this, &UMissionComponentActorEvents::OnActorHit);
	BoundActor->OnBeginCursorOver.AddDynamic(this, &UMissionComponentActorEvents::OnActorBeginCursorOver);
	BoundActor->OnClicked.AddDynamic(this, &UMissionComponentActorEvents::OnActorClicked);
	BoundActor->OnDestroyed.AddDynamic(this, &UMissionComponentActorEvents::OnActorDestroyed);
	BoundActor->OnEndCursorOver.AddDynamic(this, &UMissionComponentActorEvents::OnActorEndCursorOver);
	BoundActor->OnEndPlay.AddDynamic(this, &UMissionComponentActorEvents::OnActorEndPlay);
	BoundActor->OnInputTouchBegin.AddDynamic(this, &UMissionComponentActorEvents::OnActorInputTouchBegin);
	BoundActor->OnInputTouchEnd.AddDynamic(this, &UMissionComponentActorEvents::OnActorInputTouchEnd);
	BoundActor->OnInputTouchEnter.AddDynamic(this, &UMissionComponentActorEvents::OnActorInputTouchEnter);
	BoundActor->OnInputTouchLeave.AddDynamic(this, &UMissionComponentActorEvents::OnActorInputTouchLeave);
	BoundActor->OnReleased.AddDynamic(this, &UMissionComponentActorEvents::OnActorReleased);
	BoundActor->OnTakeAnyDamage.AddDynamic(this, &UMissionComponentActorEvents::OnActorTakeAnyDamage);
	BoundActor->OnTakePointDamage.AddDynamic(this, &UMissionComponentActorEvents::OnActorTakePointDamage);
}

void UMissionComponentActorEvents::Unbind()
{
	BoundActor->OnActorBeginOverlap.RemoveDynamic(this, &UMissionComponentActorEvents::OnActorBeginOverlap);
	BoundActor->OnActorEndOverlap.RemoveDynamic(this, &UMissionComponentActorEvents::OnActorEndOverlap);
	BoundActor->OnActorHit.RemoveDynamic(this, &UMissionComponentActorEvents::OnActorHit);
	BoundActor->OnBeginCursorOver.RemoveDynamic(this, &UMissionComponentActorEvents::OnActorBeginCursorOver);
	BoundActor->OnClicked.RemoveDynamic(this, &UMissionComponentActorEvents::OnActorClicked);
	BoundActor->OnDestroyed.RemoveDynamic(this, &UMissionComponentActorEvents::OnActorDestroyed);
	BoundActor->OnEndCursorOver.RemoveDynamic(this, &UMissionComponentActorEvents::OnActorEndCursorOver);
	BoundActor->OnEndPlay.RemoveDynamic(this, &UMissionComponentActorEvents::OnActorEndPlay);
	BoundActor->OnInputTouchBegin.RemoveDynamic(this, &UMissionComponentActorEvents::OnActorInputTouchBegin);
	BoundActor->OnInputTouchEnd.RemoveDynamic(this, &UMissionComponentActorEvents::OnActorInputTouchEnd);
	BoundActor->OnInputTouchEnter.RemoveDynamic(this, &UMissionComponentActorEvents::OnActorInputTouchEnter);
	BoundActor->OnInputTouchLeave.RemoveDynamic(this, &UMissionComponentActorEvents::OnActorInputTouchLeave);
	BoundActor->OnReleased.RemoveDynamic(this, &UMissionComponentActorEvents::OnActorReleased);
	BoundActor->OnTakeAnyDamage.RemoveDynamic(this, &UMissionComponentActorEvents::OnActorTakeAnyDamage);
	BoundActor->OnTakePointDamage.RemoveDynamic(this, &UMissionComponentActorEvents::OnActorTakePointDamage);
}

