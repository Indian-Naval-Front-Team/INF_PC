// Fill out your copyright notice in the Description page of Project Settings.


#include "JetMaster.h"

#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFrameWork/GameState.h"
#include "Kismet/KismetMathLibrary.h"

AJetMaster::AJetMaster()
{
	bReplicates = true;

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

	LeftGunAttachSocketName = "Gun_Left";
	RightGunAttachSocketName = "Gun_Right";
}

FVector AJetMaster::GetPawnViewLocation() const
{
	if (MainCamera)
	{
		return MainCamera->GetComponentLocation();
	}

	return Super::GetPawnViewLocation();
}

void AJetMaster::BeginPlay()
{
	Super::BeginPlay();
	TopSpeedInKms = TopSpeedInKms * 28.0f;

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
			LeftGun->AttachToComponent(VehicleBody, FAttachmentTransformRules::SnapToTargetNotIncludingScale, LeftGunAttachSocketName);
			RightGun->SetOwner(this);
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

void AJetMaster::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AJetMaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetLocalRole() == ROLE_AutonomousProxy)
	{
		const FVehicleMove Move = CreateMove(DeltaTime);
		UnacknowledgedMoves.Add(Move);
		SimulateMove(Move);
		Server_SendMove(Move);
	}

	// We are the server and also in control of the Pawn.
	if (GetLocalRole() == ROLE_Authority && IsLocallyControlled())
	{
		const FVehicleMove Move = CreateMove(DeltaTime);
		Server_SendMove(Move);
	}

	if (GetLocalRole() == ROLE_SimulatedProxy)
	{
		//SimulateMove(ServerState.LastMove);
		ClientTick(DeltaTime);
	}
}

void AJetMaster::ClientTick(float DeltaTime)
{
	Client_TimeSinceUpdate += DeltaTime;

	if (Client_TimeBetweenLastUpdates < KINDA_SMALL_NUMBER)
	{
		return;
	}

	const float LerpRatio = Client_TimeSinceUpdate / Client_TimeBetweenLastUpdates;

	const FVector_NetQuantize TargetLocation = ServerState.VehicleTransform.GetLocation();
	const FVector_NetQuantize StartLocation = Client_StartTransform.GetLocation();
	const float VelocityToDerivative = Client_TimeBetweenLastUpdates * 100.0f;
	const FVector_NetQuantize StartDerivative = Client_StartVelocity * VelocityToDerivative;
	const FVector_NetQuantize TargetDerivative = ServerState.Velocity * VelocityToDerivative;

	//FVector_NetQuantize NewLocation = FMath::LerpStable(StartLocation, TargetLocation, LerpRatio);
	const FVector_NetQuantize NewLocation = FMath::CubicInterp(StartLocation, StartDerivative, TargetLocation, TargetDerivative, LerpRatio);
	SetActorLocation(NewLocation);

	const FVector_NetQuantize NewDerivative = FMath::CubicInterpDerivative(StartLocation, StartDerivative, TargetLocation, TargetDerivative, LerpRatio);
	const FVector_NetQuantize NewVelocity = NewDerivative / VelocityToDerivative;
	Velocity = NewVelocity;

	const FQuat TargetRotation = ServerState.VehicleTransform.GetRotation();
	const FQuat StartRotation = Client_StartTransform.GetRotation();

	const FQuat NewRotation = FQuat::Slerp(StartRotation, TargetRotation, LerpRatio);
	SetActorRotation(NewRotation);
}

// JET MOVEMENT AND ROTATION FUNCTIONS
FVector AJetMaster::GetVehicleAirResistance()
{
	return -Velocity.GetSafeNormal() * Velocity.SizeSquared() * DragCoefficient;
}


FVector AJetMaster::GetVehicleRollingResistance()
{
	return FVector::ZeroVector;
}


void AJetMaster::UpdateVehiclePosition(float DeltaTime)
{
	Translation = FMath::Clamp(Velocity.Size(), 0.0f, TopSpeedInKms) * GetActorForwardVector() * 100.0f * DeltaTime;
	AddActorWorldOffset(Translation);
}


void AJetMaster::UpdateVehicleRotation(float DeltaTime, float PitchVal, float YawVal, float RollVal)
{
	if (Velocity.Size() > 10.0f)
	{
		QuatRot = FQuat(FRotator(PitchVal * DeltaTime, YawVal * DeltaTime, RollVal * DeltaTime));
		Velocity = QuatRot.RotateVector(Velocity);
		AddActorLocalRotation(QuatRot);
	}
}

// INPUT EVENT OVERRIDES
void AJetMaster::ThrustVehicle(float Value)
{
	Thrust = Value * ThrustMultiplier;
}


