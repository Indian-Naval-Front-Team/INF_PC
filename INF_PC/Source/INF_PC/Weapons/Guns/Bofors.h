// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "INF_PC/Weapons/WeaponMaster.h"

#include "Bofors.generated.h"

/**
 * 
 */
UCLASS()
class INF_PC_API ABofors : public AWeaponMaster
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* Support;
	UPROPERTY(EditDefaultsOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* Reticules;
	UPROPERTY(EditDefaultsOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* Barrel_Left;
	UPROPERTY(EditDefaultsOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* Barrel_Right;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	class USpringArmComponent* CameraBoom;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	class UCameraComponent* MainCamera;

public:
	ABofors();

protected:
	virtual void BeginPlay() override;

	virtual void AzimuthWeapon(float Value) override;
	virtual void ElevateWeapon(float Value) override;
	
	virtual void StartFire() override;
	virtual void StopFire() override;
	virtual void Fire() override;
	
	UFUNCTION(Server, Reliable, WithValidation)
    virtual void ServerFire();
};
