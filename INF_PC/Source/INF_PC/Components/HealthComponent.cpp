// Fill out your copyright notice in the Description page of Project Settings.


#include "INF_PC/Components/HealthComponent.h"

UHealthComponent::UHealthComponent()
{
	DefaultHealth = 100.0f;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
	AActor* ComponentOwner = GetOwner();
	if (ComponentOwner)
	{
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::HandleTakeAnyDamage);
		const FString EntityString = StaticEnum<EEntityType>()->GetValueAsString(EntityType);
		const TArray<FName> EntityTypeTag { FName(EntityString) };
		ComponentOwner->Tags = EntityTypeTag;
	}

	Health = DefaultHealth;
}

void UHealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f)
	{
		return;
	}

	// Update Health Clamped.
	Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);

	UE_LOG(LogTemp, Warning, TEXT("Health : %f"), Health);

	if (Health <= 0.0f)
	{
		GetOwner()->Destroy();
	}	
}


