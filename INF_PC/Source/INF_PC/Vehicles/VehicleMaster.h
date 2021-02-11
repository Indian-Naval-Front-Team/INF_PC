// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "INF_PC/Weapons/WeaponMaster.h"

#include "VehicleMaster.generated.h"

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

USTRUCT()
struct FVehicleMoveNew
{
	GENERATED_BODY()

	UPROPERTY()
	FVector_NetQuantize Force;
	UPROPERTY()
	FVector_NetQuantize Velocity;
	UPROPERTY()
	float Yaw;
	UPROPERTY()
	float Pitch;
	UPROPERTY()
	float Roll;
};

USTRUCT()
struct FVehicleState
{
	GENERATED_BODY()

	FVehicleState() {}

	UPROPERTY()
	FVehicleMove LastMove;
	UPROPERTY()
	FVector Velocity;
	UPROPERTY()
	FTransform VehicleTransform;
};

UCLASS()
class INF_PC_API AVehicleMaster : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AVehicleMaster();

protected:
	virtual void BeginPlay() override;

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	class UStaticMeshComponent* VehicleBody;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	class USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	class UCameraComponent* MainCamera;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
	class UWidgetComponent* CrosshairWidget;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
	class UHealthComponent* HealthComponent;
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Vehicle Setup|Weapons")
	TArray<TSubclassOf<AWeaponMaster>> Arsenal;
	
	// Vehicle Setup/Constants/Control Surfaces
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Setup|Movement|Constants|Control Surfaces")
	float MaxFlapPitch{ 10.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Setup|Movement|Constants|Control Surfaces")
	float MaxElevatorPitch{ 25.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Setup|Movement|Constants|Control Surfaces")
	float MaxAileronPitch{ 45.0f };

	// Vehicle Setup/Constants/Dynamic Vars

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Setup|Movement|Dynamic Vars")
	float ThrustSpeed{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Setup|Movement|Dynamic Vars")
	float CurrentSpeed{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Setup|Movement|Dynamic Vars")
	float AppliedGravity{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Setup|Movement|Dynamic Vars")
	FVector NewVehiclePosition
	{
		FVector::ZeroVector
	};

	// Vehicle Setup/Constants/Rotation

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Setup|Movement|Rotation")
	float TargetYaw{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Setup|Movement|Rotation")
	float CurrentYaw{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Setup|Movement|Rotation")
	float TargetPitch{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Setup|Movement|Rotation")
	float CurrentPitch{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Setup|Movement|Rotation")
	float TargetRoll{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Setup|Movement|Rotation")
	float CurrentRoll{ 0.0f };

	// Get the Air Resistance of the vehicle using the formula(-velocityNormal * velocity^2 * dragCoefficient).
	UFUNCTION()
	virtual FVector GetVehicleAirResistance() { return FVector::ZeroVector; }
	// Get the Rolling Resistance of the vehicle using the formula(-velocityNormal * rollingResistanceCoefficient * normalForce).
	UFUNCTION()
	virtual FVector GetVehicleRollingResistance() { return FVector::ZeroVector; }
	// Update Vehicle's Position on the Client.
	UFUNCTION()
	virtual void UpdateVehiclePosition(float DeltaTime) {};
	// Update Vehicle's Rotation on the Client.
	UFUNCTION()
	virtual void UpdateVehicleRotation(float DeltaTime, float PitchVal, float YawVal, float RollVal) {};

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

	virtual void FireSelectedWeapon() {};
	virtual void StopFiringSelectedWeapon() {};

	FVector Velocity;

	UPROPERTY()
	float Thrust{ 0.0f };
	UPROPERTY()
	float Yaw{ 0.0f };
	UPROPERTY()
	float Pitch{ 0.0f };
	UPROPERTY()
	float Roll{ 0.0f };
	UPROPERTY()
	FVector Translation;
	UPROPERTY()
	FQuat QuatRot;
	UPROPERTY(ReplicatedUsing = OnRep_ServerState)
	FVehicleState ServerState;
	UPROPERTY(ReplicatedUsing=OnRep_VehicleMove)
	FVehicleMove VehicleMove;

	TArray<FVehicleMove> UnacknowledgedMoves;

	UPROPERTY(Replicated)
	AWeaponMaster* LeftGun;
	UPROPERTY(Replicated)
	AWeaponMaster* RightGun;

	UFUNCTION()
	virtual void OnRep_ServerState() {};
	UFUNCTION()
	virtual void OnRep_VehicleMove() {};
	virtual void SimulatedProxy_OnRepServerState() {};
	virtual void AutonomousProxy_OnRepServerState() {};

	virtual void SimulateMove(const FVehicleMove& Move) {};
	virtual FVehicleMove CreateMove(float DeltaTime);
	virtual void ClearAcknowledgedMoves(FVehicleMove LastMove) {};

	FVector Force{ FVector::ZeroVector };
	FVector Acceleration{ FVector::ZeroVector };

	float Client_TimeSinceUpdate;
	float Client_TimeBetweenLastUpdates;
	FTransform Client_StartTransform;
	FVector_NetQuantize Client_StartVelocity;

	virtual void ClientTick(float DeltaTime) {};

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FVector GetPawnViewLocation() const override { return FVector::ZeroVector; };
	//static FString GetRoleText(ENetRole role);
	float GetVehicleSpeed() const { return Velocity.Size(); }
};
