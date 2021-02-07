// Fill out your copyright notice in the Description page of Project Settings.


#include "INF_PC/Weapons/Guns/JetGun.h"

#include "DrawDebugHelpers.h"
#include "Engine/DemoNetDriver.h"
#include "INF_PC/INF_PC.h"
#include "INF_PC/Projectiles/ProjectileMaster.h"
#include "INF_PC/Vehicles/VehicleMaster.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Custom Console Command for Linetracing Weapons
static int32 DebugWeaponDrawing = 0;
FAutoConsoleVariableRef DebugWeaponTrace(TEXT("INF.DebugWeaponTrace"),
                                           DebugWeaponDrawing,
                                         TEXT("Draw Debug Lines for Weapon Fire Linetracing"),
                                        ECVF_Cheat);

AJetGun::AJetGun() : AWeaponMaster()
{
	//SetReplicates(true);
	bReplicates = true;
	
	bAutoFire = true;
	RateOfFire = 600.0f;
}

void AJetGun::BeginPlay()
{
	Super::BeginPlay();
	//WeaponOwner = Cast<AVehicleMaster>(GetOwner());
	TimeBetweenShots = 60.0f / RateOfFire;
}

void AJetGun::StartFire()
{
	const float FirstDelay = FMath::Max(LastFiredTime + TimeBetweenShots - GetWorld()->TimeSeconds, 0.0f);
	GetWorldTimerManager().SetTimer(TimerHandle_TimeBetweenShots, this, &AJetGun::Fire,TimeBetweenShots, true, FirstDelay);
}

void AJetGun::StopFire()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_TimeBetweenShots);
}

void AJetGun::Fire()
{
	// if (GetLocalRole() < ROLE_Authority)
	// {
	// 	ServerFire();
	// 	//return;
	// }
	
	if (GetOwner())
	{																		// TODO : LINE-TRACE METHOD. DISCARDED FOR NOW! MAY USE IT LATER!!
		{
			// FVector EyeLocation;
			// FRotator EyeRotation;
			// WeaponOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
			//
			// const FVector TraceEnd = EyeLocation + (GetActorForwardVector() * 200000.0f);
			// FCollisionQueryParams QueryParams;
			// QueryParams.AddIgnoredActor(this);
			// QueryParams.AddIgnoredActor(WeaponOwner);
			// QueryParams.bTraceComplex = true;
			// QueryParams.bReturnPhysicalMaterial = true;
			//
			// // Particle "Target" parameter.
			// FVector TracerEndPoint = TraceEnd;
			// EPhysicalSurface SurfaceType = SurfaceType_Default;
			// FVector FinalImpactLocation;
			//
			// FHitResult Hit;
			// if(GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, TRACE_WEAPON))	// The Trace hit something.
			// {
			// 	FinalImpactLocation = Hit.ImpactPoint;
			// 	UE_LOG(LogTemp, Warning, TEXT("Hit something!!"));
			// }
			// else	// The Trace hit nothing, so we rotate the Muzzle Socket to the TraceEnd point.
			// {
			// 	FinalImpactLocation = TraceEnd;
			// }
			//
			// const FQuat MuzzleSocketRotation = (UKismetMathLibrary::FindLookAtRotation(WeaponMainBody->GetSocketLocation(WeaponMuzzleSocketName), FinalImpactLocation)).Quaternion();
			//
			// FActorSpawnParameters SpawnParams;
			// SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			//
			// if (DebugWeaponDrawing > 0)
			// {
			// 	DrawDebugLine(GetWorld(), EyeLocation, FinalImpactLocation, FColor::Red, true, 10.0f, 0, 5.0f);		
			// }
			//
			// FTransform ProjectileTransform;
			// ProjectileTransform.SetLocation(WeaponMainBody->GetSocketLocation(WeaponMuzzleSocketName));
			// ProjectileTransform.SetRotation(MuzzleSocketRotation);
			// ProjectileTransform.SetScale3D(FVector(1.0f));
		}

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
		// Jet Gun Firing
		Projectile = GetWorld()->SpawnActorDeferred<AProjectileMaster>(ProjectileClass, WeaponMainBody->GetSocketTransform(WeaponMuzzleSocketName), this);
		//UE_LOG(LogTemp, Warning, TEXT("Instigator = %s"), *Projectile->GetInstigator()->GetName());
		
		if (Projectile)
		{
			Projectile->SetInstigator(Cast<APawn>(GetOwner()));
			Projectile->SetOwner(this);
			Projectile->SetProjectileSpeed(Cast<AVehicleMaster>(GetOwner())->GetVehicleSpeed() * 100.0f + 40000.0f);
			UGameplayStatics::FinishSpawningActor(Projectile, WeaponMainBody->GetSocketTransform(WeaponMuzzleSocketName));
		}
		
		LastFiredTime = GetWorld()->TimeSeconds;
	}
}

void AJetGun::ServerFire_Implementation()
{
	Fire();
}

bool AJetGun::ServerFire_Validate()
{
	return true;
}
