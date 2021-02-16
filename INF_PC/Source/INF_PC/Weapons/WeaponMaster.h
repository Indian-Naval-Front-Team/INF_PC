// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "INF_PC/Vehicles/VehicleMaster.h"

#include "WeaponMaster.generated.h"

class AProjectileMaster;

UENUM(BlueprintType)
enum class EWeaponType: uint8
{
	ShipAAGun,
    ShipCannon,
    Missile,
    ShipTorpedo,
    JetGun,
    JetRocket,
    BirdTorpedo,
    AAGunSingleBarrel,
    AAGunFourBarrel,
};

// Contains information of a single HitScan weapon Line Trace.
USTRUCT()
struct FProjectiles
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TEnumAsByte<EPhysicalSurface> SurfaceType;
	UPROPERTY()
	//class AProjectileMaster* Projectile = nullptr;
	class APawn* WeaponOwner;
};

UCLASS()
class INF_PC_API AWeaponMaster : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponMaster();
	
	virtual void StartFire() {};
	virtual void StopFire() {};

	void SetWeaponOwner(AVehicleMaster* VehicleRef);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category="Components")
	class UStaticMeshComponent* WeaponMainBody;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Weapon Setup")
	FName WeaponMuzzleSocketName;
	UPROPERTY(EditDefaultsOnly, Category="Weapon Setup")
	TSubclassOf<AActor> ProjectileClass;
	UPROPERTY(EditDefaultsOnly, Category="Weapon Setup")
	bool bAutoFire;
	UPROPERTY(EditDefaultsOnly, Category="Weapon Setup", meta = (EditCondition="bAutoFire"))
	float RateOfFire;

	
	FTimerHandle TimerHandle_TimeBetweenShots;
	float LastFiredTime;
	float TimeBetweenShots;

	UPROPERTY(Replicated)
	AVehicleMaster* WeaponOwner = nullptr;
	UPROPERTY(Replicated)
	AProjectileMaster* Projectile = nullptr;
	UMovementComponentMaster* MovementComponentOfOwner;
	void SetMovementComponentOfOwner();
	
	virtual void Fire() {};
};
