// Fill out your copyright notice in the Description page of Project Settings.


#include "ShipMaster.h"

#include "INF_PC/Components/ShipMovementComponent.h"

AShipMaster::AShipMaster()
{
	ExtraObjects = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ship Extra Objects"));
	ExtraObjects->SetupAttachment(VehicleBody);
	ShipMovementComponent = CreateDefaultSubobject<UShipMovementComponent>(TEXT("Ship Movement Component"));
	
	SetReplicates(true);
	//bReplicates = true;
	SetReplicateMovement(false);
	ShipMovementComponent->SetIsReplicated(true);
}
