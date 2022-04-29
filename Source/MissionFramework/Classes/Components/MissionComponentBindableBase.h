// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MissionComponent.h"
#include "MissionComponentBindableBase.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class MISSIONFRAMEWORK_API UMissionComponentBindableBase : public UMissionComponent
{
	GENERATED_BODY()

public:
	UMissionComponentBindableBase(const FObjectInitializer& ObjectInitializer);

	/*UMissionComponent Interface Begin*/
	virtual void OnActivate_Implementation() override;
	virtual void OnDeactivate_Implementation() override;
	virtual void OnReset_Implementation() override;
	/*UMissionComponent Interface End*/

protected:
	void Rebind();

	virtual void Bind();

	virtual void Unbind();
	
	UPROPERTY(EditAnywhere, Category = "Component")
		class AActor* BoundActor;
	
};
