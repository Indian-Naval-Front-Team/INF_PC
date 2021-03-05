// Fill out your copyright notice in the Description page of Project Settings.


#include "Bofors.h"


#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "INF_PC/Components/MovementComponentMaster.h"
#include "INF_PC/Projectiles/ProjectileMaster.h"
#include "Kismet/GameplayStatics.h"

ABofors::ABofors()
{
	Support = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Support"));
	Support->SetupAttachment(WeaponMainBody);
	Reticules = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Reticules"));
	Reticules->SetupAttachment(Support);
	Barrel_Left = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Barrel Left"));
	Barrel_Left->SetupAttachment(Support);
	Barrel_Right = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Barrel Right"));
	Barrel_Right->SetupAttachment(Support);
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CameraBoom->SetupAttachment(Support);
	MainCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Main Camera"));
	MainCamera->SetupAttachment(CameraBoom);
}

void ABofors::BeginPlay()
{
	Super::BeginPlay();
	TimeBetweenShots = 60.0f / RateOfFire;
}

void ABofors::AzimuthWeapon(float Value)
{
	const float NewAzimuth = FMath::Clamp(WeaponMainBody->GetRelativeRotation().Yaw + Value, -80.0f, 80.0f);
	WeaponMainBody->SetRelativeRotation(FRotator(WeaponMainBody->GetRelativeRotation().Pitch, NewAzimuth, WeaponMainBody->GetRelativeRotation().Roll));
}

void ABofors::ElevateWeapon(float Value)
{
	const float NewElevation = FMath::Clamp(Support->GetRelativeRotation().Pitch + Value, -10.0f, 60.0f);
	Support->SetRelativeRotation(FRotator(NewElevation, Support->GetRelativeRotation().Yaw, Support->GetRelativeRotation().Roll));
}

void ABofors::StartFire()
{
	const float FirstDelay = FMath::Max(LastFiredTime + TimeBetweenShots - GetWorld()->TimeSeconds, 0.0f);
	GetWorldTimerManager().SetTimer(TimerHandle_TimeBetweenShots, this, &ABofors::Fire,TimeBetweenShots, true, FirstDelay);
}

void ABofors::StopFire()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_TimeBetweenShots);
}

void ABofors::Fire()
{
	if (GetOwner())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
		// Bofors Firing
		Projectile = GetWorld()->SpawnActorDeferred<AProjectileMaster>(ProjectileClass, Barrel_Left->GetSocketTransform(WeaponMuzzleSocketName), this);
		//UE_LOG(LogTemp, Warning, TEXT("Instigator = %s"), *Projectile->GetInstigator()->GetName());
		
		if (Projectile)
		{
			Projectile->SetInstigator(Cast<APawn>(GetOwner()));
			Projectile->SetOwner(this);
			Projectile->SetProjectileSpeed(60000.0f);
			//UE_LOG(LogTemp, Warning, TEXT("Projectile speed = %f"), Projectile->GetVelocity().Size());
			UGameplayStatics::FinishSpawningActor(Projectile, Barrel_Left->GetSocketTransform(WeaponMuzzleSocketName));
		}
		
		Projectile = GetWorld()->SpawnActorDeferred<AProjectileMaster>(ProjectileClass, Barrel_Right->GetSocketTransform(WeaponMuzzleSocketName), this);
		
		if (Projectile)
		{
			Projectile->SetInstigator(Cast<APawn>(GetOwner()));
			Projectile->SetOwner(this);
			Projectile->SetProjectileSpeed(60000.0f);
			//UE_LOG(LogTemp, Warning, TEXT("Projectile speed = %f"), Projectile->GetVelocity().Size());
			UGameplayStatics::FinishSpawningActor(Projectile, Barrel_Right->GetSocketTransform(WeaponMuzzleSocketName));
		}
		
		LastFiredTime = GetWorld()->TimeSeconds;
	}
}

void ABofors::ServerFire_Implementation()
{
	Fire();
}

bool ABofors::ServerFire_Validate()
{
	return true;
}
