// Fill out your copyright notice in the Description page of Project Settings.


#include "INF_PC/Weapons/ProjectileWeapons/JetRocket.h"

#include "DrawDebugHelpers.h"
#include "Components/WidgetComponent.h"
#include "INF_PC/Components/MovementComponentMaster.h"
#include "INF_PC/Framework/INFPlayerState.h"
#include "INF_PC/Projectiles/ProjectileMaster.h"
#include "INF_PC/Vehicles/CockpitMaster.h"
#include "INF_PC/Vehicles/JetMaster.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Custom Console Command for Linetracing Weapons
static int32 DebugWeaponDrawing = 0;
FAutoConsoleVariableRef DebugWeaponTrace(TEXT("INF.DebugWeaponTrace"),
                                           DebugWeaponDrawing,
                                         TEXT("Draw Debug Lines for Weapon Fire Linetracing"),
                                        ECVF_Cheat);

AJetRocket::AJetRocket()
{
	bReplicates = true;
}

void AJetRocket::BeginPlay()
{
	Super::BeginPlay();
}

void AJetRocket::StartFire()
{
	{
		FVector EyeLocation;
		FRotator EyeRotation;
		FVector TraceEnd;
		//OwningJet->GetActorEyesViewPoint(EyeLocation, EyeRotation);
		ACockpitMaster* CockpitTemp;
		CockpitTemp = OwningJet->GetCockpit();

		if (CockpitTemp)
		{
			EyeLocation = CockpitTemp->GetCockpitCameraPos();
			EyeRotation = CockpitTemp->GetCockpitCameraRot();
			TraceEnd = EyeLocation + (EyeRotation.Quaternion().GetAxisX() * 200000.0f);
		}

		{
			// if (WeaponOwner->GetPlayerStateRef()->GetCurrentPlayerStatus() == EPlayerStatus::W_MainVehicleView)
			// {
			// 	// EyeLocation = OwningJet->GetVehicleCrosshairWidget()->GetComponentLocation();
			// 	// EyeRotation = OwningJet->GetVehicleCrosshairWidget()->GetComponentRotation();
			// 	// TraceEnd = EyeLocation + (GetActorForwardVector() * 200000.0f);
			//
			// 	ACockpitMaster* CockpitTemp;
			// 	CockpitTemp = OwningJet->GetCockpit();
			//
			// 	if (CockpitTemp)
			// 	{
			// 		EyeLocation = CockpitTemp->GetCockpitCameraPos();
			// 		EyeRotation = CockpitTemp->GetCockpitCameraRot();
			// 		TraceEnd = EyeLocation + (EyeRotation.Quaternion().GetAxisX() * 200000.0f);
			// 	}
			// }
			// else
			// {
			// 	ACockpitMaster* CockpitTemp;
			// 	CockpitTemp = OwningJet->GetCockpit();
			//
			// 	if (CockpitTemp)
			// 	{
			// 		EyeLocation = CockpitTemp->GetCockpitCameraPos();
			// 		EyeRotation = CockpitTemp->GetCockpitCameraRot();
			// 		TraceEnd = EyeLocation + (EyeRotation.Quaternion().GetAxisX() * 200000.0f);
			// 	}
			// }	
		}
		
		FCollisionQueryParams QueryParams;
		TArray<const AActor*> IgnoredActors { this, WeaponOwner, OwningJet->GetCockpit()};
		QueryParams.AddIgnoredActors(IgnoredActors);
		//QueryParams.bTraceComplex = true;
		QueryParams.bReturnPhysicalMaterial = true;
		
		// Particle "Target" parameter.
		FVector TracerEndPoint = TraceEnd;
		EPhysicalSurface SurfaceType = SurfaceType_Default;
		FVector FinalImpactLocation;
		
		FHitResult Hit;
		if(GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, TRACE_WEAPON, QueryParams))	// The Trace hit something.
		{
			FinalImpactLocation = Hit.ImpactPoint;
			UE_LOG(LogTemp, Warning, TEXT("Hit %s"), *Hit.Actor->GetName());
		}
		else	// The Trace hit nothing, so we rotate the Muzzle Socket to the TraceEnd point.
		{
			FinalImpactLocation = TraceEnd;
		}
		//
		const FQuat OriginRot = (UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), FinalImpactLocation)).Quaternion();
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
		if (DebugWeaponDrawing > 0)
		{
			DrawDebugLine(GetWorld(), EyeLocation, FinalImpactLocation, FColor::Red, true, 10.0f, 0, 5.0f);		
		}
		//
		FTransform ProjectileTransform;
		ProjectileTransform.SetLocation(GetActorLocation());
		ProjectileTransform.SetRotation(OriginRot);
		ProjectileTransform.SetScale3D(FVector(1.0f));
		SetActorTransform(ProjectileTransform);
	}
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
	// Jet Rocket Firing 
	Projectile = GetWorld()->SpawnActorDeferred<AProjectileMaster>(ProjectileClass, GetActorTransform(), this);
	
	if (Projectile)
	{
		Projectile->SetInstigator(Cast<APawn>(GetOwner()));
		Projectile->SetOwner(this);
		Projectile->SetProjectileSpeed(MovementComponentOfOwner->GetVelocity().Size() * 100.0f + 10000.0f);
		UGameplayStatics::FinishSpawningActor(Projectile, GetActorTransform());
	}
}

void AJetRocket::StopFire()
{
}
