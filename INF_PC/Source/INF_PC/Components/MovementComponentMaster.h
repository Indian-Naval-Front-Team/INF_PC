// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MovementComponentMaster.generated.h"


USTRUCT()
struct FVehicleMove
{
	GENERATED_BODY()

	FVehicleMove() {}

	UPROPERTY()
	float Thrust;
	UPROPERTY()
	float Yaw;
	UPROPERTY()
	float Pitch;
	UPROPERTY()
	float Roll;
	UPROPERTY()
	float DeltaTime;
	UPROPERTY()
	float TimeStamp;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INF_PC_API UMovementComponentMaster : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMovementComponentMaster();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Variables

// Vehicle Setup/Constants
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Constants")
		float TopSpeedInKms{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Constants")
		float MassInKgs{ 1000.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Constants")
		float DragCoefficient{ 0.4f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Constants")
		float MaxThrustSpeed{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Constants")
		float MinThrustRequired{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Constants")
		float ThrustMultiplier{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Constants")
		float Gravity{ 981.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Constants")
		float YawRate{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Constants")
		float RotMultiplier{ 20.0f };

	// Update Vehicle's Position on the Client.
	UFUNCTION()
		virtual void UpdateVehiclePosition(float DeltaTime) {};
	// Update Vehicle's Rotation on the Client.
	UFUNCTION()
		virtual void UpdateVehicleRotation(float DeltaTime, float PitchVal, float YawVal, float RollVal) {};
	// Get the Air Resistance of the vehicle using the formula(-velocityNormal * velocity^2 * dragCoefficient).
	UFUNCTION()
		virtual FVector GetVehicleAirResistance() { return FVector::ZeroVector; }
	// Get the Rolling Resistance of the vehicle using the formula(-velocityNormal * rollingResistanceCoefficient * normalForce).
	UFUNCTION()
		virtual FVector GetVehicleRollingResistance() { return FVector::ZeroVector; }

	FVector Velocity;
	float Thrust{ 0.0f };
	float Yaw{ 0.0f };

	float ThrustSpeed{ 0.0f };
	float CurrentSpeed{ 0.0f };
	FVector NewVehiclePosition{ FVector::ZeroVector };
	float TargetYaw{ 0.0f };
	float CurrentYaw{ 0.0f };
	FVector Translation;
	FQuat QuatRot;
	FVector Force{ FVector::ZeroVector };
	FVector Acceleration{ FVector::ZeroVector };

	const APawn* OwningPawn;
	FVehicleMove LastMove;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void SimulateMove(const FVehicleMove& Move) {};
	virtual FVehicleMove CreateMove(float DeltaTime);

	void SetVelocity(FVector Value) { Velocity = Value; }
	FVector GetVelocity() { return Velocity; }
	void SetThrust(float Value) { Thrust = Value; }
	float GetThrust() { return Thrust; }
	float GetThrustMultipliyer() { return ThrustMultiplier; }
	FVehicleMove GetLastMove() { return LastMove; }
};
