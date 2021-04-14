// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get player view point this tick
	FVector playerViewPointLocation;
	FRotator playerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(playerViewPointLocation, playerViewPointRotation);


	FVector lineTraceEnd = playerViewPointLocation + playerViewPointRotation.Vector() * reach;

	DrawDebugLine(GetWorld(), playerViewPointLocation, lineTraceEnd, FColor::Red, false, 0.f, 0.f, 10.f);

	// Setup query parameters
	FCollisionQueryParams traceParams(FName(TEXT("")), false, GetOwner());
	// Ray-cast out to reach distance
	FHitResult hit;
	GetWorld()->LineTraceSingleByObjectType(hit, playerViewPointLocation, lineTraceEnd, FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), traceParams);

	// See what we hit
	AActor* actorHit = hit.GetActor();
	if (actorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line trace hit: %s"), *(actorHit->GetName()));
	}
}

