// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "MissionNotifierComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FNotifyMissionSignature, class AActor*, Owner, class UMissionNotifierComponent*, Component);

UCLASS( ClassGroup=Mission, meta=(BlueprintSpawnableComponent) )
class MISSIONFRAMEWORK_API UMissionNotifierComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMissionNotifierComponent();

	UPROPERTY(BlueprintAssignable)
		FNotifyMissionSignature OnNotifyMission;

	UFUNCTION(BlueprintCallable, Category = "Mission")
		void NotifyMission();
	
};
