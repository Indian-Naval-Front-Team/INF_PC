// Fill out your copyright notice in the Description page of Project Settings.


#include "VehicleMaster.h"

#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "Chaos/AABBTree.h"
#include "Components/ArrowComponent.h"
#include "Components/InputComponent.h"
#include "Engine/DemoNetDriver.h"
#include "GameFramework/Controller.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "INF_PC/Components/HealthComponent.h"
#include "INF_PC/Components/RepairComponent.h"
#include "INF_PC/Framework/INFPlayerState.h"
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
	SetRootComponent(VehicleBody);
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(VehicleBody);
	MainCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
	MainCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	CrosshairWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("CrosshairWidget"));
	CrosshairWidget->SetupAttachment(VehicleBody);
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	VehicleFiringRefPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Firing Ref Point"));
	VehicleFiringRefPoint->SetupAttachment(VehicleBody);
	
	VehicleMovementComponent = CreateDefaultSubobject<UMovementComponentMaster>(TEXT("VehicleMovementComponent"));
	NetworkingComponent = CreateDefaultSubobject<UNetworkingComponent>(TEXT("NetworkingComponent"));

	CameraBoom->TargetArmLength = 300.0f;
}

// Called when the game starts or when spawned
void AVehicleMaster::BeginPlay()
{
	Super::BeginPlay();
	
	//UE_LOG(LogTemp, Warning, TEXT("VehicleName = %s"), *GetName());
	PlayerStateRef = Cast<AINFPlayerState>(GetPlayerState());
	OriginalCameraBoomTransform = CameraBoom->GetRelativeTransform();
	
	if (HasAuthority())
	{
		// TODO : Change this value to 30.0f or something higher like that while Publishing.
		//NetUpdateFrequency = 5.0f;	// 30.0f while publishing
	}

	if (PlayerStateRef)
	{
		PlayerStateRef->SetCurrentPlayerStatus(EPlayerStatus::W_MainVehicleView);
	}
}

void AVehicleMaster::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AVehicleMaster, LeftGun);
	DOREPLIFETIME(AVehicleMaster, RightGun);
	DOREPLIFETIME(AVehicleMaster, JetRockets);
}

// Called every frame
void AVehicleMaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//DrawDebugString(GetWorld(), FVector(0.0f, 300.0f, 300.0f), UEnum::GetValueAsString(GetLocalRole()), this, FColor::Black, DeltaTime, false, 2.0f);
}

// Called to bind functionality to input
void AVehicleMaster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("Thrust", this, &AVehicleMaster::ThrustVehicle);
	PlayerInputComponent->BindAxis("Yaw/Azimuth", this, &AVehicleMaster::YawVehicle);
	PlayerInputComponent->BindAxis("Pitch/Elevate", this, &AVehicleMaster::PitchVehicle);
	PlayerInputComponent->BindAxis("Roll", this, &AVehicleMaster::RollVehicle);
	
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AVehicleMaster::FireSelectedWeapon);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AVehicleMaster::StopFiringSelectedWeapon);

	PlayerInputComponent->BindAction("ZoomIn", IE_Pressed, this, &AVehicleMaster::EnterWeaponOrCockpit);
	PlayerInputComponent->BindAction("ZoomOut", IE_Pressed, this, &AVehicleMaster::ExitWeaponOrCockpit);

	PlayerInputComponent->BindAction("FreeLook", IE_Pressed, this, &AVehicleMaster::FreeLookOn);
	PlayerInputComponent->BindAction("FreeLook", IE_Released, this, &AVehicleMaster::FreeLookOff);
}
