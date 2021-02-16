// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MovementComponentMaster.h"
#include "JetMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class INF_PC_API UJetMovementComponent : public UMovementComponentMaster
{
	GENERATED_BODY()

public:
	UJetMovementComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Constants")
	float PitchRate{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Constants")
	float RollRate{ 0.0f };

	// Vehicle Setup/Constants/Control Surfaces
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Constants|Control Surfaces")
	float MaxFlapPitch{ 10.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Constants|Control Surfaces")
	float MaxElevatorPitch{ 25.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Constants|Control Surfaces")
	float MaxAileronPitch{ 45.0f };

	virtual FVector GetVehicleAirResistance() override;
	virtual FVector GetVehicleRollingResistance() override;
	virtual void UpdateVehiclePosition(float DeltaTime) override;
	virtual void UpdateVehicleRotation(float DeltaTime, float PitchVal, float YawVal, float RollVal) override;

	float Pitch{ 0.0f };
	float Roll{ 0.0f };
	float TargetPitch{ 0.0f };
	float CurrentPitch{ 0.0f };
	float TargetRoll{ 0.0f };
	float CurrentRoll{ 0.0f };
	float AppliedGravity{ 0.0f };

public:
	virtual void SimulateMove(const FVehicleMove& Move) override;
	virtual FVehicleMove CreateMove(float DeltaTime) override;

	void SetYaw(float Value) { Yaw = Value; }
	float GetYaw() const { return Yaw; }
	void SetPitch(float Value) { Pitch = Value; }
	float GetPitch() const { return Pitch; }
	void SetRoll(float Value) { Roll = Value; }
	float GetRoll() const { return Roll; }
	float GetRollRate() const { return RollRate; }
	float GetPitchRate() const { return PitchRate; }
	float GetYawRate() const { return YawRate; }
	float GetRotMultiplier() const { return RotMultiplier; }
};
