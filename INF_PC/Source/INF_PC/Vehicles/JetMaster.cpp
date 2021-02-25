// Fill out your copyright notice in the Description page of Project Settings.


#include "JetMaster.h"


#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "INF_PC/Framework/INFPlayerState.h"
#include "INF_PC/Weapons/WeaponMaster.h"
#include "INF_PC/Weapons/Guns/JetGun.h"
#include "INF_PC/Weapons/ProjectileWeapons/JetRocket.h"
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

	SetReplicates(true);
	//bReplicates = true;
	SetReplicateMovement(false);
	JetMovementComponent->SetIsReplicated(true);

	LeftGunAttachSocketName = "Gun_Left";
	RightGunAttachSocketName = "Gun_Right";
}

void AJetMaster::UpdateRocketsAvailable()
{
	if (RocketsAvailable > 0)
	{
		RocketsAvailable--;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No more Rockets available!"));
	}
}

void AJetMaster::BeginPlay()
{
	Super::BeginPlay();

	SelectedWeaponType = EWeaponType::JetGun;
	JetMovementComponent->SetTopSpeedInKms(JetMovementComponent->GetTopSpeedInKms() * 28.0f);
	// NetUpdateFrequency = 5.0f;
	// MinNetUpdateFrequency = 3.0f;
	
	if (GetLocalRole() == ROLE_Authority)
	{
		for (const TPair<EWeaponType, FWeaponSetup>& Pair : GetWeaponTable())
		{
			switch (Pair.Key)
			{
			case EWeaponType::ShipAAGun: break;
			case EWeaponType::ShipCannon: break;
			case EWeaponType::Missile: break;
			case EWeaponType::ShipTorpedo: break;
			case EWeaponType::JetGun:
				if (Pair.Value.NumWeapon > 0)
				{
					SetupJetGuns();
				}
				break;
			case EWeaponType::JetRocket:
				if (Pair.Value.NumWeapon > 0)
				{
					SetupJetRockets(Pair.Value.NumWeapon);
				}
				break;
			case EWeaponType::BirdBomb:
				if (Pair.Value.NumWeapon > 0)
				{
					SetupJetBombs();
				}
				break;
			case EWeaponType::BirdTorpedo:
				if (Pair.Value.NumWeapon > 0)
				{
					SetupJetTorpedos();
				}
				break;
			case EWeaponType::AAGunSingleBarrel: break;
			case EWeaponType::AAGunFourBarrel: break;
			default: ;
			}
		}
	}
}

FVector AJetMaster::GetPawnViewLocation() const
{
	if (MainCamera)
	{
		return VehicleFiringRefPoint->GetComponentLocation();
	}

	return Super::GetPawnViewLocation();
}

void AJetMaster::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("ActivateJetRocket", IE_Pressed, this, &AJetMaster::ToggleRocketMode);
	PlayerInputComponent->BindAction("ActivateJetRocket", IE_Released, this, &AJetMaster::ToggleRocketMode);
}

