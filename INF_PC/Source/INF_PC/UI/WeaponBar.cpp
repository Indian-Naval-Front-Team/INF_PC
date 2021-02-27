// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBar.h"
#include "WeaponIcon.h"


bool UWeaponBar::Initialize()
{
	const bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(HBox_WeaponContainer != nullptr)) return false;
	
	return true;
}
