// Fill out your copyright notice in the Description page of Project Settings.

#include "MissionFrameworkPrivatePCH.h"
#include "Components/MissionComponentBindableBase.h"

UMissionComponentBindableBase::UMissionComponentBindableBase(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer),
	BoundActor(nullptr)
{

}

void UMissionComponentBindableBase::Rebind()
{
	if (BoundActor)
	{
		Unbind();
		Bind();
	}
}

void UMissionComponentBindableBase::Bind()
{
	UE_LOG(LogTemp, Warning, TEXT("Not Implemented"));
}

void UMissionComponentBindableBase::Unbind()
{
	UE_LOG(LogTemp, Warning, TEXT("Not Implemented"));
}

void UMissionComponentBindableBase::OnActivate_Implementation()
{
	if (BoundActor)
	{
		Bind();
	}
}

void UMissionComponentBindableBase::OnDeactivate_Implementation()
{
	if (BoundActor)
	{
		Unbind();
	}
}

void UMissionComponentBindableBase::OnReset_Implementation()
{
	if (BoundActor)
	{
		Rebind();
	}
}