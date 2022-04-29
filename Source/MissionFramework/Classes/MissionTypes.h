#pragma once

#include "MissionTypes.generated.h"


UENUM(BlueprintType)
enum class EMissionComponentResult : uint8
{
	Satisfied,
	Pending,
	NotSatisfied
};

struct FStringBools
{
	static const FString True;
	static const FString False;
};

struct FBuiltinStates
{
	static const FName Start;
	static const FName Fail;
	static const FName Pass;
	static const FName Invalid;
};

UENUM(BlueprintType)
enum class EEvaluationOperation : uint8
{
	LessThan UMETA(DisplayName="<"),
	GreaterThan UMETA(DisplayName = ">"),
	EqualTo UMETA(DisplayName = "=="),
	LessThanOrEqualTo UMETA(DisplayName = "<="),
	GreaterThanOrEqualTo UMETA(DisplayName = ">="),
	NotEqualTo UMETA(DisplayName = "!=")
};

USTRUCT(BlueprintType)
struct MISSIONFRAMEWORK_API FMissionRewardCosmetics
{
	GENERATED_USTRUCT_BODY()
public:
	FMissionRewardCosmetics();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FText Title;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FText DescriptionShort;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FText DescriptionLong;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UTexture* Icon;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		USoundCue* RewardReceivedSound;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FString ValueAsString;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FText CategoryText;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 Index;

};

USTRUCT(BlueprintType)
struct MISSIONFRAMEWORK_API FMissionComponentCosmetics
{
	GENERATED_USTRUCT_BODY()

public:
	FMissionComponentCosmetics();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FText Title;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FText DescriptionShort;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FText DescriptionLong;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UTexture* Icon;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float PercentComplete;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bFinished;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bPassed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 Index;
};

USTRUCT(BlueprintType)
struct MISSIONFRAMEWORK_API FMissionCosmetics
{
	GENERATED_USTRUCT_BODY()

public:
	FMissionCosmetics();

	/*The Category Group for this mission*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FText CategoryText;

	/*The Title or Name of this Mission*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FText Title;

	/*A short description of the Mission*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FText DescriptionShort;

	/*A long description of the Mission*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FText DescriptionLong;

	/*The small UI Icon for the mission*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UTexture* Icon;

	/*The large UI Image for the mission*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UTexture* Image;

	/*The amount that this mission is complete*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float PercentComplete;

	/*Boolean stating if the Mission complete, whether it is Failed or Passed*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bFinished;

	/*Boolean stating whether the Mission has Passed, false and bFinished is true means it has Failed*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bPassed;

	/*Whether this Mission is being tracked right now or not*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bTracking;

	/*Index or Id of the Mission*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 Index;

	/*The Components inside of this mission*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FMissionComponentCosmetics> MissionComponentCosmetics;

	/*The Rewards that this mission offers*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FMissionRewardCosmetics> MissionRewardCosmetics;
};


/** SEE UMG */
template<typename T>
inline bool IsConcreteTypeCompatibleWithReflectedType(UProperty* Property)
{
	if (UStructProperty* StructProperty = Cast<UStructProperty>(Property))
	{
		return StructProperty->Struct == T::StaticStruct();
	}

	return false;
}

template<>
inline bool IsConcreteTypeCompatibleWithReflectedType<bool>(UProperty* Property)
{
	return Property->GetClass() == UBoolProperty::StaticClass();
}

template<>
inline bool IsConcreteTypeCompatibleWithReflectedType<int8>(UProperty* Property)
{
	return Property->GetClass() == UInt8Property::StaticClass();
}

template<>
inline bool IsConcreteTypeCompatibleWithReflectedType<uint8>(UProperty* Property)
{
	return Property->GetClass() == UByteProperty::StaticClass();
}

template<>
inline bool IsConcreteTypeCompatibleWithReflectedType<int32>(UProperty* Property)
{
	return Property->GetClass() == UIntProperty::StaticClass();
}

template<>
inline bool IsConcreteTypeCompatibleWithReflectedType<float>(UProperty* Property)
{
	return Property->GetClass() == UFloatProperty::StaticClass();
}

template<>
inline bool IsConcreteTypeCompatibleWithReflectedType<FText>(UProperty* Property)
{
	return Property->GetClass() == UTextProperty::StaticClass();
}

template<>
inline bool IsConcreteTypeCompatibleWithReflectedType<FString>(UProperty* Property)
{
	return Property->GetClass() == UStrProperty::StaticClass();
}

template<>
inline bool IsConcreteTypeCompatibleWithReflectedType<FName>(UProperty* Property)
{
	return Property->GetClass() == UNameProperty::StaticClass();
}

template<>
inline bool IsConcreteTypeCompatibleWithReflectedType<FVector>(UProperty* Property)
{
	static const FString CPPName("FVector");
	return Property->GetCPPType().Equals(CPPName);
}

template<>
inline bool IsConcreteTypeCompatibleWithReflectedType<FTransform>(UProperty* Property)
{
	static const FString CPPName("FTransform");
	return Property->GetCPPType().Equals(CPPName);
}

template<>
inline bool IsConcreteTypeCompatibleWithReflectedType<FRotator>(UProperty* Property)
{
	static const FString CPPName("FRotator");
	return Property->GetCPPType().Equals(CPPName);
}

template<>
inline bool IsConcreteTypeCompatibleWithReflectedType<UObject*>(UProperty* Property)
{
	if (UObjectProperty* ObjectProperty = Cast<UObjectProperty>(Property))
	{
		return true;
		//return ObjectProperty->PropertyClass->IsChildOf(T::StaticClass());
	}

	return false;
}

/** A struct used for caching part of a property path.  Don't use this class directly. */
USTRUCT()
struct MISSIONFRAMEWORK_API FMissionPropertyPathSegment
{
	GENERATED_USTRUCT_BODY()

public:
	/** Implementation detail, don't use this constructor. */
	FMissionPropertyPathSegment();

