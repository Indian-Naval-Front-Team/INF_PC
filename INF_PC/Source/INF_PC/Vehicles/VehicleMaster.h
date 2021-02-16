// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include <INF_PC/Components/MovementComponentMaster.h>
#include <INF_PC/Components/NetworkingComponent.h>
#include "VehicleMaster.generated.h"



UCLASS()
class INF_PC_API AVehicleMaster : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AVehicleMaster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Components
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
	class UStaticMeshComponent* VehicleBody;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
	class USpringArmComponent* CameraBoom;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
	class UCameraComponent* MainCamera;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
	class UWidgetComponent* CrosshairWidget;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
	class UHealthComponent* HealthComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Vehicle Setup|Weapons")
	TArray<TSubclassOf<class AWeaponMaster>> Arsenal;

	// Called when 'W' or 'S' keys are pressed on the Vehicle.
	UFUNCTION()
	virtual void ThrustVehicle(float Value) {};

	// Called when the 'A' or 'D' keys are pressed to Yaw the vehicle.
	UFUNCTION()
	virtual void YawVehicle(float Value) {};

	// Called when the Mouse is moved up/down to Pitch the Vehicle Up/Down.
	UFUNCTION()
	virtual void PitchVehicle(float Value) {};

	// Called when the Mouse is moved left/right to Roll the Vehicle Left/Right.
	UFUNCTION()
	virtual void RollVehicle(float Value) {};

	virtual void FireSelectedWeapon() {};
	virtual void StopFiringSelectedWeapon() {};

	UPROPERTY(VisibleDefaultsOnly)
	UMovementComponentMaster* VehicleMovementComponent;
	UPROPERTY(VisibleAnywhere)
	UNetworkingComponent* NetworkingComponent;

	UPROPERTY(Replicated)
	class AWeaponMaster* LeftGun;
	UPROPERTY(Replicated)
	AWeaponMaster* RightGun;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual FVector GetPawnViewLocation() const override { return FVector::ZeroVector; };
	float GetVehicleSpeed() const
	{
		return VehicleMovementComponent->GetVelocity().Size();
	}
	class AWeaponMaster* GetLeftGun() const { return LeftGun; }
	class AWeaponMaster* GetRightGun() const { return RightGun; }
};
