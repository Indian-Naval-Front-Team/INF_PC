// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VehicleMaster.h"
#include "ShipMaster.generated.h"

/**
 * 
 */
UCLASS()
class INF_PC_API AShipMaster : public AVehicleMaster
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ExtraObjects;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UShipMovementComponent* ShipMovementComponent;

public:
	AShipMaster();
};
