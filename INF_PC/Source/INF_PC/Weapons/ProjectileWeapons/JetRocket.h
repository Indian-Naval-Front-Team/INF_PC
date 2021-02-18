// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "INF_PC/Weapons/WeaponMaster.h"
#include "JetRocket.generated.h"

/**
 * 
 */
UCLASS()
class INF_PC_API AJetRocket : public AWeaponMaster
{
	GENERATED_BODY()

public:
	AJetRocket();

	void SetOwningJet(class AJetMaster* JetRef) { OwningJet = JetRef; }
	
	virtual void StartFire() override;
	virtual void StopFire() override;
	
protected:
	virtual void BeginPlay() override;

private:
	class AJetMaster* OwningJet;
};
