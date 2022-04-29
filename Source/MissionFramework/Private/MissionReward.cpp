#include "MissionFrameworkPrivatePCH.h"
#include "MissionReward.h"

UMissionReward::UMissionReward(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{

}

void UMissionReward::Apply(AActor* Actor)
{
	UE_LOG(LogTemp, Log, TEXT("Applying Reward %s to Actor %s"), *GetFullName(), *Actor->GetFullName());
	OnApply(Actor);
}

void UMissionReward::OnApply_Implementation(AActor* Actor)
{
	UE_LOG(LogTemp, Warning, TEXT("Not Implemented"));
}