#pragma once

#include "MissionReward.h"
#include "MissionRewardProperty.generated.h"

UCLASS(Abstract)
class UMissionRewardProperty : public UMissionReward
{
	GENERATED_BODY()
public:
	UMissionRewardProperty(const FObjectInitializer& ObjectInitializer);
};