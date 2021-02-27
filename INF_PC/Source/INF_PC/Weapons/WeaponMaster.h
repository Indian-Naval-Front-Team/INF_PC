// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "WeaponMaster.generated.h"

#define TRACE_WEAPON  ECC_GameTraceChannel2



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
	BirdBomb,
    BirdTorpedo,
    AAGunSingleBarrel,
    AAGunFourBarrel,
};

USTRUCT(BlueprintType)
struct FWeaponSetup
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponClass Setup")
	EWeaponType WeaponType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponClass Setup")
	TSubclassOf<class AWeaponMaster> WeaponClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponClass Setup")
	int NumWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponClass Setup")
	TArray<FName> SocketNames;
};

// Contains information of a single HitScan weapon Line Trace.
USTRUCT(BlueprintType)
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

	void SetMuzzleSocketName(const FName SocketName) { WeaponMuzzleSocketName = SocketName; }
	void SetWeaponOwner(class AVehicleMaster* VehicleRef);
	EWeaponType GetWeaponType() const { return WeaponType; }
	AProjectileMaster* GetProjectile() { return Projectile; }
	TSubclassOf<AActor> GetProjectileClass() const { return ProjectileClass; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category="Components")
	class UStaticMeshComponent* WeaponMainBody;

	UPROPERTY(EditDefaultsOnly, Category="WeaponClass Setup")
	EWeaponType WeaponType;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="WeaponClass Setup")
	FName WeaponMuzzleSocketName;
	UPROPERTY(EditDefaultsOnly, Category="WeaponClass Setup")
	TSubclassOf<AActor> ProjectileClass;
	UPROPERTY(EditDefaultsOnly, Category="WeaponClass Setup")
	bool bAutoFire;
	UPROPERTY(EditDefaultsOnly, Category="WeaponClass Setup", meta = (EditCondition="bAutoFire"))
	float RateOfFire;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="WeaponClass Setup", meta = (EditCondition="bAutoFire"))
	int32 MaxShotsBeforeHeatUp;
	UPROPERTY(EditDefaultsOnly, Category="WeaponClass Setup")
	bool bIsMissileRangedWeapon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="WeaponClass Setup", meta = (EditCondition="!bIsMissileRangedWeapon"))
	int32 MaxShotsBeforeReload;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="WeaponClass Setup|Ammo", meta = (EditCondition="bIsMissileRangedWeapon"))
	int32 MaxAvailableProjectiles;

	UFUNCTION(BlueprintCallable)
	virtual void AzimuthWeapon(float Value) {};
	UFUNCTION(BlueprintCallable)
	virtual void ElevateWeapon(float Value) {};
	
	FTimerHandle TimerHandle_TimeBetweenShots;
	float LastFiredTime;
	float TimeBetweenShots;

	UPROPERTY(Replicated)
	class AVehicleMaster* WeaponOwner = nullptr;
	UPROPERTY(Replicated)
	AProjectileMaster* Projectile = nullptr;
	class UMovementComponentMaster* MovementComponentOfOwner;
	void SetMovementComponentOfOwner();
	
	virtual void Fire() {};

	class AINFPlayerController* PlayerControllerRef;
};
