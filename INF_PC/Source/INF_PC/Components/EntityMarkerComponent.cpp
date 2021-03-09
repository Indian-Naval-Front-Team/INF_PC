// Fill out your copyright notice in the Description page of Project Settings.


#include "EntityMarkerComponent.h"



#include "INF_PC/UI/EntityCardWidget.h"
#include "INF_PC/Vehicles/VehicleMaster.h"
#include "Kismet/GameplayStatics.h"
#include <iomanip>

UEntityMarkerComponent::UEntityMarkerComponent()
{
	SetComponentTickEnabled(false);
	MinDistanceForRenderInKms = 5.0f;
	EntityName = FText::FromString("Nameless Entity");
	TickFrequency = 1.0f;

	Space = EWidgetSpace::Screen;
	DrawSize = FIntPoint(250, 60);
	bDrawAtDesiredSize = true;

	SetActive(true);
	SetHiddenInGame(true);
}

void UEntityMarkerComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (WidgetClass != nullptr)
	{
		PlayerPawn = Cast<AVehicleMaster>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		EntityCardWidget = Cast<UEntityCardWidget>(GetWidget());

		if (EntityCardWidget)
		{
			const float InitialDistance = ((GetComponentLocation() - PlayerPawn->GetActorLocation()).Size() / 100000.0f);
			const FText TxtInitialDistance = GetFloatAsTextWithPrecision(InitialDistance, 2);
			
			EntityCardWidget->SetupEntityCard(EntityType, EntityName, TxtInitialDistance, this);

			UHealthComponent* HealthCompRef = GetOwner()->FindComponentByClass<UHealthComponent>();

			if (HealthCompRef)
			{
				HealthCompRef->SetEntityMarkerComponent(this);
			}
		}
	}
}

void UEntityMarkerComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	const float TimeStep = (GetWorld()->GetTimeSeconds() - LastTickTime);
	if (PlayerPawn && GetOwner() != PlayerPawn &&  (TimeStep > TickFrequency))
	{
		const float Distance = (GetComponentLocation() - PlayerPawn->GetActorLocation()).Size() / 100000.0f;
		UE_LOG(LogTemp, Warning, TEXT("Distance = %f"), Distance);
		
		if (bHiddenInGame && (Distance < MinDistanceForRenderInKms))
		{
			SetHiddenInGame(false);
		}
		else if (!bHiddenInGame && (Distance > MinDistanceForRenderInKms))
		{
			SetHiddenInGame(true);
		}

		if (!bHiddenInGame && (Distance < MinDistanceForRenderInKms))
		{
			const FText DistanceTemp = GetFloatAsTextWithPrecision(Distance, 2);
			GetEntityCardWidget()->UpdateEntityCardDistance(DistanceTemp);
		}

		LastTickTime = GetWorld()->GetTimeSeconds();
	}
}

void UEntityMarkerComponent::ShowEntityCard()
{
	SetHiddenInGame(false);
}
