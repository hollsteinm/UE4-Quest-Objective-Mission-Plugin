// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "MissionActorComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FOnMissionActorComponentMissionComponentFinished, class UMissionActorComponent*, ActorComponent, class UMission*, Mission, class UMissionState*, State, class UMissionComponent*, Component, bool, bResult);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnMissionActorComponentMissionStateTransition, class UMissionActorComponent*, ActorComponent, class UMission*, Mission, FName, CurrentState, FName, OtherState);

/*The Interfacing Actor Component that allows an AActor to take place in Missions. Replicates, and offers RPCs*/
UCLASS(ClassGroup = (Mission), meta = (BlueprintSpawnableComponent))
class MISSIONFRAMEWORK_API UMissionActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMissionActorComponent(const FObjectInitializer& ObjectInitializer);

	/*UActorComponent Interface start*/
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void InitializeComponent() override;
	/*UActorComponent Interface end*/

	/*Switch the current Mission that this Component is working with*/
	UFUNCTION(BlueprintCallable, Category = "Mission")
		void UpdateMission(UMission* NewMission, FName MoveOldToThisState = NAME_None);

	UFUNCTION(BlueprintCallable, Category = "Mission")
		void UpdateMissionFromClass(TSubclassOf<UMission> NewMission, FName MoveOldToThisState = NAME_None);

	/*Event fired when one of the MissionComponents has reached a Finish State*/
	UPROPERTY(BlueprintAssignable)
		FOnMissionActorComponentMissionComponentFinished OnMissionComponentFinished;

	UPROPERTY(BlueprintAssignable)
		FOnMissionActorComponentMissionStateTransition OnPreMissionStateChange;

	UPROPERTY(BlueprintAssignable)
		FOnMissionActorComponentMissionStateTransition OnPostMissionStateChange;

#if WITH_EDITOR
	FORCEINLINE class UMission* GetMission() const { return Mission; }
#endif

private:
	UPROPERTY(EditAnywhere)
		bool bTrackHistory;

	UPROPERTY(EditAnywhere)
		FName AutoTransitionedStateNameOnMissionDestroy;

	UPROPERTY(SaveGame)
	class UMission* Mission;

	UPROPERTY(EditAnywhere, Category = "Config")
		bool bGoToActiveStateOnBeginPlay;

	UPROPERTY(EditAnywhere, Category = "Config")
		bool bLoadHistoryOnBeginPlay;

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_UpdateMission(UMission* NewMission, FName MoveOldToThisState = NAME_None);
	virtual void Server_UpdateMission_Implementation(UMission* NewMission, FName MoveOldToThisState = NAME_None);
	virtual bool Server_UpdateMission_Validate(UMission* NewMission, FName MoveOldToThisState = NAME_None) const;

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_UpdateMissionFromClass(TSubclassOf<UMission> NewMission, FName MoveOldToThisState = NAME_None);
	virtual void Server_UpdateMissionFromClass_Implementation(TSubclassOf<UMission> NewMission, FName MoveOldToThisState = NAME_None);
	virtual bool Server_UpdateMissionFromClass_Validate(TSubclassOf<UMission> NewMission, FName MoveOldToThisState = NAME_None) const;

	void BindToMissionEvents(class UMission* BindingMission);
	void UnbindFromMissionEvents(class UMission* BindingMission);

	UFUNCTION()
		void ReceiveComponentFinished(class UMission* TriggeringMission, class UMissionState* State, class UMissionComponent* Component, bool bResult);

	UFUNCTION()
		void ReceivePreStateChange(class UMission* TriggeringMission, FName CurrentStateName, FName NewStateName);

	UFUNCTION()
		void ReceivePostStateChange(class UMission* TriggeringMission, FName CurrentStateName, FName OldStateName);

	bool bAlreadyBound;
};
