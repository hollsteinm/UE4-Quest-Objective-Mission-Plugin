#include "MissionFrameworkPrivatePCH.h"
#include "Rewards/MissionRewardPropertyMutate.h"

FMissionRewardPropertyMutateConfig::FMissionRewardPropertyMutateConfig() :
	MutationOperation(EMissionRewardPropertyMutateOp::Set),
	PropertyPath(),
	ValueToApplyAsString()
{

}

UMissionRewardPropertyMutate::UMissionRewardPropertyMutate(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer),
	Configuration()
{
}

void UMissionRewardPropertyMutate::OnApply_Implementation(AActor* Actor)
{
	FPropertyRewardMutator::Mutate(Actor, Configuration);
}

void FPropertyRewardMutator::Mutate(AActor* Actor, FMissionRewardPropertyMutateConfig& Configuration)
{
	const TArray<UClass*> BlacklistedProperties =
	{
		UArrayProperty::StaticClass(),
		UDelegateProperty::StaticClass(),
		UInterfaceProperty::StaticClass(),
		UMapProperty::StaticClass(),
		UMulticastDelegateProperty::StaticClass(),
		USetProperty::StaticClass()
	};

	if (Actor->IsA(*Configuration.PropertyPath.TargetClass))
	{
		void* ValuePtr = nullptr;
		UProperty* Property = nullptr;
		if (Configuration.PropertyPath.GetValuePtr(Actor, ValuePtr, Property))
		{
			if (!BlacklistedProperties.Contains(Property->GetClass()))
			{
				if (UNumericProperty* NumericProperty = Cast<UNumericProperty>(Property))
				{
					if (UByteProperty* ByteProperty = Cast<UByteProperty>(NumericProperty))
					{
						if (ByteProperty->IsEnum())
						{
							if (UEnum* ByteEnum = ByteProperty->Enum)
							{
								if(ByteEnum->IsValidEnumName(*Configuration.ValueToApplyAsString) || ByteEnum->IsValidEnumValue((uint8)FCString::Atoi(*Configuration.ValueToApplyAsString)))
								{
									switch (Configuration.MutationOperation)
									{
									case EMissionRewardPropertyMutateOp::Set:
										ByteProperty->SetNumericPropertyValueFromString(ValuePtr, *Configuration.ValueToApplyAsString);
										return;
									default:
										break;
									}
								}
							}
						}
					}
					else if (UIntProperty* IntProperty = Cast<UIntProperty>(NumericProperty))
					{
						int32 ValueFromString = FCString::Atoi(*Configuration.ValueToApplyAsString);
						int32 Value = *static_cast<int32*>(ValuePtr);
						switch (Configuration.MutationOperation)
						{
						case EMissionRewardPropertyMutateOp::Set:
							Value = ValueFromString;
							break;
						case EMissionRewardPropertyMutateOp::Add:
							Value += ValueFromString;
							break;
						case EMissionRewardPropertyMutateOp::Subtract:
							Value -= ValueFromString;
							break;
						case EMissionRewardPropertyMutateOp::Multiply:
							Value *= ValueFromString;
							break;
						case EMissionRewardPropertyMutateOp::Divide:
							Value /= ValueFromString;
							break;
						default:
							break;
						}
						IntProperty->SetIntPropertyValue(ValuePtr, (int64)Value);
						return;
					}
					else if (UFloatProperty* FloatProperty = Cast<UFloatProperty>(NumericProperty))
					{
						float ValueFromString = FCString::Atof(*Configuration.ValueToApplyAsString);
						float Value = *static_cast<float*>(ValuePtr);
						switch (Configuration.MutationOperation)
						{
						case EMissionRewardPropertyMutateOp::Set:
							Value = ValueFromString;
							break;
						case EMissionRewardPropertyMutateOp::Add:
							Value += ValueFromString;
							break;
						case EMissionRewardPropertyMutateOp::Subtract:
							Value -= ValueFromString;
							break;
						case EMissionRewardPropertyMutateOp::Multiply:
							Value *= ValueFromString;
							break;
						case EMissionRewardPropertyMutateOp::Divide:
							Value /= ValueFromString;
							break;
						default:
							break;
						}
						FloatProperty->SetFloatingPointPropertyValue(ValuePtr, Value);
						return;
					}
				}
				else if (UBoolProperty* BoolProperty = Cast<UBoolProperty>(Property))
				{
					bool bValueFromString = Configuration.ValueToApplyAsString.ToBool();
					switch (Configuration.MutationOperation)
					{
					case EMissionRewardPropertyMutateOp::Set:
						BoolProperty->SetPropertyValue(ValuePtr, bValueFromString);
						return;
					default:
						break;
					}
				}
				else
				{
					if (IsConcreteTypeCompatibleWithReflectedType<FText>(Property))
					{
						if (UTextProperty* TextProperty = Cast<UTextProperty>(Property))
						{
							FText ValueFromString = FText::FromString(Configuration.ValueToApplyAsString);
							switch (Configuration.MutationOperation)
							{
							case EMissionRewardPropertyMutateOp::Set:
								TextProperty->SetPropertyValue(ValuePtr, ValueFromString);
								return;
							default:
								break;
							}
						}
					}
					else if (IsConcreteTypeCompatibleWithReflectedType<FString>(Property))
					{
						if (UStrProperty* StrProperty = Cast<UStrProperty>(Property))
						{
							FString Value = *static_cast<FString*>(ValuePtr);
							FString ValueFromString = Configuration.ValueToApplyAsString;
							switch (Configuration.MutationOperation)
							{
							case EMissionRewardPropertyMutateOp::Set:
								StrProperty->SetPropertyValue(ValuePtr, ValueFromString);
								return;
							case EMissionRewardPropertyMutateOp::Add:
								Value += ValueFromString;
								StrProperty->SetPropertyValue(ValuePtr, ValueFromString);
								return;
							default:
								break;
							}
						}
					}
				}
			}

			UE_LOG(LogTemp, Warning, TEXT("%s was unhandled - Configuration(Op[%s], Path(%s), ValueToApplyAsString(%s)"),
				*Property->GetFullName(),
				*FindObject<UEnum>(ANY_PACKAGE, TEXT("EMissionRewardPropertyMutateOp"), true)->GetNameByIndex((uint8)Configuration.MutationOperation).ToString(),
				*Configuration.PropertyPath.GetPropertyName().ToString(),
				*Configuration.ValueToApplyAsString);
			return;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Unable to get property handle and value pointer - Configuration(Op[%s], Path(%s), ValueToApplyAsString(%s) on actor %s"),
				*FindObject<UEnum>(ANY_PACKAGE, TEXT("EMissionRewardPropertyMutateOp"), true)->GetNameByIndex((uint8)Configuration.MutationOperation).ToString(),
				*Configuration.PropertyPath.GetPropertyName().ToString(),
				*Configuration.ValueToApplyAsString,
				*Actor->GetFullName());
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s is not a %s"), *Actor->GetFullName(), *Configuration.PropertyPath.TargetClass->GetFullName());
	}
}