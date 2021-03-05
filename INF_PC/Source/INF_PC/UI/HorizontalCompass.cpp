// Fill out your copyright notice in the Description page of Project Settings.


#include "HorizontalCompass.h"


#include "Components/Image.h"
#include "INF_PC/Vehicles/VehicleMaster.h"

void UHorizontalCompass::NativeConstruct()
{
	Super::NativeConstruct();
}

void UHorizontalCompass::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	SetRotation();
}

void UHorizontalCompass::SetRotation()
{
	if (PlayerVehicle)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Yaw = %f"), PlayerVehicle->GetCameraRotation().Yaw * -10.0f);
		GetCompassImage()->SetRenderTranslation(FVector2D(PlayerVehicle->GetCameraRotation().Yaw * -10.0f, 0.0f));
	}
}
