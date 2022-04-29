// Fill out your copyright notice in the Description page of Project Settings.

#include "MissionFrameworkPrivatePCH.h"
#include "MissionActorComponent.h"
#include "Mission.h"
#include "MissionTypes.h"
#include "MissionComponent.h"
#include "UnrealNetwork.h"


// Sets default values for this component's properties
UMissionActorComponent::UMissionActorComponent(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer),
	bGoToActiveStateOnBeginPlay(true),
	bLoadHistoryOnBeginPlay(true),
	bAlreadyBound(false),
	bTrackHistory(true),
	AutoTransitionedStateNameOnMissionDestroy(FBuiltinStates::Pass)
{
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
	bAutoRegister = true;
	bAutoActivate = true;
	bWantsInitializeComponent = true;
	Mission = ObjectInitializer.CreateDefaultSubobject<UMission>(this, TEXT("Mission"));
	SetNetAddressable();
	SetIsReplicated(true);
}

void UMissionActorComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

// Called when the game starts
void UMissionActorComponent::BeginPlay()
{
	Super::BeginPlay();
	if (GetOwner()->HasAuthority())
	{
		if (Mission)
		{
			Mission->Activate();
			BindToMissionEvents(Mission);
			if (bGoToActiveStateOnBeginPlay)
			{
				Mission->GoToActiveState();
			}
		}
	}
}

// Called when the game starts
void UMissionActorComponent::EndPlay( const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (GetOwner()->HasAuthority())
	{
		UnbindFromMissionEvents(Mission);
		if (Mission)
		{
			Mission->Deactivate();
		}
	}
}

// Called every frame
void UMissionActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (Mission && TickType != ELevelTick::LEVELTICK_PauseTick)
	{
		Mission->TickMission(DeltaTime);
	}
}

void UMissionActorComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UMissionActorComponent::BindToMissionEvents(class UMission* BindingMission)
{
	if (!bAlreadyBound)
	{
		Mission->SetOwningActor(GetOwner());
		BindingMission->OnStateComponentFinished.AddDynamic(this, &UMissionActorComponent::ReceiveComponentFinished);
		BindingMission->OnPreStateTransition.AddDynamic(this, &UMissionActorComponent::ReceivePreStateChange);
		BindingMission->OnPostStateTransition.AddDynamic(this, &UMissionActorComponent::ReceivePostStateChange);
		bAlreadyBound = true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Bind to Mission Events called when already bound. Unbind from old Mission first"));
	}
}

void UMissionActorComponent::UnbindFromMissionEvents(class UMission* BindingMission)
{
	if (bAlreadyBound)
	{
		Mission->RemoveOwningActor();
		BindingMission->OnStateComponentFinished.RemoveDynamic(this, &UMissionActorComponent::ReceiveComponentFinished);
		BindingMission->OnPreStateTransition.RemoveDynamic(this, &UMissionActorComponent::ReceivePreStateChange);
		BindingMission->OnPostStateTransition.RemoveDynamic(this, &UMissionActorComponent::ReceivePostStateChange);
		bAlreadyBound = false;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unbind From Mission Events called, but not bound to anythings (hopefully)"));
	}
}

void UMissionActorComponent::ReceiveComponentFinished(class UMission* TriggeringMission, class UMissionState* State, class UMissionComponent* Component, bool bResult)
{
	OnMissionComponentFinished.Broadcast(this, TriggeringMission, State, Component, bResult);
}

void UMissionActorComponent::ReceivePreStateChange(class UMission* TriggeringMission, FName CurrentStateName, FName NewStateName)
{
	OnPreMissionStateChange.Broadcast(this, TriggeringMission, CurrentStateName, NewStateName);
}

void UMissionActorComponent::ReceivePostStateChange(class UMission* TriggeringMission, FName CurrentStateName, FName OldStateName)
{
	OnPostMissionStateChange.Broadcast(this, TriggeringMission, CurrentStateName, OldStateName);
}

void UMissionActorComponent::UpdateMissionFromClass(TSubclassOf<UMission> NewMission, FName MoveOldToThisState)
{
	if (GetOwner()->HasAuthority() || IsNetMode(NM_DedicatedServer))
	{
		UMission* SpawnedMission = NewObject<UMission>(this, *NewMission);
		if (SpawnedMission)
		{
			UpdateMission(SpawnedMission, MoveOldToThisState);
		}
	}
	else
	{
		Server_UpdateMissionFromClass(NewMission, MoveOldToThisState);
	}
}

void UMissionActorComponent::UpdateMission(UMission* NewMission, FName MoveOldToThisState)
{
	if (GetOwner()->HasAuthority() || IsNetMode(NM_DedicatedServer))
	{
		if (NewMission && NewMission != Mission)
		{
			if (MoveOldToThisState != NAME_None && Mission)
			{
				bool bTransitioned = false;
				if (MoveOldToThisState.IsEqual(Mission->GetCompletedStateName()))
				{
					bTransitioned = Mission->GoToCompletedState();
				}
				else if (MoveOldToThisState.IsEqual(Mission->GetFailedStateName()))
				{
					bTransitioned = Mission->GoToFailedState();
				}
				else if (MoveOldToThisState.IsEqual(Mission->GetActiveStateName()))
				{
					bTransitioned = Mission->GoToActiveState();
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Invalid Parameter MoveOldToThisState: %s"), *MoveOldToThisState.ToString());
				}
			}

			Mission->Deactivate();
			UMission* OldMission = Mission;
			Mission = NewMission;
			Mission->SetOwningActor(GetOwner());
			UnbindFromMissionEvents(OldMission);
			BindToMissionEvents(Mission);
			if (bTrackHistory && OldMission)
			{
			}
			if (OldMission)
			{
			}
		}
	}
	else
	{
		Server_UpdateMission(NewMission, MoveOldToThisState);
	}
}

void UMissionActorComponent::Server_UpdateMission_Implementation(UMission* NewMission, FName MoveOldToThisState)
{
	UpdateMission(NewMission, MoveOldToThisState);
}

bool UMissionActorComponent::Server_UpdateMission_Validate(UMission* NewMission, FName MoveOldToThisState) const
{
	return NewMission != nullptr;
}

void UMissionActorComponent::Server_UpdateMissionFromClass_Implementation(TSubclassOf<UMission> NewMission, FName MoveOldToThisState)
{
	UpdateMissionFromClass(NewMission, MoveOldToThisState);
}

bool UMissionActorComponent::Server_UpdateMissionFromClass_Validate(TSubclassOf<UMission> NewMission, FName MoveOldToThisState) const
{
	return NewMission != nullptr;
}