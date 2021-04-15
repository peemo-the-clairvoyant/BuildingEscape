// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	owner = GetOwner();

	if (!pressurePlate)
	{
		UE_LOG(LogTemp, Log, TEXT("%s missing pressure plate"), *owner->GetName());
	}
}

void UOpenDoor::OpenDoor()
{
	/*FRotator newRotation = FRotator(owner->GetActorRotation().Pitch, openAngle, owner->GetActorRotation().Roll);

	owner->SetActorRotation(newRotation);*/
	
	onOpenRequest.Broadcast();
}

void UOpenDoor::CloseDoor()
{
	/*FRotator newRotation = FRotator(owner->GetActorRotation().Pitch, -90.f, owner->GetActorRotation().Roll);

	owner->SetActorRotation(newRotation);*/

	onCloseRequest.Broadcast();
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetTotalMassOfActorsOnPlate() > triggerMass)
	{
		OpenDoor();
		lastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	if (GetWorld()->GetTimeSeconds() - doorCloseDelay > lastDoorOpenTime)
	{
		CloseDoor();
	}
}

const float UOpenDoor::GetTotalMassOfActorsOnPlate() const
{
	float totalMass = 0.f;
	TArray<AActor*> overlappingActors;

	if (!pressurePlate) { 
		return totalMass;
	}
	pressurePlate->GetOverlappingActors(overlappingActors);

	UE_LOG(LogTemp, Warning, TEXT("Num of overlappedactors %d"), overlappingActors.Num());

	for (const auto* actor : overlappingActors)
	{
		totalMass += actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Log, TEXT("Overlapped actor %s"), *actor->GetName());
	}
	return totalMass;
}

