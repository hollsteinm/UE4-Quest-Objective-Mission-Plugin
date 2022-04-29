
#include "MissionFrameworkPrivatePCH.h"
#include "MissionTypes.h"
#include "UnrealNetwork.h"

const FName FBuiltinStates::Start = TEXT("Active");
const FName FBuiltinStates::Fail = TEXT("Failed");
const FName FBuiltinStates::Pass = TEXT("Completed");
const FName FBuiltinStates::Invalid = TEXT("Invalid");

const FString FStringBools::True = "True";
const FString FStringBools::False = "False";

FMissionRewardCosmetics::FMissionRewardCosmetics() :
	Title(),
	DescriptionShort(),
	DescriptionLong(),
	Icon(nullptr),
	RewardReceivedSound(nullptr),
	ValueAsString(),
	CategoryText(),
	Index(INDEX_NONE)
{
}

FMissionComponentCosmetics::FMissionComponentCosmetics() :
	Title(),
	DescriptionShort(),
	DescriptionLong(),
	Icon(nullptr),
	PercentComplete(0.0f),
	bFinished(false),
	bPassed(false),
	Index(INDEX_NONE)
{
}

FMissionCosmetics::FMissionCosmetics() :
	CategoryText(),
	Title(),
	DescriptionShort(),
	DescriptionLong(),
	Icon(nullptr),
	Image(nullptr),
	PercentComplete(0.0f),
	bFinished(false),
	bPassed(false),
	bTracking(false),
	Index(INDEX_NONE),
	MissionComponentCosmetics(),
	MissionRewardCosmetics()
{
}


FMissionPropertyPathSegment::FMissionPropertyPathSegment()
	: Name(NAME_None)
	, ArrayIndex(INDEX_NONE)
	, Struct(nullptr)
	, Field(nullptr)
{

}

FMissionPropertyPathSegment::FMissionPropertyPathSegment(FString SegmentName)
	: ArrayIndex(INDEX_NONE)
	, Struct(nullptr)
	, Field(nullptr)
{
	// Parse the property name and (optional) array index
	int32 ArrayPos = SegmentName.Find(TEXT("["));
	if (ArrayPos != INDEX_NONE)
	{
		FString IndexToken = SegmentName.RightChop(ArrayPos + 1).LeftChop(1);
		ArrayIndex = FCString::Atoi(*IndexToken);

		SegmentName = SegmentName.Left(ArrayPos);
	}

	Name = FName(*SegmentName);
}

UField* FMissionPropertyPathSegment::Resolve(UStruct* InStruct) const
{
	if (InStruct)
	{
		// only perform the find field work if the structure where this property would resolve to
		// has changed.  If it hasn't changed, the just return the UProperty we found last time.
		if (InStruct != Struct)
		{
			Struct = InStruct;
			Field = FindField<UField>(InStruct, Name);
		}

		return Field;
	}

	return nullptr;
}

FMissionDynamicPropertyPath::FMissionDynamicPropertyPath()
{
}

FMissionDynamicPropertyPath::FMissionDynamicPropertyPath(FString Path) :
	TargetClass(nullptr)
{
	FString PropertyString;
	while (Path.Split(TEXT("."), &PropertyString, &Path))
	{
		if (!PropertyString.IsEmpty())
		{
			Segments.Add(FMissionPropertyPathSegment(PropertyString));
		}
	}

	Segments.Add(FMissionPropertyPathSegment(Path));

}

FMissionDynamicPropertyPath::FMissionDynamicPropertyPath(const TArray<FString>& PropertyChain) :
	TargetClass(nullptr)
{
	for (const FString& Segment : PropertyChain)
	{
		Segments.Add(FMissionPropertyPathSegment(Segment));
	}
}

FString FMissionDynamicPropertyPath::GetPropertyPath() const
{
	FString Final;
	for (auto Segment : Segments)
	{
		Final += Segment.Name.ToString() + ".";
	}
	Final.RemoveFromEnd(".");
	return Final;
}

bool FMissionDynamicPropertyPath::GetValuePtrRecursive(UStruct* InStruct, void* InContainer, int32 ArrayIndex, int32 SegmentIndex, void*& OutValuePtr, UProperty*& OutProperty) const
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
						return GetValuePtrRecursive(CurrentObject->GetClass(), CurrentObject, ArrayIndex, SegmentIndex + 1, OutValuePtr, OutProperty);
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
						return GetValuePtrRecursive(CurrentObject->GetClass(), CurrentObject, ArrayIndex, SegmentIndex + 1, OutValuePtr, OutProperty);
					}
				}
				// Check to see if this is a simple structure (eg. not an array of structures)
				else if (UStructProperty* StructProp = Cast<UStructProperty>(Property))
				{
					// Recursively call back into this function with the structure property and container value
					return GetValuePtrRecursive(StructProp->Struct, StructProp->ContainerPtrToValuePtr<void>(InContainer), ArrayIndex, SegmentIndex + 1, OutValuePtr, OutProperty);
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
							return GetValuePtrRecursive(ArrayOfStructsProp->Struct, ArrayHelper.GetRawPtr(Segment.ArrayIndex), ArrayIndex, SegmentIndex + 1, OutValuePtr, OutProperty);
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
						if (ArrayProp->Inner)
						{
							OutValuePtr = static_cast<void*>(ArrayHelper.GetRawPtr(Index));
							OutProperty = ArrayProp->Inner;
							return true;
						}
					}
				}
				else
				{
					// Property is a vector property, so access directly
					if (void* ValuePtr = Property->ContainerPtrToValuePtr<void>(InContainer))
					{
						OutValuePtr = ValuePtr;
						OutProperty = Property;
						return true;
					}
				}
			}
		}
	}

	return false;
}