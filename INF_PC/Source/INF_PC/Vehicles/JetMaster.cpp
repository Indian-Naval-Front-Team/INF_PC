// Fill out your copyright notice in the Description page of Project Settings.


#include "JetMaster.h"


#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"
#include "INF_PC/Weapons/WeaponMaster.h"
#include "Kismet/KismetMathLibrary.h"

AJetMaster::AJetMaster()
{
	Cockpit = CreateDefaultSubobject<UChildActorComponent>(TEXT("Cockpit"));
	Cockpit->SetupAttachment(Super::VehicleBody, "Cockpit");
	Wing_Left = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wing_Left"));
	Wing_Left->SetupAttachment(Super::VehicleBody, "Wing_Left");
	Wing_Right = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wing_Right"));
	Wing_Right->SetupAttachment(Super::VehicleBody, "Wing_Right");
	Wheel_Front = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wheel_Front"));
	Wheel_Front->SetupAttachment(Super::VehicleBody, "Wheel_Front");
	Wheel_Right = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wheel_Right"));
	Wheel_Right->SetupAttachment(Super::VehicleBody, "Wheel_Right");
	Wheel_Left = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wheel_Left"));
	Wheel_Left->SetupAttachment(Super::VehicleBody, "Wheel_Left");
	TailHook = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TailHook"));
	TailHook->SetupAttachment(Super::VehicleBody, "TailHook");
	RightWing_Flap = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightWing_Flap"));
	RightWing_Flap->SetupAttachment(Super::VehicleBody, "RightWing_Flap");
	RightWing_Aileron = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightWing_Aileron"));
	RightWing_Aileron->SetupAttachment(Super::VehicleBody, "RightWing_Aileron");
	LeftWing_Aileron = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftWing_Aileron"));
	LeftWing_Aileron->SetupAttachment(Super::VehicleBody, "LeftWing_Aileron");
	LeftWing_Flap = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftWing_Flap"));
	LeftWing_Flap->SetupAttachment(Super::VehicleBody, "LeftWing_Flap");
	RearWing_Elevator_Left = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RearWing_Elevator_Left"));
	RearWing_Elevator_Left->SetupAttachment(Super::VehicleBody, "RearWing_Elevator_Left");
	RearWing_Elevator_Right = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RearWing_Elevator_Right"));
	RearWing_Elevator_Right->SetupAttachment(Super::VehicleBody, "RearWing_Elevator_Right");

	JetMovementComponent = CreateDefaultSubobject<UJetMovementComponent>(TEXT("JetMovementComponent"));
	NetworkingComponent = CreateDefaultSubobject<UNetworkingComponent>(TEXT("NetworkingComponent"));

	SetReplicates(true);
	//bReplicates = true;
	SetReplicateMovement(false);
	JetMovementComponent->SetIsReplicated(true);

	LeftGunAttachSocketName = "Gun_Left";
	RightGunAttachSocketName = "Gun_Right";
}

