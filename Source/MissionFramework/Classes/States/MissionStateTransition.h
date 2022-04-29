// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MissionState.h"
#include "MissionStateTransition.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTransitionEventSignature, class UMission*, NewMission);

/**
 * 
 */
UCLASS()
class MISSIONFRAMEWORK_API UMissionStateTransition : public UMissionState
{
	GENERATED_BODY()
	
public:
	UMissionStateTransition(const FObjectInitializer& ObjectInitializer);

	/**Begin UMissionState Interface**/
	virtual FName GetStateName() const override;
	/**End UMissionState Interface**/

	/* Event fired when a new Mission is Spawned and set to an Active State*/
	UPROPERTY(BlueprintAssignable, EditAnywhere)
		FTransitionEventSignature OnTransition;

protected:
	/* The Mission that will be created and Set to an Active State*/
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UMission> TransitionMission;

	/* Should we clean up the Mission (in memory) that this component is owned by? */
	UPROPERTY(EditDefaultsOnly)
		bool bDestroyLeadingMission;

	virtual void OnEnter_Implementation(FName ExitedStateName) override;

};