// Fill out your copyright notice in the Description page of Project Settings.


#include "JetMaster.h"

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
	TopSpeedInKms = TopSpeedInKms * 28.0f;
}
void AJetMaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Force = Thrust * MaxThrustSpeed * GetActorForwardVector();
	Force += GetVehicleAirResistance();

	Acceleration = Force / MassInKgs;
	Velocity += Acceleration * DeltaTime;
	//UE_LOG(LogTemp, Warning, TEXT("TopSpeed = %f"), TopSpeedInKms);
	Velocity = FMath::Clamp(Velocity.Size(), 0.0f, TopSpeedInKms) * GetActorForwardVector();

	UpdateVehicleRotation(DeltaTime);
	UpdateVehiclePosition(DeltaTime);

	if (HasAuthority())
	{
		ReplicatedTransform = GetActorTransform();
	}
}


void AJetMaster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
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


void AJetMaster::UpdateVehicleRotation(float DeltaTime)
{
	if (Velocity.Size() > 10.0f)
	{
		QuatRot = FQuat(FRotator(Pitch * RotMultiplier * DeltaTime, Yaw * RotMultiplier * DeltaTime, Roll * RotMultiplier * DeltaTime));
		Velocity = QuatRot.RotateVector(Velocity);
		AddActorLocalRotation(QuatRot);
	}
}

// INPUT EVENT OVERRIDES
void AJetMaster::ThrustVehicle(float Value)
{
	Thrust = Value * ThrustMultiplier;
	Server_ThrustVehicle(Value);
}


void AJetMaster::YawVehicle(float Value)
{
	Yaw = Value * YawRate;
	Server_YawVehicle(Value);
}

void AJetMaster::PitchVehicle(float Value)
{
	Pitch = Value * PitchRate;
	Server_PitchVehicle(Value);
}

void AJetMaster::RollVehicle(float Value)
{
	Roll = Value * RollRate;
	Server_RollVehicle(Value);
}

// SERVER FUNCTIONS
void AJetMaster::Server_ThrustVehicle_Implementation(float Value)
{
	Thrust = Value * ThrustMultiplier;
}


void AJetMaster::Server_YawVehicle_Implementation(float Value)
{
	Yaw = Value * YawRate;
}


void AJetMaster::Server_PitchVehicle_Implementation(float Value)
{
	Pitch = Value * PitchRate;
}


void AJetMaster::Server_RollVehicle_Implementation(float Value)
{
	Roll = Value * RollRate;
}


bool AJetMaster::Server_ThrustVehicle_Validate(float Value)
{
	return FMath::Abs(Value) <= 1.0f;
}

bool AJetMaster::Server_YawVehicle_Validate(float Value)
{
	Engine = GetGameInstance()->GetEngine();
	if (!ensure(Engine != nullptr)) return false;

	Engine->AddOnScreenDebugMessage(0, 5.0f, FColor::Red, FString::Printf(TEXT("Yaw = %f"), Value));
	return true;
}

bool AJetMaster::Server_RollVehicle_Validate(float Value)
{
	Engine = GetGameInstance()->GetEngine();
	if (!ensure(Engine != nullptr)) return false;

	Engine->AddOnScreenDebugMessage(0, 5.0f, FColor::Blue, FString::Printf(TEXT("Roll = %f"), Value));
	return true;
}

bool AJetMaster::Server_PitchVehicle_Validate(float Value)
{
	Engine = GetGameInstance()->GetEngine();
	if (!ensure(Engine != nullptr)) return false;

	Engine->AddOnScreenDebugMessage(0, 5.0f, FColor::Green, FString::Printf(TEXT("Pitch = %f"), Value));
	return true;
}


void AJetMaster::OnRep_ReplicatedTransform()
{
	SetActorTransform(ReplicatedTransform);
}