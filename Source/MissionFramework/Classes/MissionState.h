// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "MissionTypes.h"
#include "MissionState.generated.h"

/**
 * The main driver for a Mission based on where it is at. Pretty much defines the Runtime behaviour of a Mission
 */
UCLASS(DefaultToInstanced, Within=Mission, ClassGroup=Mission, Config=Game, BlueprintType, Blueprintable)
class MISSIONFRAMEWORK_API UMissionState : public UObject
{
	GENERATED_BODY()
public:
	UMissionState(const FObjectInitializer& ObjectInitializer);

	void TickState(float DeltaTime);

	void Enter(FName PreviousStateName);

	void Exit(FName NextStateName);

	void Activate();

	void Deactivate();

	void Reset();

	/**Begin UObject Interface**/
	virtual UWorld* GetWorld() const override;
	virtual void PostInitProperties() override;
	virtual void BeginDestroy() override;
	/**End UObjectInterface**/

	/* Get the FName of this state */
	UFUNCTION(Category = "Mission|State", BlueprintCallable)
		virtual FName GetStateName() const;

	/**UMission Getter Wrappers for convenience**/

	/* Get the First Component of the given Class Type, if any */
	UFUNCTION(BlueprintCallable, Category = "Mission|Components")
		UMissionComponent* GetComponentByClass(const TSubclassOf<class UMissionComponent>& ClassType) const;

	/* Get all Components of the given Class Type, if any */
	UFUNCTION(BlueprintCallable, Category = "Mission|Components")
		TArray<class UMissionComponent*> GetComponentsByClass(const TSubclassOf<class UMissionComponent>& ClassType) const;

	/* Get all Components, if any */
	UFUNCTION(BlueprintCallable, Category = "Mission|Components")
		TArray<class UMissionComponent*> GetComponents() const;

	/* Get the First Component of the given Class Type, if any */
	UFUNCTION(BlueprintCallable, Category = "Mission|Components")
		class UMissionReward* GetRewardByClass(const TSubclassOf<class UMissionReward>& ClassType) const;

	/* Get all Components of the given Class Type, if any */
	UFUNCTION(BlueprintCallable, Category = "Mission|Components")
		TArray<class UMissionReward*> GetRewardsByClass(const TSubclassOf<class UMissionReward>& ClassType) const;

	/* Get all Components, if any */
	UFUNCTION(BlueprintCallable, Category = "Mission|Components")
		TArray<class UMissionReward*> GetRewards() const;

	/** C++ Templates for convenience **/
	template<typename T>
	FORCEINLINE T* GetComponentByClass()
	{
		return GetComponentByClass(T::StaticClass());
	}

	template<typename T>
	FORCEINLINE T* GetRewardByClass()
	{
		return GetRewardByClass(T::StaticClass());
	}
	/**End**/
	/**End convenience*/

	DECLARE_EVENT_ThreeParams(UMissionState, FMissionComponentFinished, UMissionState*, UMissionComponent*, bool);
	FMissionComponentFinished& OnMissionComponentFinished() { return MissionComponentFinishedEvent; }

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "Mission|State")
		void OnEnter(FName PreviousStateName);

	UFUNCTION(BlueprintNativeEvent, Category = "Mission|State")
		void OnExit(FName NextStateName);

	UFUNCTION(BlueprintImplementableEvent, Category = "Mission|State")
		void OnTick(float DeltaTime);

	UFUNCTION(BlueprintNativeEvent, Category = "Mission|State")
		void OnReset();

	UFUNCTION(BlueprintImplementableEvent, Category = "Mission|State")
		void OnActivate();

	UFUNCTION(BlueprintImplementableEvent, Category = "Mission|State")
		void OnDeactivate();

	UFUNCTION(BlueprintNativeEvent, Category = "Mission|State")
		void OnMissionComponentFinish(class UMissionComponent* Component, bool bResult);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bTicks;

	UFUNCTION()
		void ApplyRewards();

	/*Dynamically add a reward of the given class*/
	UFUNCTION(BlueprintCallable, Category = "Mission|Reward")
		class UMissionReward* AddNewReward(const TSubclassOf<class UMissionReward>& NewRewardClass);

	/*Dynamically add a reward that has been instantiated.*/
	UFUNCTION(BlueprintCallable, Category = "Mission|Reward")
		void AddReward(class UMissionReward* Reward);

	/*Add a new Mission Component of the given class*/
	UFUNCTION(BlueprintCallable, Category = "Mission|Component")
		class UMissionComponent* AddNewComponent(const TSubclassOf<UMissionComponent>& NewComponentClass);

	/*Add an instantiated Mission Component*/
	UFUNCTION(BlueprintCallable, Category = "Mission|Component")
		void AddComponent(class UMissionComponent* Component);

	/** C++ Templates for convenience **/
	template<typename T>
	FORCEINLINE T* AddNewComponentOfClass()
	{
		return AddNewComponent(T::StaticClass());
	}

	template<typename T>
	FORCEINLINE T* AddNewRewardOfClass()
	{
		return AddNewReward(T::StaticClass());
	}
	/**End**/
	/**End convenience*/

private:
	/* The behavioural components that make up this mission*/
	UPROPERTY(SaveGame, Instanced, EditAnywhere, Category = "Components")
		TArray<class UMissionComponent*> Components;

	/* The rewards offered up upon exiting this state*/
	UPROPERTY(Instanced, EditAnywhere, Category = "Rewards")
		TArray<class UMissionReward*> Rewards;

	UPROPERTY(EditAnywhere)
		bool bResetOnEnter;

	UPROPERTY(EditAnywhere)
		bool bResetOnExit;

	FMissionComponentFinished MissionComponentFinishedEvent;

	UFUNCTION()
		void ReceiveMissionComponentFinished(class UMissionComponent* Component, bool bResult);

	void BindToComponent(class UMissionComponent* Component);

	void UnBindFromComponent(class UMissionComponent* Component);

	void BindToAllComponents();

	void UnBindFromAllComponents();
};