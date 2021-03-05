// Fill out your copyright notice in the Description page of Project Settings.


#include "Compass.h"

#include "INF_PC/Vehicles/VehicleMaster.h"

void UCompass::NativeConstruct()
{
	Super::NativeConstruct();
	
	PlayerVehicle = Cast<AVehicleMaster>(GetOwningPlayer()->GetPawn());

	if (PlayerVehicle)
	{
		UE_LOG(LogTemp, Warning, TEXT("Vehicle = %s"), *PlayerVehicle->GetName());
	}
}
