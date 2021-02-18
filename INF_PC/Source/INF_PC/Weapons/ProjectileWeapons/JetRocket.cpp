// Fill out your copyright notice in the Description page of Project Settings.


#include "INF_PC/Weapons/ProjectileWeapons/JetRocket.h"

#include "INF_PC/Components/MovementComponentMaster.h"
#include "INF_PC/Projectiles/ProjectileMaster.h"
#include "Kismet/GameplayStatics.h"

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
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
	// Jet Gun Firing 
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
