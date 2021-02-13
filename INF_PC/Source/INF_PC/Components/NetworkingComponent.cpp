// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkingComponent.h"
#include <INF_PC/Components/JetMovementComponent.h>
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UNetworkingComponent::UNetworkingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);

	OwningPawn = Cast<APawn>(GetOwner());
}


// Called when the game starts
void UNetworkingComponent::BeginPlay()
{
	Super::BeginPlay();

	switch (TypeOfVehicle)
	{
	case VehicleType::SHIP:
		break;
	case VehicleType::JET:
		MovementComponent = GetOwner()->FindComponentByClass<UJetMovementComponent>();
		break;
	case VehicleType::CHOPPER:
		break;
	case VehicleType::OTHERS:
		MovementComponent = GetOwner()->FindComponentByClass<UMovementComponentMaster>();
		break;
	default:
		break;
	}
}

void UNetworkingComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UNetworkingComponent, ServerState);
}

// Called every frame
void UNetworkingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (MovementComponent == nullptr)
	{
		return;
	}

	FVehicleMove LastMove = MovementComponent->GetLastMove();

	// We are the server and also in control of the Pawn.
	if (GetOwner()->GetLocalRole() == ROLE_Authority && OwningPawn->IsLocallyControlled())
	{
		UpdateServerState(LastMove);
	}

	// We are the Client.
	if (GetOwner()->GetLocalRole() == ROLE_AutonomousProxy)
	{
		UnacknowledgedMoves.Add(LastMove);
		Server_SendMove(LastMove);
	}

	// Simulated Proxy Client side.
	if (GetOwner()->GetLocalRole() == ROLE_SimulatedProxy)
	{
		//MovementComponent->SimulateMove(ServerState.LastMove);
		ClientTick(DeltaTime);
	}
}

void UNetworkingComponent::UpdateServerState(const FVehicleMove& Move)
{
	// Send the canonical state to the other clients
	ServerState.LastMove = Move;
	ServerState.VehicleTransform = GetOwner()->GetActorTransform();
	ServerState.Velocity = MovementComponent->GetVelocity();
}

void UNetworkingComponent::ClientTick(float DeltaTime)
{
	ClientTimeSinceUpdate += DeltaTime;

	if (ClientTimeBetweenLastUpdates < KINDA_SMALL_NUMBER)
	{
		return;
	}

	if (MovementComponent == nullptr)
	{
		return;
	}

	float LerpRatio = ClientTimeSinceUpdate / ClientTimeBetweenLastUpdates;
	float VelocityToDerivativeInCms = ClientTimeBetweenLastUpdates * 100.0f;

	FVector TargetLocation = ServerState.VehicleTransform.GetLocation();
	FVector StartLocation = ClientStartTransform.GetLocation();
	FVector StartDerivative = ClientStartVelocity * VelocityToDerivativeInCms;
	FVector TargetDerivative = ServerState.Velocity * VelocityToDerivativeInCms;

	FVector NewLocation = FMath::CubicInterp(StartLocation, StartDerivative, TargetLocation, TargetDerivative, LerpRatio);
	//FVector NewLocation = FMath::LerpStable(StartLocation, TargetLocation, LerpRatio);
	GetOwner()->SetActorLocation(NewLocation);

	FVector NewDrivative = FMath::CubicInterpDerivative(StartLocation, StartDerivative, TargetLocation, TargetDerivative, LerpRatio);
	FVector NewVelocity = NewDrivative / VelocityToDerivativeInCms;
	MovementComponent->SetVelocity(NewVelocity);

	FQuat TargetRotation = ServerState.VehicleTransform.GetRotation();
	FQuat StartRotation = ClientStartTransform.GetRotation();
	FQuat NewRotation = FQuat::Slerp(StartRotation, TargetRotation, LerpRatio);
	GetOwner()->SetActorRotation(NewRotation);
}

void UNetworkingComponent::Server_SendMove_Implementation(FVehicleMove Move)
{
	if (MovementComponent == nullptr)
	{
		return;
	}

	MovementComponent->SimulateMove(Move);
	UpdateServerState(Move);
}

bool UNetworkingComponent::Server_SendMove_Validate(FVehicleMove Move)
{
	// TODO : Make validation better!
	return true;
}

void UNetworkingComponent::OnRep_ServerState()
{
	if (GetOwner()->GetLocalRole() == ROLE_AutonomousProxy)
	{
		AutonomousProxy_OnRep_ServerState();
	}
	else if (GetOwner()->GetLocalRole() == ROLE_SimulatedProxy)
	{
		SimulatedProxy_OnRep_ServerState();
	}
}

void UNetworkingComponent::SimulatedProxy_OnRep_ServerState()
{
	if (MovementComponent == nullptr)
	{
		return;
	}

	ClientTimeBetweenLastUpdates = ClientTimeSinceUpdate;
	ClientTimeSinceUpdate = 0.0f;

	ClientStartTransform = GetOwner()->GetActorTransform();

	// snapshot the velocity
	ClientStartVelocity = MovementComponent->GetVelocity();
}

void UNetworkingComponent::AutonomousProxy_OnRep_ServerState()
{
	if (MovementComponent == nullptr)
	{
		return;
	}

	GetOwner()->SetActorTransform(ServerState.VehicleTransform);
	MovementComponent->SetVelocity(ServerState.Velocity);

	ClearAcknowledgedMoves(ServerState.LastMove);

	for (const FVehicleMove& Move : UnacknowledgedMoves)
	{
		MovementComponent->SimulateMove(Move);
	}
}

void UNetworkingComponent::ClearAcknowledgedMoves(FVehicleMove LastMove)
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

