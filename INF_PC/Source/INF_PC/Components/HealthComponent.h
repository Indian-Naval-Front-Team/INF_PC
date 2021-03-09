// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

UENUM(BlueprintType)
enum class EEntityType : uint8
{
	W_Friend					UMETA(DisplayName = "Friend"),
    W_Foe						UMETA(DisplayName = "Foe"),
    W_Neutral					UMETA(DisplayName = "Neutral"),
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INF_PC_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHealthComponent();
	
	const FString EnumToString(const TCHAR* Enum, int32 EnumValue) const;
	EEntityType GetEntityType() const { return EntityType; }

	void SetEntityMarkerComponent(class UEntityMarkerComponent* EntityMarker) { EntityMarkerComponentRef = EntityMarker; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, Category="Setup")
	float Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Setup")
	float DefaultHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Setup")
	EEntityType EntityType;

	UFUNCTION()
    void HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

private:
	UPROPERTY()
	UEntityMarkerComponent* EntityMarkerComponentRef;
};
