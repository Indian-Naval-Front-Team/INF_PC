// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "INF_PC/Weapons/WeaponMaster.h"

#include "WeaponIcon.generated.h"
/**
 * 
 */
UCLASS()
class INF_PC_API UWeaponIcon : public UUserWidget
{
	GENERATED_BODY()

public:
	UWeaponIcon(const FObjectInitializer& ObjectInitializer);
	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;

	UFUNCTION(BlueprintCallable)
	void SetWeaponIcon(EWeaponType Weapon);

private:
	class UTexture* Ico_AAGun;
	class UTexture* Ico_Bomb;
	class UTexture* Ico_Cannon;
	class UTexture* Ico_JetCannon;
	class UTexture* Ico_JetRocket;
	class UTexture* Ico_Missile;
	class UTexture* Ico_Repair;
	class UTexture* Ico_Torpedo;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TxtAmmoCount;
	// UPROPERTY(meta = (BindWidget))
	// class USizeBox* SizeBox_IconContainer;
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* PBar_Weapon;

};