void AJetMaster::YawVehicle(float Value)
{
	const float TargetYawRate = Value * YawRate;
	Yaw = FMath::FInterpTo(Yaw, TargetYawRate, GetWorld()->GetDeltaSeconds(), 2.0f);
}

void AJetMaster::PitchVehicle(float Value)
{
	bIntentionalPitch = FMath::Abs(Value) > 0.0f;

	const float TargetPitchRate = Value * PitchRate;
	Pitch = FMath::FInterpTo(Pitch, TargetPitchRate, GetWorld()->GetDeltaSeconds(), 2.0f);
}

void AJetMaster::RollVehicle(float Value)
{
	bIntentionalRoll = FMath::Abs(Value) > 0.0f;

	if (bIntentionalPitch && !bIntentionalRoll)
	{
		return;
	}

	const float TargetRollRate = bIntentionalRoll ? (Value * RollRate) : (GetActorRotation().Roll * -0.5f);
	Roll = FMath::FInterpTo(Roll, TargetRollRate, GetWorld()->GetDeltaSeconds(), 2.0f);
}

void AJetMaster::FireSelectedWeapon()
{
	// if (LeftGun && RightGun)
	// {
	// 	LeftGun->StartFire();
	// 	RightGun->StartFire();
	// }
	ServerFire();
}

void AJetMaster::StopFiringSelectedWeapon()
{
	// if (LeftGun && RightGun)
	// {
	// 	LeftGun->StopFire();
	// 	RightGun->StopFire();
	// }
	ServerStopFire();
}

void AJetMaster::ServerFire_Implementation()
{
	if (LeftGun && RightGun)
	{
		LeftGun->StartFire();
		RightGun->StartFire();

		MulticastFire();
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

		MulticastStopFire();
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

void AJetMaster::SimulateMove(const FVehicleMove& Move)
{
	Force = Move.Thrust * MaxThrustSpeed * GetActorForwardVector();
	Force += GetVehicleAirResistance();

	Acceleration = Force / MassInKgs;
	Velocity += Acceleration * Move.DeltaTime;
	//UE_LOG(LogTemp, Warning, TEXT("TopSpeed = %f"), TopSpeedInKms);
	Velocity = FMath::Clamp(Velocity.Size(), 0.0f, TopSpeedInKms) * GetActorForwardVector();
	//UE_LOG(LogTemp, Warning, TEXT("Velocity = %f"), Velocity.Size());
	UpdateVehicleRotation(Move.DeltaTime, Move.Pitch, Move.Yaw, Move.Roll);
	UpdateVehiclePosition(Move.DeltaTime);
}

FVehicleMove AJetMaster::CreateMove(float DeltaTime)
{
	FVehicleMove Move;
	Move.DeltaTime = DeltaTime;
	Move.Yaw = this->Yaw;
	Move.Pitch = this->Pitch;
	Move.Roll = this->Roll;
	Move.Thrust = this->Thrust;
	//Move.TimeStamp = GetWorld()->TimeSeconds;
	Move.TimeStamp = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();

	return Move;
}

void AJetMaster::ClearAcknowledgedMoves(FVehicleMove LastMove)
{
	TArray<FVehicleMove> NewMoves;

	for (const FVehicleMove& Move : UnacknowledgedMoves)
	{
		if (Move.TimeStamp > LastMove.TimeStamp)
		{
			NewMoves.Add(Move);
		}
	}

	UnacknowledgedMoves = NewMoves;
}

void AJetMaster::Server_SendMove_Implementation(FVehicleMove Move)
{
	SimulateMove(Move);

	// Send the canonical state to the other clients
	ServerState.LastMove = Move;
	ServerState.VehicleTransform = GetActorTransform();
	ServerState.Velocity = Velocity;
}

bool AJetMaster::Server_SendMove_Validate(FVehicleMove Move)
{
	// TODO : Make validation better!
	return true;
}

// SERVER FUNCTIONS
void AJetMaster::OnRep_ServerState()
{
	switch (GetLocalRole())
	{
	case ROLE_AutonomousProxy:
		AutonomousProxy_OnRepServerState();
		break;
	case ROLE_SimulatedProxy:
		SimulatedProxy_OnRepServerState();
		break;
	default:
		break;
	}
}

void AJetMaster::SimulatedProxy_OnRepServerState()
{
	Client_TimeBetweenLastUpdates = Client_TimeSinceUpdate;
	Client_TimeSinceUpdate = 0.0f;

	Client_StartTransform = GetActorTransform();
	Client_StartVelocity = Velocity;
}

void AJetMaster::AutonomousProxy_OnRepServerState()
{
	SetActorTransform(ServerState.VehicleTransform);
	Velocity = ServerState.Velocity;

	ClearAcknowledgedMoves(ServerState.LastMove);

	for (const FVehicleMove& Move : UnacknowledgedMoves)
	{
		SimulateMove(Move);
	}
}