void AJetMaster::SetupJetGuns()
{
	// Spawn the Jet Guns which all the Jets will have.
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
	LeftGun = GetWorld()->SpawnActor<AWeaponMaster>(GetWeaponTable().Find(EWeaponType::JetGun)->Weapon, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	RightGun = GetWorld()->SpawnActor<AWeaponMaster>(GetWeaponTable().Find(EWeaponType::JetGun)->Weapon, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

	if (LeftGun && RightGun)
	{
		LeftGun->SetOwner(this);
		LeftGun->SetWeaponOwner(this);
		RightGun->SetOwner(this);
		RightGun->SetWeaponOwner(this);
			
		LeftGun->AttachToComponent(VehicleBody, FAttachmentTransformRules::SnapToTargetNotIncludingScale, LeftGunAttachSocketName);
		RightGun->AttachToComponent(VehicleBody, FAttachmentTransformRules::SnapToTargetNotIncludingScale, RightGunAttachSocketName);
		// LeftGun->SetActorTransform(VehicleBody->GetSocketTransform(LeftGunAttachSocketName));
		// RightGun->SetActorTransform(VehicleBody->GetSocketTransform(RightGunAttachSocketName));

		Cast<AJetGun>(LeftGun)->SetOwningJet(this);
		Cast<AJetGun>(RightGun)->SetOwningJet(this);

		// FQuat LeftGunRot = (UKismetMathLibrary::FindLookAtRotation(LeftGun->GetActorLocation(), CrosshairWidget->GetComponentLocation())).Quaternion();
		// FQuat RightGunRot = (UKismetMathLibrary::FindLookAtRotation(RightGun->GetActorLocation(), CrosshairWidget->GetComponentLocation())).Quaternion();
		//
		// RightGunRot = FQuat(FRotator(RightGunRot.Rotator().Pitch, RightGun->GetActorRotation().Yaw, RightGun->GetActorRotation().Roll));
		// LeftGunRot = FQuat(FRotator(LeftGunRot.Rotator().Pitch, LeftGun->GetActorRotation().Yaw, LeftGun->GetActorRotation().Roll));
		// 	
		// LeftGun->SetActorRotation(LeftGunRot);
		// RightGun->SetActorRotation(RightGunRot);
	}
}

void AJetMaster::SetupJetRockets(const int NumRockets)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	JetRockets.SetNum(NumRockets);
	SetMaxRockets(NumRockets);
	
	for (int SocketIndex = 0; SocketIndex < NumRockets; SocketIndex++)
	{
		AJetRocket* JetRocket = GetWorld()->SpawnActor<AJetRocket>(GetWeaponTable().Find(EWeaponType::JetRocket)->Weapon, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		JetRockets[SocketIndex] = JetRocket;
		
		JetRockets[SocketIndex]->SetOwner(this);
		JetRockets[SocketIndex]->SetOwningJet(this);
		JetRockets[SocketIndex]->SetWeaponOwner(this);

		const FName JetSocketName("Rocket_" + FString::FromInt(SocketIndex));

		JetRockets[SocketIndex]->AttachToComponent(VehicleBody, FAttachmentTransformRules::SnapToTargetNotIncludingScale, JetSocketName);
	}
}

void AJetMaster::SetupJetBombs()
{
}

void AJetMaster::SetupJetTorpedos()
{
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

	if (PlayerStateRef->GetCurrentPlayerStatus() != EPlayerStatus::W_FreeLookOn)
	{
		const float TargetYawRate = Value * JetMovementComponent->GetYawRate();
		JetMovementComponent->SetYaw(FMath::FInterpTo(JetMovementComponent->GetYaw(), TargetYawRate, GetWorld()->GetDeltaSeconds(), 2.0f));
	}
}

void AJetMaster::PitchVehicle(float Value)
{
	if (JetMovementComponent == nullptr)
	{
		return;
	}

	if (PlayerStateRef->GetCurrentPlayerStatus() != EPlayerStatus::W_FreeLookOn)
	{
		bIntentionalPitch = FMath::Abs(Value) > 0.0f;

		const float TargetPitchRate = Value * JetMovementComponent->GetPitchRate();
		JetMovementComponent->SetPitch(FMath::FInterpTo(JetMovementComponent->GetPitch(), TargetPitchRate, GetWorld()->GetDeltaSeconds(), 2.0f));
	}
	else
	{
		if (!bInsideCockpit)	// If not inside Cockpit...
		{
			const float NewPitch = FMath::Clamp(CameraBoom->GetRelativeRotation().Pitch + Value, -15.0f, 0.0f);
			CameraBoom->SetRelativeRotation(FRotator(NewPitch, CameraBoom->GetRelativeRotation().Yaw, CameraBoom->GetRelativeRotation().Roll));	
		}
	}
}

void AJetMaster::RollVehicle(float Value)
{
	if (JetMovementComponent == nullptr)
	{
		return;
	}

	if (PlayerStateRef->GetCurrentPlayerStatus() != EPlayerStatus::W_FreeLookOn)
	{
		bIntentionalRoll = FMath::Abs(Value) > 0.0f;

		if (bIntentionalPitch && !bIntentionalRoll)
		{
			return;
		}

		const float TargetRollRate = bIntentionalRoll ? (Value * JetMovementComponent->GetRollRate()) : (GetActorRotation().Roll * -0.5f);
		JetMovementComponent->SetRoll(FMath::FInterpTo(JetMovementComponent->GetRoll(), TargetRollRate, GetWorld()->GetDeltaSeconds(), 2.0f));	
	}
	else
	{
		if (!bInsideCockpit)
		{
			CameraBoom->AddRelativeRotation(FRotator(0.0f, Value, 0.0f));	
		}
		else
		{
			const float NewYaw = FMath::Clamp(GetCockpit()->GetCockpitCamera()->GetRelativeRotation().Yaw + Value, -70.0f, 70.0f);
			GetCockpit()->GetCockpitCamera()->SetRelativeRotation(FRotator(GetCockpit()->GetCockpitCamera()->GetRelativeRotation().Pitch, NewYaw, GetCockpit()->GetCockpitCamera()->GetRelativeRotation().Roll));
			
			//GetCockpit()->GetCockpitCamera()->AddRelativeRotation(FRotator(0.0f, Value, 0.0f));
		}
	}
}

void AJetMaster::EnterWeaponOrCockpit()
{
	if (PlayerStateRef->GetCurrentPlayerStatus() == EPlayerStatus::W_ZoomedInWeapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("Already inside Cockpit!!"));
		return;
	}

	if (PlayerStateRef->GetCurrentPlayerStatus() == EPlayerStatus::W_MainVehicleView)
	{
		if (Cockpit)
		{
			GetWorld()->GetFirstPlayerController()->SetViewTarget(Cockpit->GetChildActor());
			PlayerStateRef->SetCurrentPlayerStatus(EPlayerStatus::W_ZoomedInWeapon);
			bInsideCockpit = true;
			UE_LOG(LogTemp, Warning, TEXT("Inside Cockpit!!"));
		}
	}
}

void AJetMaster::ExitWeaponOrCockpit()
{
	if (PlayerStateRef->GetCurrentPlayerStatus() == EPlayerStatus::W_MainVehicleView)
	{
		UE_LOG(LogTemp, Warning, TEXT("Already in Main View!!"));
		return;	
	}
	
	if (PlayerStateRef->GetCurrentPlayerStatus() == EPlayerStatus::W_ZoomedInWeapon)
	{
		GetWorld()->GetFirstPlayerController()->SetViewTarget(this);
		bInsideCockpit = false;
		PlayerStateRef->SetCurrentPlayerStatus(EPlayerStatus::W_MainVehicleView);
		UE_LOG(LogTemp, Warning, TEXT("Back to Main View!!"));
	}
}

void AJetMaster::FreeLookOn()
{
	if (PlayerStateRef->GetCurrentPlayerStatus() != EPlayerStatus::W_FreeLookOn)
	{
		PlayerStateRef->SetCurrentPlayerStatus(EPlayerStatus::W_FreeLookOn);
	}
}

void AJetMaster::FreeLookOff()
{
	if (PlayerStateRef->GetCurrentPlayerStatus() == EPlayerStatus::W_FreeLookOn)
	{
		if (bInsideCockpit)
		{
			GetCockpit()->ResetCockpitCamera();
			PlayerStateRef->SetCurrentPlayerStatus(EPlayerStatus::W_ZoomedInWeapon);
		}
		else
		{
			PlayerStateRef->SetCurrentPlayerStatus(EPlayerStatus::W_MainVehicleView);
			CameraBoom->SetRelativeTransform(OriginalCameraBoomTransform);	
		}
	}
}

void AJetMaster::ToggleRocketMode()
{
	bRocketMode = !bRocketMode;
	
	if (bRocketMode)
	{
		SelectedWeaponType = EWeaponType::JetRocket;
	}
	else
	{
		SelectedWeaponType = EWeaponType::JetGun;
	}
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
	switch (SelectedWeaponType)
	{
	case EWeaponType::ShipAAGun: break;
	case EWeaponType::ShipCannon: break;
	case EWeaponType::Missile: break;
	case EWeaponType::ShipTorpedo: break;
	case EWeaponType::JetGun:
		if (LeftGun && RightGun)
		{
			LeftGun->StartFire();
			RightGun->StartFire();

			//MulticastFire();
		}
		break;
	case EWeaponType::JetRocket:
		if (RocketsAvailable > 0)
		{
			//JetRockets[RocketsAvailable - 1]->SetOwningJet(this);
			JetRockets[RocketsAvailable - 1]->StartFire();
			JetRockets[RocketsAvailable - 1]->SetActorScale3D(FVector::ZeroVector);
			JetRockets[RocketsAvailable - 1]->Destroy();
			JetRockets.RemoveAt(RocketsAvailable - 1);
			UpdateRocketsAvailable();
			UE_LOG(LogTemp, Warning, TEXT("RocketsAvailable = %d"), JetRockets.Num());	
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No more rockets available to fire %d"), JetRockets.Num());
		}
		break;
	case EWeaponType::BirdTorpedo: break;
	case EWeaponType::AAGunSingleBarrel: break;
	case EWeaponType::AAGunFourBarrel: break;
	default: ;
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
	switch (SelectedWeaponType)
	{
	case EWeaponType::ShipAAGun: break;
	case EWeaponType::ShipCannon: break;
	case EWeaponType::Missile: break;
	case EWeaponType::ShipTorpedo: break;
	case EWeaponType::JetGun:
		if (LeftGun && RightGun)
		{
			LeftGun->StopFire();
			RightGun->StopFire();

			//MulticastFire();
		}
		break;
	case EWeaponType::JetRocket:
		UE_LOG(LogTemp, Warning, TEXT("Stop rocket fire!!"));
		break;
	case EWeaponType::BirdTorpedo: break;
	case EWeaponType::AAGunSingleBarrel: break;
	case EWeaponType::AAGunFourBarrel: break;
	default: ;
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
