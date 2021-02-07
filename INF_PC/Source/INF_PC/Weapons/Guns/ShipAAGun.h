// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "INF_PC/Weapons/WeaponMaster.h"
#include "ShipAAGun.generated.h"

/**
 * 
 */
UCLASS()
class INF_PC_API AShipAAGun : public AWeaponMaster
{
	GENERATED_BODY()

public:
	AShipAAGun();
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapons|ShipAAGun|Components")
	class UStaticMeshComponent* TurretBase;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapons|ShipAAGun|Components")
	class UStaticMeshComponent* TurretSupport;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapons|ShipAAGun|Components")
	class UStaticMeshComponent* BarrelLeft;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapons|ShipAAGun|Components")
	class UStaticMeshComponent* BarrelRight;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapons|ShipAAGun|Components")
	class UCameraComponent* MainGunCamera;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Weapons|ShipAAGun|Setup")
	FName LeftBarrelSocketName;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Weapons|ShipAAGun|Setup")
	FName RightBarrelSocketName;
};
