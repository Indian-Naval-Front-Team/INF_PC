// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "INF_PC/Weapons/WeaponMaster.h"
#include "JetGun.generated.h"

/**
 * 
 */
UCLASS()
class INF_PC_API AJetGun : public AWeaponMaster
{
	GENERATED_BODY()

public:
	AJetGun();

	virtual void StartFire() override;
	virtual void StopFire() override;
	
protected:
	virtual void BeginPlay() override;
	virtual void Fire() override;
	
	UFUNCTION(Server, Reliable, WithValidation)
    virtual void ServerFire();
};
