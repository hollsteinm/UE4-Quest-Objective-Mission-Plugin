// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MissionComponent.h"
#include "MissionComponentChildMission.generated.h"

/**
 * 
 */
UCLASS()
class MISSIONFRAMEWORK_API UMissionComponentChildMission : public UMissionComponent
{
	GENERATED_BODY()
	
public:
	UMissionComponentChildMission(const FObjectInitializer& ObjectInitializer);

	/*Begin UObject Interface*/
	virtual void PostInitProperties() override;
	/*End UObject Interface*/

	/*UMission Wrappers Begin*/
	/* Transition to the Completed State Varaible. Will call
	* OnExit for the Current State, then if no other transitions
	* take place, OnEnter for the Completed State
	*/
	UFUNCTION(BlueprintCallable, Category = "Mission")
		bool GoToCompletedState();

	/* Transition to the Failed State Varaible. Will call
	* OnExit for the Current State, then if no other transitions
	* take place, OnEnter for the Failed State
	*/
	UFUNCTION(BlueprintCallable, Category = "Mission")
		bool GoToFailedState();

	/* Transition to the Active State Variable, Will call
	* OnExit on the Current State, then if no other transitions
	* take place, OnEnter will be called for the Active State.
	* Typically the Active State is the main Driver for Evaluation
	* conditions to transition to the Failed or Completed State*/
	UFUNCTION(BlueprintCallable, Category = "Mission")
		bool GoToActiveState();
	/*UMission Wrappers End*/

protected:
	/*UMissionComponent Interface Begin*/
	virtual void OnStateEnter_Implementation(FName EnteredStateState, FName PreviousStateName) override;
	virtual void OnTick_Implementation(float DeltaTime) override;
	/*UMissionComponent Interface End*/

private:
	UPROPERTY(Instanced, EditAnywhere, SaveGame, Category = "Component")
		class UMission* ChildMission;
	
	
};
