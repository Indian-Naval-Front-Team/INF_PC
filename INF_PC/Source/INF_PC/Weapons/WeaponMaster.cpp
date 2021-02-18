// Fill out your copyright notice in the Description page of Project Settings.


#include "INF_PC/Weapons/WeaponMaster.h"
#include "Engine/DemoNetDriver.h"
#include "INF_PC/Vehicles/VehicleMaster.h"
#include "INF_PC/Components/MovementComponentMaster.h"
#include "INF_PC/Components/JetMovementComponent.h"

// Sets default values
AWeaponMaster::AWeaponMaster()
{
	SetActorTickEnabled(false);
	
	WeaponMainBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMainBody"));
	RootComponent = WeaponMainBody;

	// NetUpdateFrequency = 66.0f;
	// MinNetUpdateFrequency = 33.0f;

	WeaponMuzzleSocketName = "MuzzlePos";
}

void AWeaponMaster::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWeaponMaster, Projectile);
	DOREPLIFETIME(AWeaponMaster, WeaponOwner);
}

void AWeaponMaster::SetWeaponOwner(AVehicleMaster* VehicleRef)
{
	WeaponOwner = VehicleRef;
	MovementComponentOfOwner = Cast<UMovementComponentMaster>(WeaponOwner->GetComponentByClass(UMovementComponentMaster::StaticClass()));

	SetMovementComponentOfOwner();
}

// Called when the game starts or when spawned
void AWeaponMaster::BeginPlay()
{
	Super::BeginPlay();
}

void AWeaponMaster::SetMovementComponentOfOwner()
{
	if (MovementComponentOfOwner)
	{
		switch (MovementComponentOfOwner->GetVehicleType())
		{
		case VehicleType::Ship:
			break;
		case VehicleType::Jet:
			MovementComponentOfOwner = GetOwner()->FindComponentByClass<UJetMovementComponent>();
			break;
		case VehicleType::Chopper:
			break;
		case VehicleType::Others:
			MovementComponentOfOwner = GetOwner()->FindComponentByClass<UMovementComponentMaster>();
			break;
		default:
			break;
		}	
	}
}

