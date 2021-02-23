// Fill out your copyright notice in the Description page of Project Settings.


#include "INF_PC/Weapons/Guns/JetGun.h"
#include "DrawDebugHelpers.h"
#include "Components/ArrowComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/DemoNetDriver.h"
#include "INF_PC/Framework/INFPlayerController.h"
#include "INF_PC/Projectiles/ProjectileMaster.h"
#include "INF_PC/Vehicles/JetMaster.h"
#include "INF_PC/Vehicles/VehicleMaster.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


// static int32 DebugWeaponDrawing = 0;
// FAutoConsoleVariableRef DebugWeaponTrace(TEXT("INF.DebugWeaponTrace"),
//                                            DebugWeaponDrawing,
//                                          TEXT("Draw Debug Lines for Weapon Fire Linetracing"),
//                                         ECVF_Cheat);

AJetGun::AJetGun()
{
	//SetReplicates(true);
	bReplicates = true;
	
	bAutoFire = true;
	RateOfFire = 600.0f;
}

void AJetGun::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(true);
	TimeBetweenShots = 60.0f / RateOfFire;
	PlayerControllerRef = Cast<AINFPlayerController>(GetWorld()->GetFirstPlayerController());
	UE_LOG(LogTemp, Warning, TEXT("PlayerControllerRef = %s"), *PlayerControllerRef->GetName());
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
	if (GetOwner())
	{
		FTransform ProjectileTransform;
																			// TODO : LINE-TRACE METHOD. DISCARDED FOR NOW! MAY USE IT LATER!!
		{
			//WeaponOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
			// EyeLocation = OwningJet->GetVehicleFiringRefPoint()->GetComponentLocation();
			// EyeRotation = OwningJet->GetVehicleFiringRefPoint()->GetComponentRotation();
			//
			// const FVector TraceEnd = EyeLocation + (EyeRotation.Quaternion().GetForwardVector() * 200000.0f);
			// FCollisionQueryParams QueryParams;
			// QueryParams.AddIgnoredActor(this);
			// QueryParams.AddIgnoredActor(WeaponOwner);
			// //QueryParams.bTraceComplex = true;
			// QueryParams.bReturnPhysicalMaterial = true;
			//
			// // Particle "Target" parameter.
			// FVector TracerEndPoint = TraceEnd;
			// EPhysicalSurface SurfaceType = SurfaceType_Default;
			// FVector FinalImpactLocation;
			//
			// FHitResult Hit;
			// if(GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, TRACE_WEAPON, QueryParams))	// The Trace hit something.
			// {
			// 	FinalImpactLocation = Hit.ImpactPoint;
			// 	UE_LOG(LogTemp, Warning, TEXT("Hit %s"), *Hit.Actor->GetName());
			// }
			// else	// The Trace hit nothing, so we rotate the Muzzle Socket to the TraceEnd point.
			// {
			// 	UE_LOG(LogTemp, Warning, TEXT("Hit nothing!"));
			// 	FinalImpactLocation = TraceEnd;
			// }
			//
			// const FQuat MuzzleRotation = (UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), FinalImpactLocation)).Quaternion();
			//
			// FActorSpawnParameters SpawnParams;
			// SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			//
			// if (DebugWeaponDrawing > 0)
			// {
			// 	DrawDebugLine(GetWorld(), EyeLocation, FinalImpactLocation, FColor::Red, true, 10.0f, 0, 15.0f);		
			// }
			//
			//
			// ProjectileTransform.SetLocation(GetActorLocation());
			// // FRotator FinalRotation(FMath::Clamp(MuzzleRotation.Rotator().Pitch, -10.0f, -5.0f), MuzzleRotation.Rotator().Yaw, MuzzleRotation.Rotator().Roll);
			// // ProjectileTransform.SetRotation(FQuat(FinalRotation));
			// ProjectileTransform.SetRotation(MuzzleRotation);
			// ProjectileTransform.SetScale3D(FVector(1.0f));
			// SetActorRotation(ProjectileTransform.GetRotation());
		}

		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
			// Jet Gun Firing 
			Projectile = GetWorld()->SpawnActorDeferred<AProjectileMaster>(ProjectileClass, WeaponMainBody->GetSocketTransform(WeaponMuzzleSocketName), this);
			//UE_LOG(LogTemp, Warning, TEXT("Instigator = %s"), *Projectile->GetInstigator()->GetName());
		
			if (Projectile)
			{
				Projectile->SetInstigator(Cast<APawn>(GetOwner()));
				Projectile->SetOwner(this);
				Projectile->SetProjectileSpeed(MovementComponentOfOwner->GetVelocity().Size() * 100.0f + 40000.0f);
				//UE_LOG(LogTemp, Warning, TEXT("Projectile speed = %f"), Projectile->GetVelocity().Size());
				UGameplayStatics::FinishSpawningActor(Projectile, WeaponMainBody->GetSocketTransform(WeaponMuzzleSocketName));
			}
		}
		
		LastFiredTime = GetWorld()->TimeSeconds;
	}
}

void AJetGun::TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction& ThisTickFunction)
{
	//Super::TickActor(DeltaTime, TickType, ThisTickFunction);
}

void AJetGun::ServerFire_Implementation()
{
	Fire();
}

bool AJetGun::ServerFire_Validate()
{
	return true;
}
