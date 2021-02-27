// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "MovementComponentMaster.h"
#include "ShipMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class INF_PC_API UShipMovementComponent : public UMovementComponentMaster
{
	GENERATED_BODY()

public:
	UShipMovementComponent();
	
protected:
	virtual void BeginPlay() override;

	virtual FVector GetVehicleAirResistance() override;
	virtual FVector GetVehicleRollingResistance() override;
	virtual void UpdateVehiclePosition(float DeltaTime) override;
	virtual void UpdateVehicleRotation(float DeltaTime, float PitchVal, float YawVal, float RollVal) override;

public:
	virtual void SimulateMove(const FVehicleMove& Move) override;
	virtual FVehicleMove CreateMove(float DeltaTime) override;

	void SetYaw(float Value) { Yaw = Value; }
	float GetYaw() const { return Yaw; }
	float GetYawRate() const { return YawRate; }
	float GetRotMultiplier() const { return RotMultiplier; }
};
