// Fill out your copyright notice in the Description page of Project Settings.


#include "JetMovementComponent.h"
#include "GameFrameWork/GameState.h"

UJetMovementComponent::UJetMovementComponent() : UMovementComponentMaster()
{

}

void UJetMovementComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UJetMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

FVector UJetMovementComponent::GetVehicleAirResistance()
{
	return -Velocity.GetSafeNormal() * Velocity.SizeSquared() * DragCoefficient;
}

FVector UJetMovementComponent::GetVehicleRollingResistance()
{
	return FVector::ZeroVector;
}

void UJetMovementComponent::UpdateVehiclePosition(float DeltaTime)
{
	Translation = FMath::Clamp(Velocity.Size(), 0.0f, TopSpeedInKms) * GetOwner()->GetActorForwardVector() * 100.0f * DeltaTime;
	GetOwner()->AddActorWorldOffset(Translation);
}

void UJetMovementComponent::UpdateVehicleRotation(float DeltaTime, float PitchVal, float YawVal, float RollVal)
{
	if (Velocity.Size() > 10.0f)
	{
		QuatRot = FQuat(FRotator(PitchVal * DeltaTime, YawVal * DeltaTime, RollVal * DeltaTime));
		Velocity = QuatRot.RotateVector(Velocity);
		GetOwner()->AddActorLocalRotation(QuatRot);

		/*FRotator DeltaRotation(0.0f, 0.0f, 0.0f);
		DeltaRotation.Pitch = Pitch * DeltaTime;
		DeltaRotation.Yaw = Yaw * DeltaTime;
		DeltaRotation.Roll = Roll * DeltaTime;

		Velocity = DeltaRotation.RotateVector(Velocity);
		AddActorLocalRotation(DeltaRotation);*/
	}
}

void UJetMovementComponent::SimulateMove(const FVehicleMove& Move)
{
	Force = Move.Thrust * MaxThrustSpeed * GetOwner()->GetActorForwardVector();
	Force += GetVehicleAirResistance();

	Acceleration = Force / MassInKgs;
	Velocity += Acceleration * Move.DeltaTime;
	//UE_LOG(LogTemp, Warning, TEXT("TopSpeed = %f"), TopSpeedInKms);
	Velocity = FMath::Clamp(Velocity.Size(), 0.0f, TopSpeedInKms) * GetOwner()->GetActorForwardVector();

	UpdateVehicleRotation(Move.DeltaTime, Move.Pitch, Move.Yaw, Move.Roll);
	UpdateVehiclePosition(Move.DeltaTime);
}

FVehicleMove UJetMovementComponent::CreateMove(float DeltaTime)
{
	FVehicleMove Move;
	Move.DeltaTime = DeltaTime;
	Move.Yaw = this->Yaw;
	Move.Pitch = this->Pitch;
	Move.Roll = this->Roll;
	Move.Thrust = this->Thrust;
	//Move.TimeStamp = GetWorld()->TimeSeconds;
	Move.TimeStamp = GetOwner()->GetWorld()->GetGameState()->GetServerWorldTimeSeconds();

	return Move;
}
