// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponMaster.h"
#include "Camera/CameraComponent.h"
#include "Engine/DemoNetDriver.h"
#include "GameFramework/SpringArmComponent.h"
#include "INF_PC/Projectiles/ProjectileMaster.h"
#include "INF_PC/Vehicles/VehicleMaster.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AWeaponMaster::AWeaponMaster()
{
	SetActorTickEnabled(false);
	
	WeaponMainBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMainBody"));
	RootComponent = WeaponMainBody;

	NetUpdateFrequency = 66.0f;
	MinNetUpdateFrequency = 33.0f;

	WeaponMuzzleSocketName = "MuzzlePos";
}

void AWeaponMaster::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWeaponMaster, Projectile);
	DOREPLIFETIME(AWeaponMaster, WeaponOwner);
}

// Called when the game starts or when spawned
void AWeaponMaster::BeginPlay()
{
	Super::BeginPlay();
	WeaponOwner = Cast<AVehicleMaster>(GetOwner());
}
