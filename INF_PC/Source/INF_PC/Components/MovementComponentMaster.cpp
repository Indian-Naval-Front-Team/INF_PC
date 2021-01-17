// Fill out your copyright notice in the Description page of Project Settings.


#include "MovementComponentMaster.h"

// Sets default values for this component's properties
UMovementComponentMaster::UMovementComponentMaster()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UMovementComponentMaster::BeginPlay()
{
	Super::BeginPlay();
	TopSpeedInKms = TopSpeedInKms * 28.0f;
}


FVehicleMove UMovementComponentMaster::CreateMove(float DeltaTime)
{
	FVehicleMove Move;
	return Move;
}

// Called every frame
void UMovementComponentMaster::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

