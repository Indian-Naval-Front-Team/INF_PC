// Fill out your copyright notice in the Description page of Project Settings.


#include "JetMaster.h"
#include "Net/UnrealNetwork.h"

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
}

void AJetMaster::BeginPlay()
{
	Super::BeginPlay();
	//TopSpeedInKms = TopSpeedInKms * 28.0f;

	if (HasAuthority())
	{
		//NetUpdateFrequency = 1.0f;
	}
}

void AJetMaster::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AJetMaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Force = Thrust * MaxThrustSpeed * GetActorForwardVector();
	Force += GetVehicleAirResistance();
	Acceleration = Force / MassInKgs;
	Velocity = Velocity + Acceleration * DeltaTime;
	//UE_LOG(LogTemp, Warning, TEXT("Speed = %f"), Velocity.Size());
	Velocity = FMath::Clamp(Velocity.Size(), 0.0f, TopSpeedInKms) * GetActorForwardVector();

	UpdateVehicleRotation(DeltaTime);
	UpdateVehiclePosition(DeltaTime);

	if (HasAuthority())
	{
		ReplicatedTransform = GetActorTransform();
	}
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
	//Translation = FMath::Clamp(Velocity.Size(), 0.0f, TopSpeedInKms) * GetActorForwardVector() * 100.0f * DeltaTime;
	Translation = Velocity * 100.0f * DeltaTime;
	AddActorWorldOffset(Translation);
}

void AJetMaster::UpdateVehicleRotation(float DeltaTime)
{
	if (Velocity.Size() > 10.0f)
	{
		QuatRot = FQuat(FRotator(Pitch * DeltaTime, Yaw * DeltaTime, Roll * DeltaTime));
		Velocity = QuatRot.RotateVector(Velocity);
		AddActorLocalRotation(QuatRot);
	}
}

// INPUT EVENT OVERRIDES
void AJetMaster::ThrustVehicle(float Value)
{
	const float TargetThrust = Value * ThrustMultiplier;
	Thrust = FMath::FInterpTo(Thrust, TargetThrust, GetWorld()->GetDeltaSeconds(), 10.0f);
	//Thrust = Value * ThrustMultiplier;
	Server_ThrustVehicle(Value);
}


void AJetMaster::YawVehicle(float Value)
{
	const float TargetYawRate = Value * YawRate;
	Yaw = FMath::FInterpTo(Yaw, TargetYawRate, GetWorld()->GetDeltaSeconds(), 1.0f);
	//Yaw = Value * YawRate;
	Server_YawVehicle(Value);
}

void AJetMaster::PitchVehicle(float Value)
{
	bIntentionalPitch = FMath::Abs(Value) > 0.0f;

	const float TargetPitchRate = Value * PitchRate;
	Pitch = FMath::FInterpTo(Pitch, TargetPitchRate, GetWorld()->GetDeltaSeconds(), 1.0f);

	Server_PitchVehicle(Value);
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

	Server_RollVehicle(Value);
}

// SERVER FUNCTIONS
void AJetMaster::Server_ThrustVehicle_Implementation(float Value)
{
	const float TargetThrust = Value * ThrustMultiplier;
	Thrust = FMath::FInterpTo(Thrust, TargetThrust, GetWorld()->GetDeltaSeconds(), 10.0f);
	//Thrust = Value * ThrustMultiplier;
}

void AJetMaster::Server_YawVehicle_Implementation(float Value)
{
	const float TargetYawRate = Value * YawRate;
	Yaw = FMath::FInterpTo(Yaw, TargetYawRate, GetWorld()->GetDeltaSeconds(), 1.0f);
}

void AJetMaster::Server_PitchVehicle_Implementation(float Value)
{
	bIntentionalPitch = FMath::Abs(Value) > 0.0f;

	const float TargetPitchRate = Value * PitchRate;
	Pitch = FMath::FInterpTo(Pitch, TargetPitchRate, GetWorld()->GetDeltaSeconds(), 1.0f);
}

void AJetMaster::Server_RollVehicle_Implementation(float Value)
{
	bIntentionalRoll = FMath::Abs(Value) > 0.0f;

	if (bIntentionalPitch && !bIntentionalRoll)
	{
		return;
	}

	const float TargetRollRate = bIntentionalRoll ? (Value * RollRate) : (GetActorRotation().Roll * -0.5f);
	Roll = FMath::FInterpTo(Roll, TargetRollRate, GetWorld()->GetDeltaSeconds(), 2.0f);
}

bool AJetMaster::Server_ThrustVehicle_Validate(float Value)
{
	return FMath::Abs(Value) <= 1.0f;
}

bool AJetMaster::Server_YawVehicle_Validate(float Value)
{
	return true;
}

bool AJetMaster::Server_RollVehicle_Validate(float Value)
{
	return true;
}

bool AJetMaster::Server_PitchVehicle_Validate(float Value)
{
	return true;
}

void AJetMaster::OnRep_ReplicatedTransform()
{
	SetActorTransform(ReplicatedTransform);
	//UE_LOG(LogTemp, Warning, TEXT("Replicated Transform at time %f"), GetWorld()->GetRealTimeSeconds());
}
