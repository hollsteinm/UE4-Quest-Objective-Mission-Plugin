// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "MissionTypes.h"
#include "MissionBindingObservable.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FMissionBindingChangedBoolean, FName, PropertyName, AActor*, OwnerActor, class UMissionBindingObservable*, OwnerComponent, bool, bNewValue, bool, bOldValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FMissionBindingChangedInteger, FName, PropertyName, AActor*, OwnerActor, class UMissionBindingObservable*, OwnerComponent, int32, NewValue, int32, OldValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FMissionBindingChangedFloat, FName, PropertyName, AActor*, OwnerActor, class UMissionBindingObservable*, OwnerComponent, float, NewValue, float, OldValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FMissionBindingChangedString, FName, PropertyName, AActor*, OwnerActor, class UMissionBindingObservable*, OwnerComponent, FString, NewValue, FString, OldValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FMissionBindingChangedText, FName, PropertyName, AActor*, OwnerActor, class UMissionBindingObservable*, OwnerComponent, FText, NewValue, FText, OldValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FMissionBindingChangedVector, FName, PropertyName, AActor*, OwnerActor, class UMissionBindingObservable*, OwnerComponent, FVector, NewValue, FVector, OldValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FMissionBindingChangedRotator, FName, PropertyName, AActor*, OwnerActor, class UMissionBindingObservable*, OwnerComponent, FRotator, NewValue, FRotator, OldValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FMissionBindingChangedTransform, FName, PropertyName, AActor*, OwnerActor, class UMissionBindingObservable*, OwnerComponent, FTransform, NewValue, FTransform, OldValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FMissionBindingChangedName, FName, PropertyName, AActor*, OwnerActor, class UMissionBindingObservable*, OwnerComponent, FName, NewValue, FName, OldValue);

UCLASS(ClassGroup = Mission, meta = (BlueprintSpawnableComponent))
class MISSIONFRAMEWORK_API UMissionBindingObservable : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMissionBindingObservable();

	// UActorComponent Interface Begin
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	// UActorComponent Interface End

	UFUNCTION(BlueprintCallable, Category = "Mission")
		void SyncBindings();

	UPROPERTY(BlueprintAssignable)
		FMissionBindingChangedBoolean OnBooleanChanged;

	UPROPERTY(BlueprintAssignable)
		FMissionBindingChangedInteger OnIntegerChanged;

	UPROPERTY(BlueprintAssignable)
		FMissionBindingChangedFloat OnFloatChanged;

	UPROPERTY(BlueprintAssignable)
		FMissionBindingChangedString OnStringChanged;

	UPROPERTY(BlueprintAssignable)
		FMissionBindingChangedText OnTextChanged;

	UPROPERTY(BlueprintAssignable)
		FMissionBindingChangedVector OnVectorChanged;

	UPROPERTY(BlueprintAssignable)
		FMissionBindingChangedRotator OnRotatorChanged;

	UPROPERTY(BlueprintAssignable)
		FMissionBindingChangedTransform OnTransformChanged;

	UPROPERTY(BlueprintAssignable)
		FMissionBindingChangedName OnNameChanged;

protected:
	UPROPERTY(EditAnywhere)
		TArray<FMissionDynamicPropertyPath> Bindings;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bSyncBindingsOnTick;

private:

	void ReceiveBooleanChanged(FName PropertyName, AActor* OwnerActor, bool bNewValue, bool bOldValue);
	void ReceiveIntegerChanged(FName PropertyName, AActor* OwnerActor, int32 NewValue, int32 OldValue);
	void ReceiveFloatChanged(FName PropertyName, AActor* OwnerActor, float NewValue, float OldValue);
	void ReceiveStringChanged(FName PropertyName, AActor* OwnerActor, FString OldValue, FString NewValue);
	void ReceiveTextChanged(FName PropertyName, AActor* OwnerActor, FText OldValue, FText NewValue);
	void ReceiveVectorChanged(FName PropertyName, AActor* OwnerActor, FVector OldValue, FVector NewValue);
	void ReceiveRotatorChanged(FName PropertyName, AActor* OwnerActor, FRotator OldValue, FRotator NewValue);
	void ReceiveTransformChanged(FName PropertyName, AActor* OwnerActor, FTransform OldValue, FTransform NewValue);
	void ReceiveNameChanged(FName PropertyName, AActor* OwnerActor, FName OldValue, FName NewValue);

	TMap<int32, FVariant> OldValues;
	TMap<int32, FText> OldTextValues;

	template<typename TType>
	FORCEINLINE void SyncField(const FMissionDynamicPropertyPath& Path, int32 Index, void (UMissionBindingObservable::*OnChangeDetectedFunc)(FName, class AActor*, TType, TType))
	{
		TType NowValue;
		if (Path.GetValue<TType>(GetOwner(), NowValue))
		{
			TType OldValue = GetLastValue<TType>(Index);
			if (NowValue != OldValue)
			{
				OldValues[Index] = NowValue;
				(this->*OnChangeDetectedFunc)(Path.GetPropertyName(), GetOwner(), NowValue, OldValue);
			}
		}
	}

	template<typename TType>
	FORCEINLINE TType GetLastValue(int32 Index)
	{
		if (OldValues.Contains(Index))
		{
			TType Result = OldValues[Index];
			return Result;
		}
		else
		{
			OldValues.Add(Index, TType());
			return TType();
		}
	}


};
