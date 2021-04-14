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

	FindPhysicsHandleComponent();
	
	SetupInputComponent();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector playerViewPointLocation;
	FRotator playerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(playerViewPointLocation, playerViewPointRotation);


	FVector lineTraceEnd = playerViewPointLocation + playerViewPointRotation.Vector() * reach;
	/// if the physics handle is attached, move the object we are holding
	if (physicsHandle->GrabbedComponent)
	{
		physicsHandle->SetTargetLocation(lineTraceEnd);
	}
}

void UGrabber::FindPhysicsHandleComponent()
{
	/// Look for attached physics handle
	physicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (physicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle"), *GetOwner()->GetName());
	}
}

void UGrabber::SetupInputComponent()
{
	/// Look for attac hed input component only when running
	inputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (inputComponent)
	{
		/// Bind the input axis
		inputComponent->BindAction("Grab", EInputEvent::IE_Pressed, this, &UGrabber::Grab);
		inputComponent->BindAction("Grab", EInputEvent::IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing input component"), *GetOwner()->GetName());
	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Log, TEXT("Grab pressed"));

	/// Try and reach any actors with physics body collision channel set
	auto hitResult = GetFirstPhysicsBodyInReach();
	auto componentToGrab = hitResult.GetComponent();
	auto actorHit = hitResult.GetActor();

	/// If we hit something, then we attach a physics handle
	if (actorHit)
	{
		physicsHandle->GrabComponent(
			componentToGrab,
			NAME_None,
			componentToGrab->GetOwner()->GetActorLocation(),
			true // allow rotation
		);
	}
}

void UGrabber::Release()
{
	physicsHandle->ReleaseComponent();
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	// Get player view point this tick
	FVector playerViewPointLocation;
	FRotator playerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(playerViewPointLocation, playerViewPointRotation);


	FVector lineTraceEnd = playerViewPointLocation + playerViewPointRotation.Vector() * reach;

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

	return hit;
}