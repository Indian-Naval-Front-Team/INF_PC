// Fill out your copyright notice in the Description page of Project Settings.


#include "INF_PC/Components/HealthComponent.h"



#include "EntityMarkerComponent.h"
#include "QuestComponent.h"
#include "INF_PC/Gameplay/QuestMaster.h"
#include "INF_PC/UI/EntityCardWidget.h"
#include "INF_PC/Vehicles/VehicleMaster.h"


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
	UE_LOG(LogTemp, Warning, TEXT("Health : %f"), (1.0f - Health / 100.0f));

	if (EntityMarkerComponentRef)
	{
		EntityMarkerComponentRef->GetEntityCardWidget()->UpdateEntityCardHealth(1.0f - Health / 100.0f);
	}

	//UE_LOG(LogTemp, Warning, TEXT("Health : %f"), Health);
	//UE_LOG(LogTemp, Warning, TEXT("DamageCauser = %s"), *DamageCauser->GetOwner()->GetOwner()->GetName());

	if (Health <= 0.0f)
	{
		AVehicleMaster* Vehicle = Cast<AVehicleMaster>(DamageCauser->GetOwner()->GetOwner());
		
		if (Vehicle)
		{
			Vehicle->GetQuestComponent()->GetActiveQuest()->CallEnemyKilledObjectiveAchieved(Cast<AVehicleMaster>(GetOwner()));	
		}
		 
		GetOwner()->Destroy();
	}	
}


