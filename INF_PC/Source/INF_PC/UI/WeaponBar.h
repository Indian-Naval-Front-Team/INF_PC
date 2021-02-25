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
	UFUNCTION(BlueprintImplementableEvent)
	void SetupWeaponBar(const TMap<EWeaponType, FWeaponSetup> & WeaponTableRef);
	UFUNCTION(BlueprintCallable)
	void SetWeaponTable(const TMap<EWeaponType, FWeaponSetup> & WeaponTableRef) { WeaponTable = WeaponTableRef; }
	UFUNCTION(BlueprintCallable)
	TMap<EWeaponType, FWeaponSetup> GetWeaponTable() const { return WeaponTable; }

private:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	class UHorizontalBox* HBox_WeaponContainer;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TMap<EWeaponType, FWeaponSetup> WeaponTable;

protected:
	virtual bool Initialize() override;
};
