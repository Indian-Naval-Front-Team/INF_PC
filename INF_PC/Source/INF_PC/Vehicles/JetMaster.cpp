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

	JetMovementComponent = CreateDefaultSubobject<UJetMovementComponent>(TEXT("JetMovementComponent"));
	NetworkingComponent = CreateDefaultSubobject<UNetworkingComponent>(TEXT("NetworkingComponent"));
}

void AJetMaster::BeginPlay()
{
	Super::BeginPlay();
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
