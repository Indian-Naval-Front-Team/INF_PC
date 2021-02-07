// Fill out your copyright notice in the Description page of Project Settings.


#include "INF_PC/Projectiles/ProjectileMaster.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AProjectileMaster::AProjectileMaster()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComponent"));

	RootComponent = ProjectileMesh;
}

// Called when the game starts or when spawned
void AProjectileMaster::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AProjectileMaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectileMaster::SetProjectileSpeed(const float Speed)
{
	//UE_LOG(LogTemp, Warning, TEXT("Final Speed = %f"), Speed);
	ProjectileComponent->InitialSpeed = Speed;
	//ProjectileComponent->SetVelocityInLocalSpace(FVector(Speed, 0.0f, 0.0f));
}

