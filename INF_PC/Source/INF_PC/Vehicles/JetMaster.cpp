// Fill out your copyright notice in the Description page of Project Settings.


#include "JetMaster.h"
#include "GameFrameWork/GameState.h"

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

void AJetMaster::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AJetMaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (IsLocallyControlled())
	//{
	//	FVehicleMove Move = CreateMove(DeltaTime);

	//	// Don't add Moves to the UnacknowledgedMoves Table unless you are the client.
	//	if (!HasAuthority())
	//	{
	//		UnacknowledgedMoves.Add(Move);
	//		SimulateMove(Move);
	//		UE_LOG(LogTemp, Warning, TEXT("Queue Length = %d"), UnacknowledgedMoves.Num());
	//	}

	//	Server_SendMove(Move);
	//}

	if (GetLocalRole() == ROLE_AutonomousProxy)
	{
		FVehicleMove Move = CreateMove(DeltaTime);
		UnacknowledgedMoves.Add(Move);
		SimulateMove(Move);
		Server_SendMove(Move);
	}

	// We are the server and also in control of the Pawn.
	if (GetLocalRole() == ROLE_Authority && IsLocallyControlled())
	{
		FVehicleMove Move = CreateMove(DeltaTime);
		Server_SendMove(Move);
	}

	if (GetLocalRole() == ROLE_SimulatedProxy)
	{
		SimulateMove(ServerState.LastMove);
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

		/*FRotator DeltaRotation(0.0f, 0.0f, 0.0f);
		DeltaRotation.Pitch = Pitch * DeltaTime;
		DeltaRotation.Yaw = Yaw * DeltaTime;
		DeltaRotation.Roll = Roll * DeltaTime;

		Velocity = DeltaRotation.RotateVector(Velocity);
		AddActorLocalRotation(DeltaRotation);*/
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
	//Yaw = Value * YawRate;
}

void AJetMaster::PitchVehicle(float Value)
{
	//Pitch = Value * PitchRate;
	bIntentionalPitch = FMath::Abs(Value) > 0.0f;

	const float TargetPitchRate = Value * PitchRate;
	Pitch = FMath::FInterpTo(Pitch, TargetPitchRate, GetWorld()->GetDeltaSeconds(), 2.0f);
}

void AJetMaster::RollVehicle(float Value)
{
	//Roll = Value * RollRate;

	bIntentionalRoll = FMath::Abs(Value) > 0.0f;

	if (bIntentionalPitch && !bIntentionalRoll)
	{
		return;
	}

	const float TargetRollRate = bIntentionalRoll ? (Value * RollRate) : (GetActorRotation().Roll * -0.5f);
	Roll = FMath::FInterpTo(Roll, TargetRollRate, GetWorld()->GetDeltaSeconds(), 2.0f);
}

void AJetMaster::SimulateMove(const FVehicleMove& Move)
{
	Force = Move.Thrust * MaxThrustSpeed * GetActorForwardVector();
	Force += GetVehicleAirResistance();

	Acceleration = Force / MassInKgs;
	Velocity += Acceleration * Move.DeltaTime;
	//UE_LOG(LogTemp, Warning, TEXT("TopSpeed = %f"), TopSpeedInKms);
	Velocity = FMath::Clamp(Velocity.Size(), 0.0f, TopSpeedInKms) * GetActorForwardVector();

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
	/*Thrust = Move.Thrust;
	Yaw = Move.Yaw;
	Pitch = Move.Pitch;
	Roll = Move.Roll;*/
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
	SetActorTransform(ServerState.VehicleTransform);
	Velocity = ServerState.Velocity;

	ClearAcknowledgedMoves(ServerState.LastMove);

	for (const FVehicleMove& Move : UnacknowledgedMoves)
	{
		SimulateMove(Move);
	}
}
