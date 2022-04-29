// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/MissionComponentBindableBase.h"
#include "MissionComponentActorEvents.generated.h"

/**
 * 
 */
UCLASS()
class MISSIONFRAMEWORK_API UMissionComponentActorEvents : public UMissionComponentBindableBase
{
	GENERATED_BODY()

protected:
	virtual void Bind() override;
	virtual void Unbind() override;

protected:
	UFUNCTION(BlueprintImplementableEvent)
		void OnActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION(BlueprintImplementableEvent)
		void OnActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION(BlueprintImplementableEvent)
		void OnActorHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintImplementableEvent)
		void OnActorBeginCursorOver(AActor* TouchedActor);

	UFUNCTION(BlueprintImplementableEvent)
		void OnActorClicked(AActor* TouchedActor, FKey ButtonPressed);

	UFUNCTION(BlueprintImplementableEvent)
		void OnActorDestroyed(AActor* DestroyedActor);

	UFUNCTION(BlueprintImplementableEvent)
		void OnActorEndCursorOver(AActor* TouchedActor);

	UFUNCTION(BlueprintImplementableEvent)
		void OnActorEndPlay(AActor* Actor, EEndPlayReason::Type EndPlayReason);

	UFUNCTION(BlueprintImplementableEvent)
		void OnActorInputTouchBegin(ETouchIndex::Type FingerIndex, AActor* TouchedActor);

	UFUNCTION(BlueprintImplementableEvent)
		void OnActorInputTouchEnd(ETouchIndex::Type FingerIndex, AActor* TouchedActor);

	UFUNCTION(BlueprintImplementableEvent)
		void OnActorInputTouchEnter(ETouchIndex::Type FingerIndex, AActor* TouchedActor);

	UFUNCTION(BlueprintImplementableEvent)
		void OnActorInputTouchLeave(ETouchIndex::Type FingerIndex, AActor* TouchedActor);

	UFUNCTION(BlueprintImplementableEvent)
		void OnActorReleased(AActor* TouchedActor, FKey ButtonReleased);

	UFUNCTION(BlueprintImplementableEvent)
		void OnActorTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION(BlueprintImplementableEvent)
		void OnActorTakePointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser);

	
	
};
