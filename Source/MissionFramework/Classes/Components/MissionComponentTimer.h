#pragma once

#include "MissionComponent.h"
#include "MissionComponentTimer.generated.h"

/*
* A simple Timer Component that Evaluates when the Time is reached.
*/
UCLASS()
class MISSIONFRAMEWORK_API UMissionComponentTimer : public UMissionComponent
{
	GENERATED_BODY()
public:
	UMissionComponentTimer(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category="Mission|Timer")
		void ResetTimer();

	UFUNCTION(BlueprintCallable, Category="Mission|Timer")
		void PauseTimer();

	UFUNCTION(BlueprintCallable, Category="Mission|Timer")
		void ContinueTimer();

	UFUNCTION(BlueprintCallable, Category = "Mission|Timer")
		void StopTimer();

	UFUNCTION(BlueprintCallable, Category = "Mission|Timer")
		void PlayTimer();

	UFUNCTION(BlueprintCallable, Category = "Mission|Timer")
		float GetSecondsRemaining() const;

	UFUNCTION(BlueprintCallable, Category = "Mission|Timer")
		float GetInitialSeconds() const;

	/*Begin UObject Interface*/
	virtual void BeginDestroy() override;
	/*End UObject Interface*/

protected:
	/* How long should the Timer be set for? */
	UPROPERTY(EditAnywhere, Category = "Component")
		FTimespan TimeSpan;
		
	virtual void OnReset_Implementation() override;
	virtual void OnStateEnter_Implementation(FName EnteredStateName, FName PreviousStateName) override;
	virtual void OnStateExit_Implementation(FName ExitedStateName, FName NextStateName) override;

private:
	UPROPERTY(SaveGame)
		FTimerHandle TimerHandle;

	UPROPERTY(EditAnywhere, Category = "Component")
		bool bSuccessOnExpire;

	UFUNCTION()
		void FinishTimer();
};