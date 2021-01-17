// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <INF_PC/Vehicles/VehicleMaster.h>
#include <INF_PC/Components/JetMovementComponent.h>
#include "JetMaster.generated.h"

/**
 * 
 */
UCLASS()
class INF_PC_API AJetMaster : public AVehicleMaster
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Cockpit, meta = (AllowPrivateAccess = "true"))
	class UChildActorComponent* Cockpit;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Wing_Left;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Wing_Right;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Wheel_Front;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Wheel_Right;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Wheel_Left;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* TailHook;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* RightWing_Flap;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* RightWing_Aileron;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* LeftWing_Aileron;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* LeftWing_Flap;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* RearWing_Elevator_Left;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* RearWing_Elevator_Right;

public:
	AJetMaster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Update Transform to every client out there if authority.
	virtual void OnRep_ServerState() override;

	// Called when 'W' or 'S' keys are pressed on the Jet.
	virtual void ThrustVehicle(float Value) override;

	// Called when the 'A' or 'D' keys are pressed to Yaw the vehicle.
	virtual void YawVehicle(float Value) override;

	// Called when the Mouse is moved up/down to Pitch the Vehicle Up/Down.
	virtual void PitchVehicle(float Value) override;

	// Called when the Mouse is moved left/right to Roll the Vehicle Left/Right.
	virtual void RollVehicle(float Value) override;

private:
	class UEngine* Engine;
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SendMove(FVehicleMove Move);

	virtual void ClearAcknowledgedMoves(FVehicleMove LastMove) override;

	bool bIntentionalPitch{ false };
	bool bIntentionalRoll{ false };

	UPROPERTY(VisibleAnywhere)
	UJetMovementComponent* JetMovementComponent;
};
