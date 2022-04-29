// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MissionComponentBindableBase.h"
#include "MissionComponentNotification.generated.h"

/**
 * 
 */
UCLASS()
class MISSIONFRAMEWORK_API UMissionComponentNotification : public UMissionComponentBindableBase
{
	GENERATED_BODY()
	
public:
	UMissionComponentNotification(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void Bind() override;
	virtual void Unbind() override;

	UFUNCTION(BlueprintNativeEvent, Category = "Mission")
		void OnReceiveNotification(class AActor* Sender, class UMissionNotifierComponent* Component);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Component")
		int32 ReceivedCountRequired;

private:
	UFUNCTION()
		void OnNotification(class AActor* Sender, class UMissionNotifierComponent* Component);	

	UPROPERTY(SaveGame)
		int32 ReceivedCount;
};