// Fill out your copyright notice in the Description page of Project Settings.

#include "MissionFrameworkPrivatePCH.h"
#include "Components/MissionComponentNotification.h"
#include "ActorComponents/MissionNotifierComponent.h"

UMissionComponentNotification::UMissionComponentNotification(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer),
	ReceivedCountRequired(1),
	ReceivedCount(0)
{

}

void UMissionComponentNotification::Bind()
{
	if (UMissionNotifierComponent* Component = Cast<UMissionNotifierComponent>(BoundActor->GetComponentByClass(UMissionNotifierComponent::StaticClass())))
	{
		Component->OnNotifyMission.RemoveDynamic(this, &UMissionComponentNotification::OnNotification);
	}
}

void UMissionComponentNotification::Unbind()
{
	if (UMissionNotifierComponent* Component = Cast<UMissionNotifierComponent>(BoundActor->GetComponentByClass(UMissionNotifierComponent::StaticClass())))
	{
		Component->OnNotifyMission.RemoveDynamic(this, &UMissionComponentNotification::OnNotification);
	}
}

void UMissionComponentNotification::OnNotification(AActor* Sender, UMissionNotifierComponent* Component)
{
	if (IsEnabled())
	{
		ReceivedCount++;
		OnReceiveNotification(Sender, Component);
	}
}

void UMissionComponentNotification::OnReceiveNotification_Implementation(class AActor* Sender, class UMissionNotifierComponent* Component)
{
	if (ReceivedCountRequired == ReceivedCount)
	{
		Finish(true);
	}
}