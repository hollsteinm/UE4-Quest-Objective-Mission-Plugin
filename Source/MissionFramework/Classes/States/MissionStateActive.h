// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MissionState.h"
#include "MissionStateActive.generated.h"

/**
 * Mediator for Mission completion / failure evaluation
 */
UCLASS()
class MISSIONFRAMEWORK_API UMissionStateActive : public UMissionState
{
	GENERATED_BODY()
public:
	UMissionStateActive(const FObjectInitializer& ObjectInitializer);

	/*UMissionState Interface Begin*/
	virtual FName GetStateName() const override;
	/*UMissionState Interface End*/

protected:
	void OnMissionComponentFinish_Implementation(UMissionComponent* Component, bool bResult) override;
};