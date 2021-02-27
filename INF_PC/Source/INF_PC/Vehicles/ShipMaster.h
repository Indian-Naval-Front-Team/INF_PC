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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class URepairComponent* RepairComponent;
	
	void SetupAAGuns();
	void SetupCannons();
	void SetupMissiles(int NumMissiles);
	void SetupShipTorpedos();

	TArray<AWeaponMaster*> AAGuns;
	TArray<AWeaponMaster*> Cannons;
	TArray<AWeaponMaster*> MissilePods;
	TArray<AWeaponMaster*> TorpedoTubes;

public:
	AShipMaster();

	TArray<AWeaponMaster*> GetAAGuns() const { return AAGuns; }
	TArray<AWeaponMaster*> GetCannons() const { return Cannons; }
	TArray<AWeaponMaster*> GetMissilePods() const { return MissilePods; }
	TArray<AWeaponMaster*> GetTorpedoTubes() const { return TorpedoTubes; }
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	virtual void ThrustVehicle(float Value) override;
	virtual void YawVehicle(float Value) override;
	virtual void PitchVehicle(float Value) override;
	virtual void RollVehicle(float Value) override;
	
	virtual void EnterWeaponOrCockpit() override;
	virtual void ExitWeaponOrCockpit() override;
	virtual void FireSelectedWeapon() override;
	virtual void StopFiringSelectedWeapon() override;
	
	UFUNCTION(Server, Reliable, WithValidation)
    virtual void ServerFire();
	UFUNCTION(Server, Reliable, WithValidation)
	virtual void ServerStopFire();
	UFUNCTION(NetMulticast, Reliable, WithValidation)
    virtual void MulticastFire();
	UFUNCTION(NetMulticast, Reliable, WithValidation)
    virtual void MulticastStopFire();
};
