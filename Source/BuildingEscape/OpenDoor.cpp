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
	actorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}

void UOpenDoor::OpenDoor()
{
	FRotator newRotation = FRotator(owner->GetActorRotation().Pitch, openAngle, owner->GetActorRotation().Roll);

	owner->SetActorRotation(newRotation);
}

void UOpenDoor::CloseDoor()
{
	FRotator newRotation = FRotator(owner->GetActorRotation().Pitch, -90.f, owner->GetActorRotation().Roll);

	owner->SetActorRotation(newRotation);
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (pressurePlate && pressurePlate->IsOverlappingActor(actorThatOpens))
	{
		OpenDoor();
		lastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	if (GetWorld()->GetTimeSeconds() - doorCloseDelay > lastDoorOpenTime)
	{
		CloseDoor();
	}
}

