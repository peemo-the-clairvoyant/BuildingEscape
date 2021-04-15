// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOpenRequest);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCloseRequest);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void OpenDoor();
	void CloseDoor();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable)
		FOnOpenRequest onOpenRequest;

	UPROPERTY(BlueprintAssignable)
		FOnCloseRequest onCloseRequest;

private:
	UPROPERTY(EditAnywhere)
		float openAngle = -30.f;

	UPROPERTY(EditAnywhere)
		float doorCloseDelay = 1.f;

	float lastDoorOpenTime;

	UPROPERTY(EditAnywhere)
		ATriggerVolume *pressurePlate = nullptr;

	UPROPERTY(EditAnywhere)
		float triggerMass = 5.f;

	AActor* owner = nullptr; // the owning door

	const float GetTotalMassOfActorsOnPlate() const;
};
