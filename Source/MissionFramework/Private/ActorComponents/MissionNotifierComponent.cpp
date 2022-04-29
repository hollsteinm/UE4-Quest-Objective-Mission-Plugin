// Fill out your copyright notice in the Description page of Project Settings.

#include "MissionFrameworkPrivatePCH.h"
#include "ActorComponents/MissionNotifierComponent.h"


// Sets default values for this component's properties
UMissionNotifierComponent::UMissionNotifierComponent()
{

}


void UMissionNotifierComponent::NotifyMission()
{
	OnNotifyMission.Broadcast(GetOwner(), this);
}