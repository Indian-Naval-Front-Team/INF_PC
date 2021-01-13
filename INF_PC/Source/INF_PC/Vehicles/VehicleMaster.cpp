// Fill out your copyright notice in the Description page of Project Settings.


#include "VehicleMaster.h"

#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Engine/DemoNetDriver.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AVehicleMaster::AVehicleMaster()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	//RootComponent = VehicleBody;

	VehicleBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VehicleBody"));
	VehicleBody->SetupAttachment(RootComponent);
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(VehicleBody);
	MainCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
	MainCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	CameraBoom->TargetArmLength = 300.0f;

}

// Called when the game starts or when spawned
void AVehicleMaster::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		// TODO : Change this value to 30.0f or something higher like that while Publishing.
		NetUpdateFrequency = 30.0f;	// 30.0f while publishing
	}
}

FString AVehicleMaster::GetRoleText(ENetRole role)
{
	switch (role)
	{
	case ROLE_None:
		return "None";
	case ROLE_SimulatedProxy:
		return "Simulated Proxy";
	case ROLE_AutonomousProxy:
		return "Autonomous Proxy";
	case ROLE_Authority:
		return "Authority";
	default:
		return "ERROR!";
	}
}

void AVehicleMaster::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(AVehicleMaster, Velocity);
	DOREPLIFETIME(AVehicleMaster, Thrust);
	DOREPLIFETIME(AVehicleMaster, Yaw);
	DOREPLIFETIME(AVehicleMaster, Pitch);
	DOREPLIFETIME(AVehicleMaster, Roll);
	DOREPLIFETIME(AVehicleMaster, Translation);
	DOREPLIFETIME(AVehicleMaster, QuatRot);
	DOREPLIFETIME(AVehicleMaster, ReplicatedTransform);
}

// Called every frame
void AVehicleMaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AVehicleMaster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("Thrust", this, &AVehicleMaster::ThrustVehicle);
	PlayerInputComponent->BindAxis("Yaw/Azimuth", this, &AVehicleMaster::YawVehicle);
	PlayerInputComponent->BindAxis("Pitch/Elevate", this, &AVehicleMaster::PitchVehicle);
	PlayerInputComponent->BindAxis("Roll", this, &AVehicleMaster::RollVehicle);
}

