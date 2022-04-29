// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataAsset.h"
#include "MissionTypes.h"
#include "MissionData.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class MISSIONFRAMEWORK_API UMissionData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UMissionData(const FObjectInitializer& ObjectIntialzier);

	const FGuid GetMissionReferenceId() const;

	//UObject interface
	virtual bool IsAsset() const override { return true; }
	//End UObject interface

#if WITH_EDITOR
	void LinkToMission(UMission* NewMission);
#endif

	UFUNCTION(BlueprintCallable, Category="Mission")
	FMissionCosmetics GetMissionCosmeticData() const;

	UFUNCTION(BlueprintCallable, Category="Mission")
	class UMission* GetMission() const;

private:
	UPROPERTY(EditAnywhere)
		FMissionCosmetics CosmeticData;

	UPROPERTY(EditAnywhere, Meta=(AllowedClasses="Mission"))
		FStringAssetReference MissionReference;	
};