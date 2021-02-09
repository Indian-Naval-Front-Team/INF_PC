// Fill out your copyright notice in the Description page of Project Settings.


#include "INF_PC/Components/HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	DefaultHealth = 100.0f;
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* ComponentOwner = GetOwner();
	if (ComponentOwner)
	{
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::HandleTakeAnyDamage);
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

