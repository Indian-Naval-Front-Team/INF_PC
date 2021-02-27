// Fill out your copyright notice in the Description page of Project Settings.


#include "MainHUD.h"


#include "WeaponBar.h"
#include "INF_PC/Framework/INFPlayerController.h"
#include "INF_PC/Vehicles/VehicleMaster.h"


bool UMainHUD::Initialize()
{
	const bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(WeaponBar != nullptr)) return false;
	
	return true;
}

void UMainHUD::PopulateWeaponsTable(class AVehicleMaster* Vehicle)
{
	//for (const TPair<EWeaponType, FWeaponSetup>& Pair : Vehicle->GetVehicleArsenal())
	for(FWeaponSetup Weapon : Vehicle->GetVehicleArsenal())
	{
		switch (Weapon.WeaponType)
		{
			case EWeaponType::ShipAAGun: break;
			case EWeaponType::ShipCannon: break;
			case EWeaponType::Missile: break;
			case EWeaponType::ShipTorpedo: break;
			case EWeaponType::JetGun: break;
			case EWeaponType::JetRocket: break;
			case EWeaponType::BirdBomb: break;
			case EWeaponType::BirdTorpedo: break;
			case EWeaponType::AAGunSingleBarrel: break;
			case EWeaponType::AAGunFourBarrel: break;
			default: ;
		}
	}
}

void UMainHUD::SetupMainHUD(APawn* Vehicle)
{
	AVehicleMaster* SelectedVehicle = Cast<AVehicleMaster>(Vehicle);

	if (SelectedVehicle == nullptr)
	{
		return;
	}
	
	this->AddToViewport();
	
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	PlayerControllerRef = Cast<AINFPlayerController>(World->GetFirstPlayerController());
	if (!ensure(PlayerControllerRef != nullptr)) return;

	// FInputModeUIOnly InputModeData;
	// InputModeData.SetWidgetToFocus(this->TakeWidget());
	// InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	//
	// PlayerControllerRef->SetInputMode(InputModeData);

	PlayerControllerRef->bShowMouseCursor = false;
	WeaponBar->SetupWeaponBar(SelectedVehicle->GetVehicleArsenal(), SelectedVehicle->GetIsVehicleRepairable());
}
