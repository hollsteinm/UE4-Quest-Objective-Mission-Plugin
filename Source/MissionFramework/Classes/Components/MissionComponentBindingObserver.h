#pragma once

#include "MissionComponentBindableBase.h"
#include "MissionComponentBindingObserver.generated.h"

UCLASS(Abstract)
class MISSIONFRAMEWORK_API UMissionComponentBindingObserver : public UMissionComponentBindableBase
{
	GENERATED_BODY()

public:
	UMissionComponentBindingObserver(const FObjectInitializer& ObjectInitializer);

protected:
		virtual void Bind() override;
		virtual void Unbind() override;

	UFUNCTION(BlueprintImplementableEvent)
		void OnBoolPropertyChanged(FName PropertyName, class AActor* Sender, class UMissionBindingObservable* SenderComponent, bool bOldValue, bool bNewValue);

	UFUNCTION(BlueprintImplementableEvent)
		void OnFloatPropertyChanged(FName PropertyName, class AActor* Sender, class UMissionBindingObservable* SenderComponent, float OldValue, float NewValue);

	UFUNCTION(BlueprintImplementableEvent)
		void OnIntPropertyChanged(FName PropertyName, class AActor* Sender, class UMissionBindingObservable* SenderComponent, int32 OldValue, int32 NewValue);

	UFUNCTION(BlueprintImplementableEvent)
		void OnNamePropertyChanged(FName PropertyName, class AActor* Sender, class UMissionBindingObservable* SenderComponent, FName OldValue, FName NewValue);

	UFUNCTION(BlueprintImplementableEvent)
		void OnStringPropertyChanged(FName PropertyName, class AActor* Sender, class UMissionBindingObservable* SenderComponent, FString& OldValue, FString& NewValue);

	UFUNCTION(BlueprintImplementableEvent)
		void OnTextPropertyChanged(FName PropertyName, class AActor* Sender, class UMissionBindingObservable* SenderComponent, FText& OldValue, FText& NewValue);

	UFUNCTION(BlueprintImplementableEvent)
		void OnVectorPropertyChanged(FName PropertyName, class AActor* Sender, class UMissionBindingObservable* SenderComponent, FVector OldValue, FVector NewValue);

	UFUNCTION(BlueprintImplementableEvent)
		void OnRotatorPropertyChanged(FName PropertyName, class AActor* Sender, class UMissionBindingObservable* SenderComponent, FRotator OldValue, FRotator NewValue);

	UFUNCTION(BlueprintImplementableEvent)
		void OnTransformPropertyChanged(FName PropertyName, class AActor* Sender, class UMissionBindingObservable* SenderComponent, FTransform OldValue, FTransform NewValue);

private:
	UFUNCTION()
		void ReceiveBoolPropertyChanged(FName PropertyName, class AActor* Sender, class UMissionBindingObservable* SenderComponent, bool bOldValue, bool bNewValue);

	UFUNCTION()
		void ReceiveFloatPropertyChanged(FName PropertyName, class AActor* Sender, class UMissionBindingObservable* SenderComponent, float OldValue, float NewValue);

	UFUNCTION()
		void ReceiveIntPropertyChanged(FName PropertyName, class AActor* Sender, class UMissionBindingObservable* SenderComponent, int32 OldValue, int32 NewValue);

	UFUNCTION()
		void ReceiveNamePropertyChanged(FName PropertyName, class AActor* Sender, class UMissionBindingObservable* SenderComponent, FName OldValue, FName NewValue);

	UFUNCTION()
		void ReceiveStringPropertyChanged(FName PropertyName, class AActor* Sender, class UMissionBindingObservable* SenderComponent, FString OldValue, FString NewValue);

	UFUNCTION()
		void ReceiveTextPropertyChanged(FName PropertyName, class AActor* Sender, class UMissionBindingObservable* SenderComponent, FText OldValue, FText NewValue);

	UFUNCTION()
		void ReceiveVectorPropertyChanged(FName PropertyName, class AActor* Sender, class UMissionBindingObservable* SenderComponent, FVector OldValue, FVector NewValue);

	UFUNCTION()
		void ReceiveRotatorPropertyChanged(FName PropertyName, class AActor* Sender, class UMissionBindingObservable* SenderComponent, FRotator OldValue, FRotator NewValue);

	UFUNCTION()
		void ReceiveTransformPropertyChanged(FName PropertyName, class AActor* Sender, class UMissionBindingObservable* SenderComponent, FTransform OldValue, FTransform NewValue);
};