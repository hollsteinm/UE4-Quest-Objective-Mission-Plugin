#include "MissionFrameworkPrivatePCH.h"
#include "ActorComponents/MissionBindingObservable.h"

UMissionBindingObservable::UMissionBindingObservable()
	: Super(),
	bSyncBindingsOnTick(true)
{
	PrimaryComponentTick.bCanEverTick = true;
	bAutoRegister = true;
	bAutoActivate = true;
}

void UMissionBindingObservable::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bSyncBindingsOnTick)
	{
		SyncBindings();
	}
}

void UMissionBindingObservable::ReceiveBooleanChanged(FName PropertyName, AActor* OwnerActor, bool bNewValue, bool bOldValue)
{
	OnBooleanChanged.Broadcast(PropertyName, OwnerActor, this, bNewValue, bOldValue);
}

void UMissionBindingObservable::ReceiveIntegerChanged(FName PropertyName, AActor* OwnerActor, int32 NewValue, int32 OldValue)
{
	OnIntegerChanged.Broadcast(PropertyName, OwnerActor, this, NewValue, OldValue);
}

void UMissionBindingObservable::ReceiveFloatChanged(FName PropertyName, AActor* OwnerActor, float NewValue, float OldValue)
{
	OnFloatChanged.Broadcast(PropertyName, OwnerActor, this, NewValue, OldValue);
}

void UMissionBindingObservable::ReceiveStringChanged(FName PropertyName, AActor* OwnerActor, FString NewValue, FString OldValue)
{
	OnStringChanged.Broadcast(PropertyName, OwnerActor, this, NewValue, OldValue);
}

void UMissionBindingObservable::ReceiveTextChanged(FName PropertyName, AActor* OwnerActor, FText NewValue, FText OldValue)
{
	OnTextChanged.Broadcast(PropertyName, OwnerActor, this, NewValue, OldValue);
}

void UMissionBindingObservable::ReceiveVectorChanged(FName PropertyName, AActor* OwnerActor, FVector NewValue, FVector OldValue)
{
	OnVectorChanged.Broadcast(PropertyName, OwnerActor, this, NewValue, OldValue);
}

void UMissionBindingObservable::ReceiveRotatorChanged(FName PropertyName, AActor* OwnerActor, FRotator NewValue, FRotator OldValue)
{
	OnRotatorChanged.Broadcast(PropertyName, OwnerActor, this, NewValue, OldValue);
}

void UMissionBindingObservable::ReceiveTransformChanged(FName PropertyName, AActor* OwnerActor, FTransform NewValue, FTransform OldValue)
{
	OnTransformChanged.Broadcast(PropertyName, OwnerActor, this, NewValue, OldValue);
}

void UMissionBindingObservable::ReceiveNameChanged(FName PropertyName, AActor* OwnerActor, FName NewValue, FName OldValue)
{
	OnNameChanged.Broadcast(PropertyName, OwnerActor, this, NewValue, OldValue);
}

void UMissionBindingObservable::SyncBindings()
{
	int32 Idx = 0;
	for (auto& Binding : Bindings)
	{
		FName CPPName = Binding.GetPropertyTypeName();
		if (CPPName == "FVector")
		{
			SyncField<FVector>(Binding, Idx, &UMissionBindingObservable::ReceiveVectorChanged);
		}

		else if (CPPName == "FRotator")
		{
			SyncField<FRotator>(Binding, Idx, &UMissionBindingObservable::ReceiveRotatorChanged);
		}

		else if (CPPName == "FTransform")
		{
			FTransform NowValue;
			if (Binding.GetValue<FTransform>(GetOwner(), NowValue))
			{
				FTransform OldValue = GetLastValue<FTransform>(Idx);
				if (!NowValue.Equals(OldValue))
				{
					OldValues[Idx] = NowValue;
					ReceiveTransformChanged(Binding.GetPropertyName(), GetOwner(), NowValue, OldValue);
				}
			}
		}

		else if (CPPName == "FString")
		{
			SyncField<FString>(Binding, Idx, &UMissionBindingObservable::ReceiveStringChanged);
		}

		else if (CPPName == "FName")
		{
			SyncField<FName>(Binding, Idx, &UMissionBindingObservable::ReceiveNameChanged);
		}

		else if (CPPName == "FText")
		{
			FText NowValue;
			if (Binding.GetValue<FText>(GetOwner(), NowValue))
			{
				FText OldValue;
				if (OldTextValues.Contains(Idx))
				{
					OldValue = OldTextValues[Idx];
				}
				else
				{
					OldTextValues.Add(Idx, FText());
				}
				
				if (!NowValue.EqualTo(OldValue))
				{
					OldTextValues[Idx] = NowValue;
					ReceiveTextChanged(Binding.GetPropertyName(), GetOwner(), NowValue, OldValue);
				}
			}
		}

		else if (CPPName == "int32")
		{
			SyncField<int32>(Binding, Idx, &UMissionBindingObservable::ReceiveIntegerChanged);
		}

		else if (CPPName == "uint8" || CPPName == "bool")
		{
			SyncField<bool>(Binding, Idx, &UMissionBindingObservable::ReceiveBooleanChanged);
		}

		else if (CPPName == "float")
		{
			SyncField<float>(Binding, Idx, &UMissionBindingObservable::ReceiveFloatChanged);
		}

		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%s was not a handled types"), *CPPName.ToString());
		}

		Idx++;
	}
}