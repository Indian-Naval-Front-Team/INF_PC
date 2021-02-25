// Fill out your copyright notice in the Description page of Project Settings.


#include "Bofors.h"


#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

ABofors::ABofors()
{
	Support = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Support"));
	Support->SetupAttachment(WeaponMainBody);
	Reticules = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Reticules"));
	Reticules->SetupAttachment(Support);
	Barrel_Left = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Barrel Left"));
	Barrel_Left->SetupAttachment(Support);
	Barrel_Right = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Barrel Right"));
	Barrel_Right->SetupAttachment(Support);
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CameraBoom->SetupAttachment(Support);
	MainCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Main Camera"));
	MainCamera->SetupAttachment(CameraBoom);
}
