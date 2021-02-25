// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBar.h"



#include "WeaponIcon.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "INF_PC/Vehicles/VehicleMaster.h"


bool UWeaponBar::Initialize()
{
	const bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(HBox_WeaponContainer != nullptr)) return false;
	
	return true;
}

// void UWeaponBar::SetupWeaponBar(AVehicleMaster* Vehicle)
// {
// 	if (HBox_WeaponContainer && Vehicle)
// 	{
// 		const int32 NumWeapons = Vehicle->GetWeaponTable().Num();
//
// 		if (NumWeapons > 0)
// 		{
// 			for (int32 i = 0; i < NumWeapons; i++)
// 			{
// 				UWeaponIcon* WeaponIcon = CreateWidget<UWeaponIcon>(GetWorld(), UWeaponIcon::StaticClass());
// 				
// 				if (!ensure(WeaponIcon != nullptr))
// 				{
// 					return;
// 				}
// 				
// 				HBox_WeaponContainer->AddChildToHorizontalBox(WeaponIcon);
// 			}
// 		}
// 	}
//
// 	UE_LOG(LogTemp, Warning, TEXT("Child Count = %d"), HBox_WeaponContainer->GetChildrenCount());
// }
