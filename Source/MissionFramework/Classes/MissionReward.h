#pragma once

#include "MissionTypes.h"
#include "MissionReward.generated.h"

/**
* The behaviour driver for a Mission. The Mission Component has three roles within a Mission as well as knowledge of what
* states it partakes in. This is the main class that, if none of the default MissionComponents satisfy your needs, should
* be overriden for functions evaluating whether a Mission is passed or failed.
*/
UCLASS(Abstract, Within = MissionState, ClassGroup = Mission, Config = Game, BlueprintType, Blueprintable, EditInlineNew, NotPlaceable)
class MISSIONFRAMEWORK_API UMissionReward : public UObject
{
	GENERATED_BODY()

public:
	UMissionReward(const FObjectInitializer& ObjectInitializer);

	void Apply(class AActor* Actor);

	UFUNCTION(BlueprintNativeEvent, Category="Mission|Reward")
		void OnApply(class AActor* Actor);
};