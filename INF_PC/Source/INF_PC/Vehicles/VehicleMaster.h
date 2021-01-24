// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Net/UnrealNetwork.h"
#include "VehicleMaster.generated.h"

USTRUCT()
struct FPawnMove
{
	GENERATED_BODY()

	FPawnMove() {}

	UPROPERTY()
	float Thrust{ 0.0f };
	UPROPERTY()
	float Yaw{ 0.0f };
	UPROPERTY()
	float Pitch{ 0.0f };
	UPROPERTY()
	float Roll{ 0.0f };

	UPROPERTY()
	float DeltaTime{ 0.0f };
	uint8_t MoveIndex{ 0 };
};

USTRUCT()
struct FPawnState
{
	GENERATED_BODY()

public:
	FPawnState() {}

	UPROPERTY()
	FPawnMove LastMove;
	UPROPERTY()
	FVector_NetQuantize Velocity;
	UPROPERTY()
	FTransform PawnTransform;
};

UCLASS()
class INF_PC_API AVehicleMaster : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AVehicleMaster();

protected:
												// Variables
										// Vehicle Setup/Constants
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Setup|Movement|Constants")
	float TopSpeedInKms{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Setup|Movement|Constants")
	float MassInKgs{ 1000.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Setup|Movement|Constants")
	float DragCoefficient{ 0.4f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Setup|Movement|Constants")
	float MaxThrustSpeed{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Setup|Movement|Constants")
	float MinThrustRequired{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Setup|Movement|Constants")
	float ThrustMultiplier{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Setup|Movement|Constants")
	float Gravity{ 981.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Setup|Movement|Constants")
	float YawRate{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Setup|Movement|Constants")
	float PitchRate{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Setup|Movement|Constants")
	float RollRate{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Setup|Movement|Constants")
	float RotMultiplier{ 20.0f };
									// Vehicle Setup/Constants/Control Surfaces
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Setup|Movement|Constants|Control Surfaces")
	float MaxFlapPitch{ 10.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Setup|Movement|Constants|Control Surfaces")
	float MaxElevatorPitch{ 25.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Setup|Movement|Constants|Control Surfaces")
	float MaxAileronPitch{ 45.0f };
									// Vehicle Setup/Constants/Dynamic Vars
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vehicle Setup|Movement|Dynamic Vars")
	float ThrustSpeed{ 0.0f };
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vehicle Setup|Movement|Dynamic Vars")
	float CurrentSpeed{ 0.0f };
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vehicle Setup|Movement|Dynamic Vars")
	float AppliedGravity{ 0.0f };
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vehicle Setup|Movement|Dynamic Vars")
	FVector_NetQuantize NewVehiclePosition;
									// Vehicle Setup/Constants/Rotation
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vehicle Setup|Movement|Rotation")
	float TargetYaw{ 0.0f };
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vehicle Setup|Movement|Rotation")
	float CurrentYaw{ 0.0f };
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vehicle Setup|Movement|Rotation")
	float TargetPitch{ 0.0f };
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vehicle Setup|Movement|Rotation")
	float CurrentPitch{ 0.0f };
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vehicle Setup|Movement|Rotation")
	float TargetRoll{ 0.0f };
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vehicle Setup|Movement|Rotation")
	float CurrentRoll{ 0.0f };

	UPROPERTY(ReplicatedUsing = OnRep_ServerState)
	FPawnState ServerState;

	float Thrust{ 0.0f };
	float Yaw{ 0.0f };
	float Pitch{ 0.0f };
	float Roll{ 0.0f };

	UPROPERTY()
	FVector_NetQuantize Translation;
	UPROPERTY()
	FQuat QuatRot;

	FVector_NetQuantize Velocity;
	FVector_NetQuantize Force{ FVector_NetQuantize::ZeroVector };
	FVector_NetQuantize Acceleration{ FVector_NetQuantize::ZeroVector };
	FRotator NewRotation;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* VehicleBody;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* MainCamera;

	// Called when 'W' or 'S' keys are pressed on the Vehicle.
	UFUNCTION()
	virtual void ThrustVehicle(float Value) {};
	// Called when the 'A' or 'D' keys are pressed to Yaw the vehicle.
	UFUNCTION()
	virtual void YawVehicle(float Value) {};
	// Called when the Mouse is moved up/down to Pitch the Vehicle Up/Down.
	UFUNCTION()
	virtual void PitchVehicle(float Value) {};
	// Called when the Mouse is moved left/right to Roll the Vehicle Left/Right.
	UFUNCTION()
	virtual void RollVehicle(float Value) {};
	// Get the Air Resistance of the vehicle using the formula(-velocityNormal * velocity^2 * dragCoefficient).
	UFUNCTION()
	virtual FVector_NetQuantize GetVehicleAirResistance() { return FVector_NetQuantize::ZeroVector; }
	// Get the Rolling Resistance of the vehicle using the formula(-velocityNormal * rollingResistanceCoefficient * normalForce).
	UFUNCTION()
	virtual FVector_NetQuantize GetVehicleRollingResistance() { return FVector_NetQuantize::ZeroVector; }
	
	UFUNCTION()
	virtual void UpdateVehiclePosition(float DeltaTime) {};
	UFUNCTION()
	virtual void UpdateVehicleRotation(float DeltaTime, float YawVal, float PitchVal, float RollVal) {};
	UFUNCTION()
	virtual void OnRep_ServerState() {};

	FPawnMove VehicleMove;
	virtual void SimulateMove(FPawnMove PawnMove) {};

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	static FString GetRoleText(ENetRole role);
};
