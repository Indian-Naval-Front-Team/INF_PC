// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainHUD.generated.h"

/**
 * 
 */
UCLASS()
class INF_PC_API UMainHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	void PopulateWeaponsTable(class AVehicleMaster* Vehicle);

	UFUNCTION(BlueprintCallable)
	void SetupMainHUD(APawn* Vehicle);
	
private:
	UPROPERTY(meta = (BindWidget))
	class UWeaponBar* WeaponBar;

	TArray<class AWeaponMaster*> Weapons;
	class AINFPlayerController* PlayerControllerRef;
	
protected:
	virtual bool Initialize() override;
};
