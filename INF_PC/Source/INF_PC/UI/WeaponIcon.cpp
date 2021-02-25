// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponIcon.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

UWeaponIcon::UWeaponIcon(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	const ConstructorHelpers::FObjectFinder<UTexture> Ico_AAGunTemp(TEXT("/Game/_Assets/UI/Ico_AAGun"));
	const ConstructorHelpers::FObjectFinder<UTexture> Ico_BombTemp(TEXT("/Game/_Assets/UI/Ico_Bomb"));
	const ConstructorHelpers::FObjectFinder<UTexture> Ico_CannonTemp(TEXT("/Game/_Assets/UI/Ico_Cannon"));
	const ConstructorHelpers::FObjectFinder<UTexture> Ico_JetCannonTemp(TEXT("/Game/_Assets/UI/Ico_JetCannon"));
	const ConstructorHelpers::FObjectFinder<UTexture> Ico_JetRocketTemp(TEXT("/Game/_Assets/UI/Ico_JetRocket"));
	const ConstructorHelpers::FObjectFinder<UTexture> Ico_MissileTemp(TEXT("/Game/_Assets/UI/Ico_Missile"));
	const ConstructorHelpers::FObjectFinder<UTexture> Ico_RepairTemp(TEXT("/Game/_Assets/UI/Ico_Repair"));
	const ConstructorHelpers::FObjectFinder<UTexture> Ico_TorpedoTemp(TEXT("/Game/_Assets/UI/Ico_Torpedo"));

	if (!ensure(Ico_AAGunTemp.Object != nullptr)) return;
	if (!ensure(Ico_BombTemp.Object != nullptr)) return;
	if (!ensure(Ico_CannonTemp.Object != nullptr)) return;
	if (!ensure(Ico_JetCannonTemp.Object != nullptr)) return;
	if (!ensure(Ico_JetRocketTemp.Object != nullptr)) return;
	if (!ensure(Ico_MissileTemp.Object != nullptr)) return;
	if (!ensure(Ico_RepairTemp.Object != nullptr)) return;
	if (!ensure(Ico_TorpedoTemp.Object != nullptr)) return;

	Ico_AAGun = Ico_AAGunTemp.Object;
	Ico_Bomb = Ico_BombTemp.Object;
	Ico_Cannon = Ico_CannonTemp.Object;
	Ico_JetCannon = Ico_JetCannonTemp.Object;
	Ico_JetRocket = Ico_JetRocketTemp.Object;
	Ico_Missile = Ico_MissileTemp.Object;
	Ico_Repair = Ico_RepairTemp.Object;
	Ico_Torpedo = Ico_TorpedoTemp.Object;
}

void UWeaponIcon::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);
	this->RemoveFromViewport();
}

void UWeaponIcon::SetWeaponIcon(EWeaponType Weapon)
{
	FText AmmoText;
	
	switch (Weapon)
	{
		case EWeaponType::ShipAAGun:
			PBar_Weapon->WidgetStyle.BackgroundImage.SetResourceObject(Ico_AAGun);
			break;
		case EWeaponType::ShipCannon:
			PBar_Weapon->WidgetStyle.BackgroundImage.SetResourceObject(Ico_Cannon);
			break;
		case EWeaponType::Missile:
			PBar_Weapon->WidgetStyle.BackgroundImage.SetResourceObject(Ico_Missile);
			break;
		case EWeaponType::ShipTorpedo:
			PBar_Weapon->WidgetStyle.BackgroundImage.SetResourceObject(Ico_Torpedo);
			break;
		case EWeaponType::JetGun:
			PBar_Weapon->WidgetStyle.BackgroundImage.SetResourceObject(Ico_JetCannon);
			TxtAmmoCount->SetText(AmmoText.FromString(""));
			break;
		case EWeaponType::JetRocket:
			PBar_Weapon->WidgetStyle.BackgroundImage.SetResourceObject(Ico_JetRocket);
			TxtAmmoCount->SetText(AmmoText.FromString("8"));
			break;
		case EWeaponType::BirdBomb:
			PBar_Weapon->WidgetStyle.BackgroundImage.SetResourceObject(Ico_Bomb);
			break;
		case EWeaponType::BirdTorpedo:
			PBar_Weapon->WidgetStyle.BackgroundImage.SetResourceObject(Ico_Torpedo);
			break;
		case EWeaponType::AAGunFourBarrel:
			PBar_Weapon->WidgetStyle.BackgroundImage.SetResourceObject(Ico_AAGun);
			break;
		case EWeaponType::AAGunSingleBarrel:
			PBar_Weapon->WidgetStyle.BackgroundImage.SetResourceObject(Ico_AAGun);
			break;
	}
}
