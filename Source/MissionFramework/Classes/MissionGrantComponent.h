// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "MissionGrantComponent.generated.h"



/*The Interfacing Actor Component that allows an AActor to take place in Missions. Replicates, and offers RPCs*/
UCLASS(ClassGroup = (Mission), meta = (BlueprintSpawnableComponent))
class MISSIONFRAMEWORK_API UMissionGrantComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMissionGrantComponent(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Mission")
	class UMission* GetMissionInstance(UObject* OwnerIfCreateCopy = nullptr, bool bCreateCopy = true);

private:
	UPROPERTY()
		class UMission* Mission;
};