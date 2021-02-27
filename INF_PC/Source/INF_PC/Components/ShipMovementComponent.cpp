// Fill out your copyright notice in the Description page of Project Settings.


#include "ShipMovementComponent.h"

#include "GameFramework/GameStateBase.h"

UShipMovementComponent::UShipMovementComponent() : UMovementComponentMaster()
{
}

void UShipMovementComponent::BeginPlay()
{
	Super::BeginPlay();
}

FVector UShipMovementComponent::GetVehicleAirResistance()
{
	return -Velocity.GetSafeNormal() * Velocity.SizeSquared() * DragCoefficient;
}

FVector UShipMovementComponent::GetVehicleRollingResistance()
{
	return FVector::ZeroVector;
}

void UShipMovementComponent::UpdateVehiclePosition(float DeltaTime)
{
	Translation = FMath::Clamp(Velocity.Size(), 0.0f, TopSpeedInKms) * GetOwner()->GetActorForwardVector() * 100.0f * DeltaTime;
	GetOwner()->AddActorWorldOffset(Translation);
}

void UShipMovementComponent::UpdateVehicleRotation(float DeltaTime, float PitchVal, float YawVal, float RollVal)
{
	if (Velocity.Size() > 10.0f)
	{
		QuatRot = FQuat(FRotator(0.0f, YawVal * DeltaTime, 0.0f));
		Velocity = QuatRot.RotateVector(Velocity);
		GetOwner()->AddActorLocalRotation(QuatRot);
	}
}

void UShipMovementComponent::SimulateMove(const FVehicleMove& Move)
{
	Force = Move.Thrust * MaxThrustSpeed * GetOwner()->GetActorForwardVector();
	Force += GetVehicleAirResistance();

	Acceleration = Force / MassInKgs;
	Velocity += Acceleration * Move.DeltaTime;
	Velocity = FMath::Clamp(Velocity.Size(), 0.0f, TopSpeedInKms) * GetOwner()->GetActorForwardVector();
	//UE_LOG(LogTemp, Warning, TEXT("Force = %f"), Force.Size());

	UpdateVehicleRotation(Move.DeltaTime, Move.Pitch, Move.Yaw, Move.Roll);
	UpdateVehiclePosition(Move.DeltaTime);
}

FVehicleMove UShipMovementComponent::CreateMove(float DeltaTime)
{
	FVehicleMove Move;
	Move.DeltaTime = DeltaTime;
	Move.Yaw = this->Yaw;
	Move.Thrust = this->Thrust;
	//Move.TimeStamp = GetWorld()->TimeSeconds;
	Move.TimeStamp = GetOwner()->GetWorld()->GetGameState()->GetServerWorldTimeSeconds();

	return Move;
}
