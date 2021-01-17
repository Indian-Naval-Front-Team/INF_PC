// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include <INF_PC/Components/MovementComponentMaster.h>
#include "VehicleMaster.generated.h"


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

	UPROPERTY(ReplicatedUsing = OnRep_ServerState)
	FVehicleState ServerState;

	TArray<FVehicleMove> UnacknowledgedMoves;

	UFUNCTION()
	virtual void OnRep_ServerState() {};
	virtual void ClearAcknowledgedMoves(FVehicleMove LastMove) {};

	UPROPERTY()
	UMovementComponentMaster* VehicleMovementComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//static FString GetRoleText(ENetRole role);
};
