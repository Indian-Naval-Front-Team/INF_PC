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

	if (TypeOfVehicle == VehicleType::JET)
	{
		MovementComponent = GetOwner()->FindComponentByClass<UJetMovementComponent>();
	}
	else if (TypeOfVehicle == VehicleType::OTHERS)
	{
		MovementComponent = GetOwner()->FindComponentByClass<UMovementComponentMaster>();
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

	if (GetOwner()->GetLocalRole() == ROLE_AutonomousProxy)
	{
		FVehicleMove Move = MovementComponent->CreateMove(DeltaTime);
		UnacknowledgedMoves.Add(Move);
		MovementComponent->SimulateMove(Move);
		Server_SendMove(Move);
	}

	// We are the server and also in control of the Pawn.
	if (GetOwner()->GetLocalRole() == ROLE_Authority && OwningPawn->IsLocallyControlled())
	{
		FVehicleMove Move = MovementComponent->CreateMove(DeltaTime);
		Server_SendMove(Move);
	}

	if (GetOwner()->GetLocalRole() == ROLE_SimulatedProxy)
	{
		MovementComponent->SimulateMove(ServerState.LastMove);
	}
}

void UNetworkingComponent::Server_SendMove_Implementation(FVehicleMove Move)
{
	if (MovementComponent == nullptr)
	{
		return;
	}

	MovementComponent->SimulateMove(Move);

	// Send the canonical state to the other clients
	ServerState.LastMove = Move;
	ServerState.VehicleTransform = GetOwner()->GetActorTransform();
	ServerState.Velocity = MovementComponent->GetVelocity();
}

bool UNetworkingComponent::Server_SendMove_Validate(FVehicleMove Move)
{
	// TODO : Make validation better!
	return true;
}

void UNetworkingComponent::OnRep_ServerState()
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