	/** Initializes the segment for particular name. */
	FMissionPropertyPathSegment(FString SegmentName);

	/**
	* Resolves the name on the given Struct.  Caches the resulting property so that future calls can be processed quickly.
	* @param InStruct the ScriptStruct or Class to look for the property on.
	*/
	UField* Resolve(UStruct* InStruct) const;

	/** The sub-component of the property path, a single value between .'s of the path */
	UPROPERTY(EditAnywhere)
		FName Name;

	/** The optional array index. */
	UPROPERTY(EditAnywhere)
		int32 ArrayIndex;

	UPROPERTY(EditAnywhere)
		FName CPPTypeName;

private:

	/** The cached Class or ScriptStruct that was used last to resolve Name to a property. */
	UPROPERTY(Transient)
		mutable UStruct* Struct;

	/**
	* The cached property on the Struct that this Name resolved to on it last time Resolve was called, if
	* the Struct doesn't change, this value is returned to avoid performing another Field lookup.
	*/
	UPROPERTY(Transient)
		mutable UField* Field;
};

/** */
USTRUCT()
struct MISSIONFRAMEWORK_API FMissionDynamicPropertyPath
{
	GENERATED_USTRUCT_BODY()

public:

	/** */
	FMissionDynamicPropertyPath();

	/** */
	FMissionDynamicPropertyPath(FString Path);

	/** */
	FMissionDynamicPropertyPath(const TArray<FString>& PropertyChain);

	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> TargetClass;

	UPROPERTY(EditAnywhere)
		TArray<FMissionPropertyPathSegment> Segments;

	/** */
	bool IsValid() const { return Segments.Num() > 0 && TargetClass != nullptr; }

	/** */
	template<typename T>
	bool GetValue(UObject* InContainer, T& OutValue) const
	{
		UProperty* OutProperty;
		return GetValue<T>(InContainer, OutValue, OutProperty);
	}

	/** */
	template<typename T>
	bool GetValue(UObject* InContainer, T& OutValue, UProperty*& OutProperty) const
	{
		if (InContainer && IsValid())
		{
			return GetValueRecursive(InContainer->GetClass(), InContainer, INDEX_NONE, 0, OutValue, OutProperty);
		}

		return false;
	}

	bool GetValuePtr(UObject* InContainer, void*& OutValuePtr, UProperty*& OutProperty) const
	{
		if (InContainer && IsValid())
		{
			return GetValuePtrRecursive(InContainer->GetClass(), InContainer, INDEX_NONE, 0, OutValuePtr, OutProperty);
		}

		return false;
	}

	FORCEINLINE FName GetPropertyName() const { return Segments.Last().Name; }
	FORCEINLINE FName GetPropertyTypeName() const { return Segments.Last().CPPTypeName; }

	FString GetPropertyPath() const;

private:

