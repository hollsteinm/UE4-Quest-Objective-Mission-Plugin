// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "MissionState.h"
#include "Tickable.h"
#include "Mission.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnPreStateTransition, class UMission*, Mission, FName, CurrentState, FName, TargetState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnPostStateTransition, class UMission*, Mission, FName, OldState, FName, NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnMissionStateMissionComponentFinished, class UMission*, Mission, class UMissionState*, State, class UMissionComponent*, Component, bool, bResult);

DECLARE_STATS_GROUP(TEXT("Mission"), STATGROUP_Mission, STATCAT_Advanced);

/**
 * A UMission is the core driver for all goal oriented objectives for an object.
 * It contains behaviour components that describe evaluations of a Mission. It
 * also contains states that define the actions to take when Ticking and Evaluating.
 */
UCLASS(BlueprintType, Config = Game, ClassGroup = Mission)
class MISSIONFRAMEWORK_API UMission : public UObject
{
	GENERATED_BODY()
public:
	UMission(const FObjectInitializer& ObjectInitializer);

	/*Tick the Mission. No need to call in the MissionTickable subclass*/
	UFUNCTION(BlueprintCallable, Category = "Mission")
		virtual void TickMission(float DeltaTime);

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

	/*Gets the FName of the Current State*/
	UFUNCTION(BlueprintCallable, Category = "Mission")
		FName GetCurrentStateName() const;

	/*Gets the FName of the Active State Variable*/
	UFUNCTION(Category = "Mission|State", BlueprintCallable)
		FName GetActiveStateName() const;

	/*Gets the FName of the Failed State Variable*/
	UFUNCTION(Category = "Mission|State", BlueprintCallable)
		FName GetFailedStateName() const;

	/*Gets the FName of the Completed State Variable*/
	UFUNCTION(Category = "Mission|State", BlueprintCallable)
		FName GetCompletedStateName() const;

	/*UObject Interface Begin*/
	virtual class UWorld* GetWorld() const override;
	virtual void PostInitProperties() override;
	virtual void BeginDestroy() override;
	/*UObject Interface End*/

	/** Wrapper functions to query outer information*/

	/* Similiar to getting the Owner. This returns the Outer / Container that holds the UMission reference*/
	UFUNCTION(BlueprintCallable, Category = "Mission")
	class AActor* GetActingActor() const;

	/* Will GetActingActor() return nullptr? */
	UFUNCTION(BlueprintCallable, Category = "Mission")
		bool IsActingObjectActor() const;

	/* Get the Auth Game Mode for this world */
	UFUNCTION(BlueprintCallable, Category = "Mission")
	class AGameMode* GetActingGameMode() const;

	/* Will GetActingController() return nullptr? */
	UFUNCTION(BlueprintCallable, Category = "Mission")
		bool DoesActingActorHaveController() const;

	/* Gets the Controller for the ActingActor, if present. */
	UFUNCTION(BlueprintCallable, Category = "Mission")
	class AController* GetActingController() const;

	/** End Wrapper Functions **/

	void SetOwningActor(AActor* Actor);

	void RemoveOwningActor();

	void Activate();

	void Deactivate();

	/*All Public Facing Events*/

	/*Fired right before a state transition is about to take place*/
	UPROPERTY(BlueprintAssignable)
		FOnPreStateTransition OnPreStateTransition;

	/*Fire right after a state transition has been successful*/
	UPROPERTY(BlueprintAssignable)
		FOnPostStateTransition OnPostStateTransition;

	/*Fired when a Mission component in a Mission state has fired the Finish event*/
	UPROPERTY(BlueprintAssignable)
		FOnMissionStateMissionComponentFinished OnStateComponentFinished;

	/*End All Public Facing Events*/

	UFUNCTION(BlueprintCallable, Category = "Mission")
		const FGuid GetUniqueMissionId() const;

protected:
	/*The initial starting state of the Mission. Typically, should not do anything*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		UMissionState* Invalid;

	/*The state to enter when a Mission is considered 'Completed' or 'Successful'. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		UMissionState* Complete;

	/*The state to enter when a Mission is considered 'Failed' or 'Incomplete' */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		UMissionState* Fail;

	/* The state to enter to evaulate the next state to enter - usually Complete or Fail.
	* This is typically the Runtime state. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		UMissionState* Active;

	/* Go to a new state. May inject a NewState; however, it will be Garbage
	* collected if not referenced anywhere else after the passed in NewState
	* is no longer the CurrentState. bForceUpdate will transition to NewState
	without conditions evaluated */
	UFUNCTION(BlueprintCallable, Category = "Mission|State")
		bool GoToState(UMissionState* NewState, bool bForceUpdate = false);

private:
	/* Our current state that we are in. Will be calle din TickMission and Evaluate*/
	UPROPERTY(SaveGame)
		UMissionState* CurrentState;

	UPROPERTY(Transient)
		class AActor* ActorContext;

	UPROPERTY()
		FGuid UniqueMissionId;

	UFUNCTION()
		void ReceiveMissionStateMissionComponentFinished(UMissionState* MissionState, UMissionComponent* Component, bool bResult);
};