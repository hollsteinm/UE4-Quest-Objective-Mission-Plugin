#include "MissionFrameworkPrivatePCH.h"
#include "Components/MissionComponentBindingObserver.h"
#include "ActorComponents/MissionBindingObservable.h"
#include "GameFramework/Actor.h"

UMissionComponentBindingObserver::UMissionComponentBindingObserver(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UMissionComponentBindingObserver::Bind()
{
	TArray<UActorComponent*> Bindables = BoundActor->GetComponentsByClass(UMissionBindingObservable::StaticClass());
	for (auto Bindable : Bindables)
	{
		if (UMissionBindingObservable* AsBindable = Cast<UMissionBindingObservable>(Bindable))
		{
			AsBindable->OnBooleanChanged.AddDynamic(this, &UMissionComponentBindingObserver::ReceiveBoolPropertyChanged);
			AsBindable->OnIntegerChanged.AddDynamic(this, &UMissionComponentBindingObserver::ReceiveIntPropertyChanged);
			AsBindable->OnFloatChanged.AddDynamic(this, &UMissionComponentBindingObserver::ReceiveFloatPropertyChanged);
			AsBindable->OnNameChanged.AddDynamic(this, &UMissionComponentBindingObserver::ReceiveNamePropertyChanged);
			AsBindable->OnStringChanged.AddDynamic(this, &UMissionComponentBindingObserver::ReceiveStringPropertyChanged);
			AsBindable->OnTextChanged.AddDynamic(this, &UMissionComponentBindingObserver::ReceiveTextPropertyChanged);
			AsBindable->OnVectorChanged.AddDynamic(this, &UMissionComponentBindingObserver::ReceiveVectorPropertyChanged);
			AsBindable->OnRotatorChanged.AddDynamic(this, &UMissionComponentBindingObserver::ReceiveRotatorPropertyChanged);
			AsBindable->OnTransformChanged.AddDynamic(this, &UMissionComponentBindingObserver::ReceiveTransformPropertyChanged);
		}
	}
}

void UMissionComponentBindingObserver::Unbind()
{
	TArray<UActorComponent*> Bindables = BoundActor->GetComponentsByClass(UMissionBindingObservable::StaticClass());
	for (auto Bindable : Bindables)
	{
		if (UMissionBindingObservable* AsBindable = Cast<UMissionBindingObservable>(Bindable))
		{
			AsBindable->OnBooleanChanged.RemoveAll(this);
			AsBindable->OnIntegerChanged.RemoveAll(this);
			AsBindable->OnFloatChanged.RemoveAll(this);
			AsBindable->OnNameChanged.RemoveAll(this);
			AsBindable->OnStringChanged.RemoveAll(this);
			AsBindable->OnTextChanged.RemoveAll(this);
			AsBindable->OnVectorChanged.RemoveAll(this);
			AsBindable->OnRotatorChanged.RemoveAll(this);
			AsBindable->OnTransformChanged.RemoveAll(this);
		}
	}
}

void UMissionComponentBindingObserver::ReceiveBoolPropertyChanged(FName PropertyName, class AActor* Sender, class UMissionBindingObservable* SenderComponent, bool bOldValue, bool bNewValue)
{
	if (IsEnabled())
	{
		OnBoolPropertyChanged(PropertyName, Sender, SenderComponent, bOldValue, bNewValue);
	}
}

void UMissionComponentBindingObserver::ReceiveFloatPropertyChanged(FName PropertyName, class AActor* Sender, class UMissionBindingObservable* SenderComponent, float OldValue, float NewValue)
{
	if (IsEnabled())
	{
		OnFloatPropertyChanged(PropertyName, Sender, SenderComponent, OldValue, NewValue);
	}
}

void UMissionComponentBindingObserver::ReceiveIntPropertyChanged(FName PropertyName, class AActor* Sender, class UMissionBindingObservable* SenderComponent, int32 OldValue, int32 NewValue)
{
	if (IsEnabled())
	{
		OnIntPropertyChanged(PropertyName, Sender, SenderComponent, OldValue, NewValue);
	}
}

void UMissionComponentBindingObserver::ReceiveNamePropertyChanged(FName PropertyName, class AActor* Sender, class UMissionBindingObservable* SenderComponent, FName OldValue, FName NewValue)
{
	if (IsEnabled())
	{
		OnNamePropertyChanged(PropertyName, Sender, SenderComponent, OldValue, NewValue);
	}
}

void UMissionComponentBindingObserver::ReceiveStringPropertyChanged(FName PropertyName, class AActor* Sender, class UMissionBindingObservable* SenderComponent, FString OldValue, FString NewValue)
{
	if (IsEnabled())
	{
		OnStringPropertyChanged(PropertyName, Sender, SenderComponent, OldValue, NewValue);
	}
}

void UMissionComponentBindingObserver::ReceiveTextPropertyChanged(FName PropertyName, class AActor* Sender, class UMissionBindingObservable* SenderComponent, FText OldValue, FText NewValue)
{
	if (IsEnabled())
	{
		OnTextPropertyChanged(PropertyName, Sender, SenderComponent, OldValue, NewValue);
	}
}

void UMissionComponentBindingObserver::ReceiveVectorPropertyChanged(FName PropertyName, class AActor* Sender, class UMissionBindingObservable* SenderComponent, FVector OldValue, FVector NewValue)
{
	if (IsEnabled())
	{
		OnVectorPropertyChanged(PropertyName, Sender, SenderComponent, OldValue, NewValue);
	}
}

void UMissionComponentBindingObserver::ReceiveRotatorPropertyChanged(FName PropertyName, class AActor* Sender, class UMissionBindingObservable* SenderComponent, FRotator OldValue, FRotator NewValue)
{
	if (IsEnabled())
	{
		OnRotatorPropertyChanged(PropertyName, Sender, SenderComponent, OldValue, NewValue);
	}
}

void UMissionComponentBindingObserver::ReceiveTransformPropertyChanged(FName PropertyName, class AActor* Sender, class UMissionBindingObservable* SenderComponent, FTransform OldValue, FTransform NewValue)
{
	if (IsEnabled())
	{
		OnTransformPropertyChanged(PropertyName, Sender, SenderComponent, OldValue, NewValue);
	}
}