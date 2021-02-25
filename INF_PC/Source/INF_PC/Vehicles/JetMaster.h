// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <INF_PC/Vehicles/VehicleMaster.h>
#include <INF_PC/Components/JetMovementComponent.h>

#include "CockpitMaster.h"

#include "JetMaster.generated.h"

/**
 * 
 */
UCLASS()
class INF_PC_API AJetMaster : public AVehicleMaster
{
	GENERATED_BODY()

private:
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
	UPROPERTY(VisibleDefaultsOnly, Category="Vehicle Setup|Weapons")
	FName LeftGunAttachSocketName;
	UPROPERTY(VisibleDefaultsOnly, Category="Vehicle Setup|Weapons")
	FName RightGunAttachSocketName;
	

public:
	AJetMaster();
	virtual FVector GetPawnViewLocation() const override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	ACockpitMaster* GetCockpit() const { return Cast<class ACockpitMaster>(Cockpit->GetChildActor()); }

	int32 GetMaxRockets() const { return MaxRockets; }
	int32 GetRocketsAvailable() const { return RocketsAvailable; }
	void SetMaxRockets(const int32 Value) { MaxRockets = Value; RocketsAvailable = MaxRockets; }
	void UpdateRocketsAvailable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called when 'W' or 'S' keys are pressed on the Jet.
	virtual void ThrustVehicle(float Value) override;

	// Called when the 'A' or 'D' keys are pressed to Yaw the vehicle.
	virtual void YawVehicle(float Value) override;

	// Called when the Mouse is moved up/down to Pitch the Vehicle Up/Down.
	virtual void PitchVehicle(float Value) override;

	// Called when the Mouse is moved left/right to Roll the Vehicle Left/Right.
	virtual void RollVehicle(float Value) override;

	virtual void EnterWeaponOrCockpit() override;
	virtual void ExitWeaponOrCockpit() override;
	virtual void FreeLookOn() override;
	virtual void FreeLookOff() override;
	virtual void FireSelectedWeapon() override;
	virtual void StopFiringSelectedWeapon() override;
	
	UFUNCTION(Server, Reliable, WithValidation)
    virtual void ServerFire();
	UFUNCTION(NetMulticast, Reliable, WithValidation)
    virtual void MulticastFire();
	UFUNCTION(Server, Reliable, WithValidation)
    virtual void ServerStopFire();
	UFUNCTION(NetMulticast, Reliable, WithValidation)
    virtual void MulticastStopFire();

private:
	class UEngine* Engine;

	bool bIntentionalPitch{ false };
	bool bIntentionalRoll{ false };
	bool bRocketMode { false };

	int32 MaxRockets;
	int32 RocketsAvailable;

	UPROPERTY(VisibleAnywhere)
	UJetMovementComponent* JetMovementComponent;

	void ToggleRocketMode();
	
	void SetupJetGuns();
	void SetupJetRockets(int NumRockets);
	void SetupJetBombs();
	void SetupJetTorpedos();
};