void AJetMaster::BeginPlay()
{
	Super::BeginPlay();
	
	JetMovementComponent->SetTopSpeedInKms(JetMovementComponent->GetTopSpeedInKms() * 28.0f);
	// NetUpdateFrequency = 5.0f;
	// MinNetUpdateFrequency = 3.0f;

	if (GetLocalRole() == ROLE_Authority)
	{
		// Spawn the Jet Gun.
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		LeftGun = GetWorld()->SpawnActor<AWeaponMaster>(Arsenal[0], FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		RightGun = GetWorld()->SpawnActor<AWeaponMaster>(Arsenal[1], FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

		if (LeftGun && RightGun)
		{
			LeftGun->SetOwner(this);
			LeftGun->SetWeaponOwner(this);
			RightGun->SetOwner(this);
			RightGun->SetWeaponOwner(this);
			
			LeftGun->AttachToComponent(VehicleBody, FAttachmentTransformRules::SnapToTargetNotIncludingScale, LeftGunAttachSocketName);
			RightGun->AttachToComponent(VehicleBody, FAttachmentTransformRules::SnapToTargetNotIncludingScale, RightGunAttachSocketName);

			FQuat LeftGunRot = (UKismetMathLibrary::FindLookAtRotation(LeftGun->GetActorLocation(), CrosshairWidget->GetComponentLocation())).Quaternion();
			FQuat RightGunRot = (UKismetMathLibrary::FindLookAtRotation(RightGun->GetActorLocation(), CrosshairWidget->GetComponentLocation())).Quaternion();

			RightGunRot = FQuat(FRotator(RightGunRot.Rotator().Pitch, RightGun->GetActorRotation().Yaw, RightGunRot.Rotator().Roll));
			LeftGunRot = FQuat(FRotator(LeftGunRot.Rotator().Pitch, LeftGun->GetActorRotation().Yaw, LeftGunRot.Rotator().Roll));
			
			LeftGun->SetActorRotation(LeftGunRot);
			RightGun->SetActorRotation(RightGunRot);
		}
	}
}

FVector AJetMaster::GetPawnViewLocation() const
{
	if (MainCamera)
	{
		return MainCamera->GetComponentLocation();
	}

	return Super::GetPawnViewLocation();
}

void AJetMaster::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AJetMaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// INPUT EVENT OVERRIDES
void AJetMaster::ThrustVehicle(float Value)
{
	if (JetMovementComponent == nullptr)
	{
		return;
	}

	JetMovementComponent->SetThrust(Value * JetMovementComponent->GetThrustMultipliyer());
}


void AJetMaster::YawVehicle(float Value)
{
	if (JetMovementComponent == nullptr)
	{
		return;
	}

	const float TargetYawRate = Value * JetMovementComponent->GetYawRate();
	JetMovementComponent->SetYaw(FMath::FInterpTo(JetMovementComponent->GetYaw(), TargetYawRate, GetWorld()->GetDeltaSeconds(), 2.0f));
}

void AJetMaster::PitchVehicle(float Value)
{
	if (JetMovementComponent == nullptr)
	{
		return;
	}

	bIntentionalPitch = FMath::Abs(Value) > 0.0f;

	const float TargetPitchRate = Value * JetMovementComponent->GetPitchRate();
	JetMovementComponent->SetPitch(FMath::FInterpTo(JetMovementComponent->GetPitch(), TargetPitchRate, GetWorld()->GetDeltaSeconds(), 2.0f));
}

void AJetMaster::RollVehicle(float Value)
{
	if (JetMovementComponent == nullptr)
	{
		return;
	}

	bIntentionalRoll = FMath::Abs(Value) > 0.0f;

	if (bIntentionalPitch && !bIntentionalRoll)
	{
		return;
	}

	const float TargetRollRate = bIntentionalRoll ? (Value * JetMovementComponent->GetRollRate()) : (GetActorRotation().Roll * -0.5f);
	JetMovementComponent->SetRoll(FMath::FInterpTo(JetMovementComponent->GetRoll(), TargetRollRate, GetWorld()->GetDeltaSeconds(), 2.0f));
}

void AJetMaster::FireSelectedWeapon()
{
	ServerFire();
}

void AJetMaster::StopFiringSelectedWeapon()
{
	ServerStopFire();
}

void AJetMaster::ServerFire_Implementation()
{	
	if (LeftGun && RightGun)
	{
		LeftGun->StartFire();
		RightGun->StartFire();

		//MulticastFire();
	}
}

bool AJetMaster::ServerFire_Validate()
{
	return true;
}

void AJetMaster::MulticastFire_Implementation()
{
	if (LeftGun && RightGun)
	{
		LeftGun->StartFire();
		RightGun->StartFire();
	}
}

bool AJetMaster::MulticastFire_Validate()
{
	return true;
}

void AJetMaster::ServerStopFire_Implementation()
{
	if (LeftGun && RightGun)
	{
		LeftGun->StopFire();
		RightGun->StopFire();

		//MulticastStopFire();
	}
}

bool AJetMaster::ServerStopFire_Validate()
{
	return true;
}

void AJetMaster::MulticastStopFire_Implementation()
{
	if (LeftGun && RightGun)
	{
		LeftGun->StopFire();
		RightGun->StopFire();
	}
}

bool AJetMaster::MulticastStopFire_Validate()
{
	return true;
}
