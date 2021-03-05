// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "INF_PC/Weapons/WeaponMaster.h"

#include "WeaponBar.generated.h"

/**
 * 
 */
UCLASS()
class INF_PC_API UWeaponBar : public UUserWidget
{
	GENERATED_BODY()

public:
	// UFUNCTION(BlueprintImplementableEvent)
	// void SetupWeaponBar(const TMap<EWeaponType, FWeaponSetup> & WeaponTableRef);
	UFUNCTION(BlueprintImplementableEvent)
    void SetupWeaponBar(const TArray<FWeaponSetup> & Arsenal, bool bIsRepairable, class AVehicleMaster* SelectedVehicleRef);
	UFUNCTION(BlueprintCallable)
	void SetWeaponTable(const TArray<FWeaponSetup> & ArsenalRef) { VehicleArsenal = ArsenalRef; }
	UFUNCTION(BlueprintCallable)
	TArray<FWeaponSetup> GetArsenal() const { return VehicleArsenal; }

private:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	class UHorizontalBox* HBox_WeaponContainer;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<FWeaponSetup> VehicleArsenal;

protected:
	virtual bool Initialize() override;
};