	bool GetValuePtrRecursive(UStruct* InStruct, void* InContainer, int32 ArrayIndex, int32 SegmentIndex, void*& OutValuePtr, UProperty*& OutProperty) const;
	/**
	* Evaluates the dynamic property path, and gets the value or calls the function at the end of the evaluation if possible.
	* Otherwise returns false.
	*/
	template<typename T>
	bool GetValueRecursive(UStruct* InStruct, void* InContainer, int32 ArrayIndex, int32 SegmentIndex, T& OutValue, UProperty*& OutProperty) const
	{
		if (!InStruct->IsChildOf(*TargetClass) && SegmentIndex == 0)
		{
			return false;
		}

		const FMissionPropertyPathSegment& Segment = Segments[SegmentIndex];

		// Obtain the property info from the given structure definition
		if (UField* Field = Segment.Resolve(InStruct))
		{
			if (UProperty* Property = Cast<UProperty>(Field))
			{
				if (SegmentIndex < (Segments.Num() - 1))
				{
					// Check first to see if this is a simple object (eg. not an array of objects)
					if (UObjectProperty* ObjectProperty = Cast<UObjectProperty>(Property))
					{
						// If it's an object we need to get the value of the property in the container first before we 
						// can continue, if the object is null we safely stop processing the chain of properties.
						if (UObject* CurrentObject = ObjectProperty->GetPropertyValue_InContainer(InContainer))
						{
							return GetValueRecursive(CurrentObject->GetClass(), CurrentObject, ArrayIndex, SegmentIndex + 1, OutValue, OutProperty);
						}
					}
					// Check to see if this is a simple weak object property (eg. not an array of weak objects).
					if (UWeakObjectProperty* WeakObjectProperty = Cast<UWeakObjectProperty>(Property))
					{
						FWeakObjectPtr WeakObject = WeakObjectProperty->GetPropertyValue_InContainer(InContainer);

						// If it's an object we need to get the value of the property in the container first before we 
						// can continue, if the object is null we safely stop processing the chain of properties.
						if (UObject* CurrentObject = WeakObject.Get())
						{
							return GetValueRecursive(CurrentObject->GetClass(), CurrentObject, ArrayIndex, SegmentIndex + 1, OutValue, OutProperty);
						}
					}
					// Check to see if this is a simple structure (eg. not an array of structures)
					else if (UStructProperty* StructProp = Cast<UStructProperty>(Property))
					{
						//TODO: Get String, Text, Name, Vector, Rotator, and Transforms
						// Recursively call back into this function with the structure property and container value
						return GetValueRecursive(StructProp->Struct, StructProp->ContainerPtrToValuePtr<void>(InContainer), ArrayIndex, SegmentIndex + 1, OutValue, OutProperty);
					}
					else if (UArrayProperty* ArrayProp = Cast<UArrayProperty>(Property))
					{
						// It is an array, now check to see if this is an array of structures
						if (UStructProperty* ArrayOfStructsProp = Cast<UStructProperty>(ArrayProp->Inner))
						{
							FScriptArrayHelper_InContainer ArrayHelper(ArrayProp, InContainer);
							if (ArrayHelper.IsValidIndex(Segment.ArrayIndex))
							{
								// Recursively call back into this function with the array element and container value
								return GetValueRecursive(ArrayOfStructsProp->Struct, ArrayHelper.GetRawPtr(Segment.ArrayIndex), ArrayIndex, SegmentIndex + 1, OutValue, OutProperty);
							}
						}
						// if it's not an array of structs, maybe it's an array of classes
						//else if ( UObjectProperty* ObjectProperty = Cast<UObjectProperty>(ArrayProp->Inner) )
						{
							//TODO Add support for arrays of objects.
						}
					}
				}
				else
				{
					// We're on the final property in the path, it may be an array property, so check that first.
					if (UArrayProperty* ArrayProp = Cast<UArrayProperty>(Property))
					{
						// if it's an array property, we need to see if we parsed an index as part of the segment
						// as a user may have baked the index directly into the property path.
						int32 Index = ArrayIndex != INDEX_NONE ? ArrayIndex : Segment.ArrayIndex;
						ensure(Index != INDEX_NONE);

						// Property is an array property, so make sure we have a valid index specified
						FScriptArrayHelper_InContainer ArrayHelper(ArrayProp, InContainer);
						if (ArrayHelper.IsValidIndex(Index))
						{
							// Verify that the cpp type matches a known property type.
							if (IsConcreteTypeCompatibleWithReflectedType<T>(ArrayProp->Inner))
							{
								// Ensure that the element sizes are the same, prevents the user from doing something terribly wrong.
								if (ArrayProp->Inner->ElementSize == sizeof(T))
								{
									OutValue = *static_cast<T*>(static_cast<void*>(ArrayHelper.GetRawPtr(Index)));
									OutProperty = ArrayProp->Inner;
								}
							}
						}
					}
					else
					{
						// Verify that the cpp type matches a known property type.
						if (IsConcreteTypeCompatibleWithReflectedType<T>(Property))
						{
							// Ensure that the element sizes are the same, prevents the user from doing something terribly wrong.
							if (Property->ElementSize == sizeof(T))
							{
								// Property is a vector property, so access directly
								if (T* ValuePtr = Property->ContainerPtrToValuePtr<T>(InContainer))
								{
									OutValue = *ValuePtr;
									OutProperty = Property;
									return true;
								}
							}
						}
					}
				}
			}
			else
			{
				// Only allow functions as the final link in the chain.
				if (SegmentIndex == (Segments.Num() - 1))
				{
					UFunction* Function = CastChecked<UFunction>(Field);
					UObject* ContainerObject = static_cast<UObject*>(InContainer);

					// We only support calling functions that return a single value and take no parameters.
					if (Function->NumParms == 1)
					{
						// Verify there's a return property.
						if (UProperty* ReturnProperty = Function->GetReturnProperty())
						{
							// Verify that the cpp type matches a known property type.
							if (IsConcreteTypeCompatibleWithReflectedType<T>(ReturnProperty))
							{
								// Ensure that the element sizes are the same, prevents the user from doing something terribly wrong.
								if (ReturnProperty->ElementSize == sizeof(T) && !ContainerObject->IsUnreachable())
								{
									ContainerObject->ProcessEvent(Function, &OutValue);
									return true;
								}
							}
						}
					}
				}
			}
		}

		return false;
	}
};