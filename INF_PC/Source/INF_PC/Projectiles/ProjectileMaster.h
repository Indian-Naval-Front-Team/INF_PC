// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileMaster.generated.h"

UCLASS()
class INF_PC_API AProjectileMaster : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileMaster();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
	class UBoxComponent* CollisionComp;
	UPROPERTY(EditDefaultsOnly, Category="Components")
	class UStaticMeshComponent* ProjectileMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
	class UProjectileMovementComponent* ProjectileComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectile Setup")
	class UParticleSystem* ImpactParticles;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Damage")
	float Damage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	TSubclassOf<class UDamageType> DamageType;

	UFUNCTION()
	void HandleComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetProjectileSpeed(const float Speed);
};
