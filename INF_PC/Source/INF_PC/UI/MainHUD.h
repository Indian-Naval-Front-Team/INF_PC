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

	UFUNCTION(BlueprintCallable)
	class UWeaponBar* GetWeaponBar() const { return WeaponBar; }

	UFUNCTION(BlueprintCallable)
	class UCompass* GetHorizontalCompass() const { return Compass_Horizontal; }

	UFUNCTION(BlueprintCallable)
	class AVehicleMaster* GetSelectedVehicle() const { return SelectedVehicle; }
	
	UFUNCTION(BlueprintCallable)
    class UObjectiveWidget* GetObjectiveWidget() const { return ObjectiveWidget; }
	
private:
	UPROPERTY(meta = (BindWidget))
	class UWeaponBar* WeaponBar;
	UPROPERTY(meta = (BindWidget))
	class UCompass* Compass_Horizontal;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	class URadar* Radar;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	class UObjectiveWidget* ObjectiveWidget;

	TArray<class AWeaponMaster*> Weapons;
	class AINFPlayerController* PlayerControllerRef;
	class AVehicleMaster* SelectedVehicle;
	
protected:
	virtual bool Initialize() override;
};
