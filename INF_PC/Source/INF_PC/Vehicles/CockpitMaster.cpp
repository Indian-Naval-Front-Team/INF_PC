// Fill out your copyright notice in the Description page of Project Settings.


#include "INF_PC/Vehicles/CockpitMaster.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ACockpitMaster::ACockpitMaster()
{
	DashboardBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DashboardBase"));
	DashboardBase->SetupAttachment(RootComponent);

	Dashboard = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Dashboard"));
	Dashboard->SetupAttachment(DashboardBase);
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(Dashboard);
	CockpitCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CockpitCamera"));
	CockpitCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
}

// Called when the game starts or when spawned
void ACockpitMaster::BeginPlay()
{
	Super::BeginPlay();
}

