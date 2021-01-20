// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <INF_PC/Components/MovementComponentMaster.h>
#include "NetworkingComponent.generated.h"


UENUM()
enum class VehicleType
{
	SHIP		UMETA(DisplayName = "Ship"),
	JET			UMETA(DisplayName = "Jet"),
	CHOPPER		UMETA(DisplayName = "Chopper"),
	OTHERS		UMETA(DisplayName = "Others")
};

USTRUCT()
struct FVehicleState
{
	GENERATED_BODY()

	FVehicleState() {}

	UPROPERTY()
	FVehicleMove LastMove;
	UPROPERTY()
	FVector Velocity;
	UPROPERTY()
	FTransform VehicleTransform;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INF_PC_API UNetworkingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UNetworkingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(EditAnywhere)
	VehicleType TypeOfVehicle;

private:
	UPROPERTY()
	UMovementComponentMaster* MovementComponent;
	UPROPERTY(ReplicatedUsing = OnRep_ServerState)
	FVehicleState ServerState;
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SendMove(FVehicleMove Move);

	TArray<FVehicleMove> UnacknowledgedMoves;
	const APawn* OwningPawn;

	float ClientTimeSinceUpdate;
	float ClientTimeBetweenLastUpdates;
	FTransform ClientStartTransform;
	FVector ClientStartVelocity;

	UFUNCTION()
	void OnRep_ServerState();
	void SimulatedProxy_OnRep_ServerState();
	void AutonomousProxy_OnRep_ServerState();
	void ClearAcknowledgedMoves(FVehicleMove LastMove);
	void UpdateServerState(const FVehicleMove& Move);

	void ClientTick(float DeltaTime);
};
