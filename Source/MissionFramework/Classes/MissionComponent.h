#pragma once

#include "MissionTypes.h"
#include "MissionComponent.generated.h"

/**
* The behaviour driver for a Mission. The Mission Component has three roles within a Mission as well as knowledge of what
* states it partakes in. This is the main class that, if none of the default MissionComponents satisfy your needs, should
* be overriden for functions evaluating whether a Mission is passed or failed.
*/
UCLASS(Abstract, Within = MissionState, ClassGroup = Mission, Config = Game, BlueprintType, Blueprintable, EditInlineNew, NotPlaceable)
class MISSIONFRAMEWORK_API UMissionComponent : public UObject
{
	GENERATED_BODY()

public:
	UMissionComponent(const FObjectInitializer& ObjectInitializer);

	/*Begin UObject Interface*/
	virtual class UWorld* GetWorld() const override;
	/*End UObject Interface*/

	/* Tick the Component. Best for Evaluations that need are time based or need precision. */
	UFUNCTION()
		void TickComponent(float DeltaTime);

	/* Do we even bother calling the TickComponent function on this Coponent? */
	UFUNCTION(BlueprintCallable, Category = "Mission|Component")
		bool CanEverTick() const;

	/* Are we in a valid state and is it set to being Active? */
	UFUNCTION(BlueprintCallable, Category = "Mission|Component")
		bool IsEnabled() const;

	void Activate();

	void Deactivate();

	void ExitState(FName ExitedStateName, FName NextStateName);

	void EnterState(FName EnteredStateState, FName PreviousStateName);

	bool HasResult(EMissionComponentResult TestResult) const;

	void Reset();

	DECLARE_EVENT_TwoParams(UMissionComponent, FOnMissionComponentFinished, UMissionComponent*, bool);
	FOnMissionComponentFinished& OnFinished() { return OnFinishedEvent; }

	DECLARE_EVENT_OneParam(UMissionComponent, FOnActiveChanged, UMissionComponent*);
	FOnActiveChanged& OnActiveChanged() { return ActiveChangedEvent; }

private:
	UPROPERTY(EditAnywhere, Category = "Component")
		TArray<FName> EnableInStates;

	UPROPERTY(EditAnywhere, Category = "Component")
		TArray<FName> DisableInStates;

	UPROPERTY(EditAnywhere, Category = "Component")
		bool bCanEverTick;

	UPROPERTY(EditAnywhere, Category = "Component")
		bool bIsActive;

	UPROPERTY()
		bool bInEnabledState;

	UPROPERTY()
		EMissionComponentResult Result;

		FOnMissionComponentFinished OnFinishedEvent;

		FOnActiveChanged ActiveChangedEvent;

protected:
	UFUNCTION(BlueprintCallable, Category = "Mission|Component")
		void Finish(bool bSatisfied);

	/* Notify this Component that the supplied state is being exited for the Mission */
	UFUNCTION(BlueprintNativeEvent, Category="Mission|Component")
		void OnStateExit(FName ExitedStateName, FName NextStateName);

	/* Notify this Component that the supplied state is being entered for the Mission */
	UFUNCTION(BlueprintNativeEvent, Category = "Mission|Component")
		void OnStateEnter(FName EnteredStateState, FName PreviousStateName);

	/* This component has been ticked */
	UFUNCTION(BlueprintNativeEvent, Category = "Mission|Component")
		void OnTick(float DeltaTime);

	UFUNCTION(BlueprintNativeEvent, Category = "Mission|Component")
		void OnReset();

	UFUNCTION(BlueprintNativeEvent, Category = "Mission|Component")
		void OnActivate();

	UFUNCTION(BlueprintNativeEvent, Category = "Mission|Component")
		void OnDeactivate();

	/* Set the active state of this component*/
	UFUNCTION(BlueprintCallable, Category = "Mission|Component")
		void SetActive(bool bNewActive);

	/*Set the can tick property of this component*/
	UFUNCTION(BlueprintCallable, Category = "Mission|Component")
		void SetCanTick(bool bNewCanTick);

	UPROPERTY(EditAnywhere, Category = "Component")
		bool bResetOnStateExit;

	UPROPERTY(EditAnywhere, Category = "Component")
		bool bResetOnStateEnter;

};