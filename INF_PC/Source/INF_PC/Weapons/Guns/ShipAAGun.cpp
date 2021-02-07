// Fill out your copyright notice in the Description page of Project Settings.


#include "INF_PC/Weapons/Guns/ShipAAGun.h"
#include "Camera/CameraComponent.h"

#include "INF_PC/INF_PC.h"

AShipAAGun::AShipAAGun()
{
	LeftBarrelSocketName = "Barrel_Left";
	RightBarrelSocketName = "Barrel_Right";
	
	TurretBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretBase"));
	TurretSupport = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretSupport"));
	BarrelLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BarrelLeft"));
	BarrelRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BarrelRight"));
	MainGunCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MainGunCamera"));

	TurretBase->SetupAttachment(WeaponMainBody);
	TurretSupport->SetupAttachment(TurretBase);
	BarrelLeft->SetupAttachment(TurretSupport, LeftBarrelSocketName);
	BarrelRight->SetupAttachment(TurretSupport, RightBarrelSocketName);
	MainGunCamera->SetupAttachment(TurretSupport);
}
