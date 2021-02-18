// Fill out your copyright notice in the Description page of Project Settings.


#include "VehicleMaster.h"

#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "Chaos/AABBTree.h"
#include "Components/InputComponent.h"
#include "Engine/DemoNetDriver.h"
#include "GameFramework/Controller.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "INF_PC/Components/HealthComponent.h"
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
	CrosshairWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("CrosshairWidget"));
	CrosshairWidget->SetupAttachment(VehicleBody);
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	
	VehicleMovementComponent = CreateDefaultSubobject<UMovementComponentMaster>(TEXT("VehicleMovementComponent"));

	CameraBoom->TargetArmLength = 300.0f;
}

// Called when the game starts or when spawned
void AVehicleMaster::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		// TODO : Change this value to 30.0f or something higher like that while Publishing.
		//NetUpdateFrequency = 5.0f;	// 30.0f while publishing
	}

	SetupVehicleWeaponTable();
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
}

void AVehicleMaster::SetupVehicleWeaponTable()
{
	for (const TPair<TSubclassOf<AWeaponMaster>, int>& Pair : Arsenal)
	{
		EWeaponType WeaponTypeTemp = Pair.Key->GetDefaultObject<AWeaponMaster>()->GetWeaponType();
		FWeaponSetup WeaponSetupTemp;
		WeaponSetupTemp.Weapon = Pair.Key;
		WeaponSetupTemp.NumWeapon = Pair.Value;
		
		WeaponTable.Add(WeaponTypeTemp, WeaponSetupTemp);
	}

																		// DEBUG ONLY : Print out the contents of the WeaponTable
	// for (const TPair<EWeaponType, FWeaponSetup>& Pair : WeaponTable)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("WeaponType = %s, WeaponClass = %s"), *StaticEnum<EWeaponType>()->GetValueAsString(Pair.Key), *Pair.Value.Weapon->GetName());
	// }
}
