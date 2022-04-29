#pragma once

#include "MissionRewardProperty.h"
#include "MissionRewardPropertyMutate.generated.h"

UENUM(BlueprintType)
enum class EMissionRewardPropertyMutateOp : uint8
{
	Add,
	Subtract,
	Divide,
	Multiply,
	Set
};

USTRUCT(BlueprintType)
struct MISSIONFRAMEWORK_API FMissionRewardPropertyMutateConfig
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, meta=(ClassPickerVisible=true))
		FMissionDynamicPropertyPath PropertyPath;

	UPROPERTY(EditAnywhere)
		EMissionRewardPropertyMutateOp MutationOperation;

	UPROPERTY(EditAnywhere)
		FString ValueToApplyAsString;

	FMissionRewardPropertyMutateConfig();

};

struct MISSIONFRAMEWORK_API FPropertyRewardMutator
{
	static void Mutate(class AActor* Actor, FMissionRewardPropertyMutateConfig& Configuration);
};

UCLASS()
class MISSIONFRAMEWORK_API UMissionRewardPropertyMutate : public UMissionRewardProperty
{
	GENERATED_BODY()

public:
	UMissionRewardPropertyMutate(const FObjectInitializer& ObjectInitializer);

protected:
	/*UMissionReward interface begin*/
	virtual void OnApply_Implementation(class AActor* Actor) override;
	/*UMissionReward interface end*/

private:
	UPROPERTY(EditAnywhere)
		FMissionRewardPropertyMutateConfig Configuration;
};