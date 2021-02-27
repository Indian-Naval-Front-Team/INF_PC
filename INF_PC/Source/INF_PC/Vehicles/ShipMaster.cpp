// Fill out your copyright notice in the Description page of Project Settings.


#include "ShipMaster.h"


#include "GameFramework/SpringArmComponent.h"
#include "INF_PC/Components/ShipMovementComponent.h"
#include "INF_PC/Components/RepairComponent.h"
#include "INF_PC/Framework/INFPlayerState.h"


AShipMaster::AShipMaster()
{
	ExtraObjects = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ship Extra Objects"));
	ExtraObjects->SetupAttachment(VehicleBody);
	ShipMovementComponent = CreateDefaultSubobject<UShipMovementComponent>(TEXT("Ship Movement Component"));
	RepairComponent = CreateDefaultSubobject<URepairComponent>(TEXT("Repair Component"));
	
	SetReplicates(true);
	//bReplicates = true;
	SetReplicateMovement(false);
	ShipMovementComponent->SetIsReplicated(true);
}

void AShipMaster::BeginPlay()
{
	Super::BeginPlay();

	ShipMovementComponent->SetTopSpeedInKms(ShipMovementComponent->GetTopSpeedInKms() * 28.0f);
	SelectedWeaponType = EWeaponType::ShipAAGun;
	//UE_LOG(LogTemp, Warning, TEXT("Vehicle Body length = %f"), VehicleBody->GetStaticMesh()->ExtendedBounds.BoxExtent.Y);

	if (FindComponentByClass<URepairComponent>())
	{
		bIsVehicleRepairable = true;
	}
	
	if (GetLocalRole() == ROLE_Authority)
	{
		for(FWeaponSetup Weapon : Arsenal)
		{
			switch (Weapon.WeaponType)
			{
				case EWeaponType::ShipAAGun:
					SetupAAGuns();
					// TODO : Hack for testing AA Guns. Remove later.
					SelectedWeapon = AAGuns[0];
					break;
				case EWeaponType::ShipCannon:
					SetupCannons();
					break;
				case EWeaponType::Missile:
					SetupMissiles(Weapon.NumWeapon);
					break;
				case EWeaponType::ShipTorpedo:
					SetupShipTorpedos();
					break;
				case EWeaponType::JetGun:
					if (Weapon.NumWeapon > 0)
					{
						
					}
					break;
				case EWeaponType::JetRocket:
					if (Weapon.NumWeapon > 0)
					{
						
					}
					break;
				case EWeaponType::BirdBomb:
					if (Weapon.NumWeapon > 0)
					{
						
					}
					break;
				case EWeaponType::BirdTorpedo:
					if (Weapon.NumWeapon > 0)
					{
						
					}
					break;
				case EWeaponType::AAGunSingleBarrel: break;
				case EWeaponType::AAGunFourBarrel: break;
				default: ;
			}
		}
	}
}

void AShipMaster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AShipMaster::SetupAAGuns()
{	
	// Spawn the Jet Guns which all the Jets will have.
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FWeaponSetup VehicleArsenalTemp;
	for (FWeaponSetup Weapon : GetVehicleArsenal())
	{
		if (Weapon.WeaponType == EWeaponType::ShipAAGun)
		{
			VehicleArsenalTemp = Weapon;
			break;
		}
	}

	for (int i = 0; i < VehicleArsenalTemp.NumWeapon; i++)
	{
		AWeaponMaster* AAGun = GetWorld()->SpawnActor<AWeaponMaster>(VehicleArsenalTemp.WeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

		if (AAGun)
		{
			AAGun->SetOwner(this);
			AAGun->SetWeaponOwner(this);	
			AAGun->AttachToComponent(VehicleBody, FAttachmentTransformRules::SnapToTargetNotIncludingScale, VehicleArsenalTemp.SocketNames[i]);
			AAGuns.AddUnique(AAGun);
		}
	}
}

void AShipMaster::SetupCannons()
{
}

void AShipMaster::SetupMissiles(int NumMissiles)
{
}

void AShipMaster::SetupShipTorpedos()
{
}

void AShipMaster::ThrustVehicle(float Value)
{
	if (ShipMovementComponent == nullptr)
	{
		return;
	}

	ShipMovementComponent->SetThrust(Value * ShipMovementComponent->GetThrustMultipliyer());
}

void AShipMaster::YawVehicle(float Value)
{
	if (ShipMovementComponent == nullptr)
	{
		return;
	}

	const float TargetYawRate = Value * ShipMovementComponent->GetYawRate();
	ShipMovementComponent->SetYaw(FMath::FInterpTo(ShipMovementComponent->GetYaw(), TargetYawRate, GetWorld()->GetDeltaSeconds(), 2.0f));
}

void AShipMaster::PitchVehicle(float Value)
{
	const float NewPitch = FMath::Clamp(MainCamera->GetRelativeRotation().Pitch + Value, 0.0f, 30.0f);
	MainCamera->SetRelativeRotation(FRotator(NewPitch, MainCamera->GetRelativeRotation().Yaw, MainCamera->GetRelativeRotation().Roll));
}

void AShipMaster::RollVehicle(float Value)
{
	CameraBoom->AddRelativeRotation(FRotator(0.0f, Value, 0.0f));
}

// WEAPON FUNCTIONS

void AShipMaster::EnterWeaponOrCockpit()
{
	if (PlayerStateRef->GetCurrentPlayerStatus() == EPlayerStatus::W_ZoomedInWeapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("Already inside Weapon!!"));
		return;
	}

	if (PlayerStateRef->GetCurrentPlayerStatus() == EPlayerStatus::W_MainVehicleView)
	{
		PlayerStateRef->SetCurrentPlayerStatus(EPlayerStatus::W_ZoomedInWeapon);
		UE_LOG(LogTemp, Warning, TEXT("Inside Weapon!!"));
		GetWorld()->GetFirstPlayerController()->SetViewTarget(SelectedWeapon);
	}
}

void AShipMaster::ExitWeaponOrCockpit()
{
	if (PlayerStateRef->GetCurrentPlayerStatus() == EPlayerStatus::W_MainVehicleView)
	{
		UE_LOG(LogTemp, Warning, TEXT("Already in Main View!!"));
		return;	
	}
	
	if (PlayerStateRef->GetCurrentPlayerStatus() == EPlayerStatus::W_ZoomedInWeapon)
	{
		GetWorld()->GetFirstPlayerController()->SetViewTarget(this);
		PlayerStateRef->SetCurrentPlayerStatus(EPlayerStatus::W_MainVehicleView);
		UE_LOG(LogTemp, Warning, TEXT("Back to Main View!!"));
	}
}

void AShipMaster::FireSelectedWeapon()
{
}

void AShipMaster::StopFiringSelectedWeapon()
{
}

// SERVER FUNCTIONS

void AShipMaster::ServerFire_Implementation()
{
}

bool AShipMaster::ServerFire_Validate()
{
	return true;
}

void AShipMaster::ServerStopFire_Implementation()
{
}

bool AShipMaster::ServerStopFire_Validate()
{
	return true;
}

void AShipMaster::MulticastFire_Implementation()
{
}

bool AShipMaster::MulticastFire_Validate()
{
	return true;
}

void AShipMaster::MulticastStopFire_Implementation()
{
}

bool AShipMaster::MulticastStopFire_Validate()
{
	return true;
}
