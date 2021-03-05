// Fill out your copyright notice in the Description page of Project Settings.


#include "Radar.h"



#include "DrawDebugHelpers.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "INF_PC/INF_PC.h"
#include "INF_PC/Vehicles/VehicleMaster.h"
#include "Kismet/GameplayStatics.h"
#include "INF_PC/Components/HealthComponent.h"

void URadar::NativeConstruct()
{
	Super::NativeConstruct();
	SelectedVehicleRef = Cast<AVehicleMaster>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}

bool URadar::GetAllActorsForRadar()
{
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), OutActors);

	if (OutActors.Num() > 0)
	{
		for (AActor* CurrentActor : OutActors)
		{
			if(CurrentActor->FindComponentByClass<UHealthComponent>() != nullptr && CurrentActor != SelectedVehicleRef)
			{
				AllRadarActors.AddUnique(CurrentActor);

				switch (CurrentActor->FindComponentByClass<UHealthComponent>()->GetEntityType())
				{
					case EEntityType::W_Friend:
						AllFriendActors.AddUnique(CurrentActor);
						break;
					case EEntityType::W_Foe:
						AllFoeActors.AddUnique(CurrentActor);
						break;
					case EEntityType::W_Neutral:
						AllNeutralActors.AddUnique(CurrentActor);
						break;
					default: ;
				}
			}
		}
	}

	return (AllRadarActors.Num() > 0);
}

void URadar::ClearAllTables()
{
	AllRadarActors.Empty();
	AllFoeActors.Empty();
	AllFriendActors.Empty();
	AllNeutralActors.Empty();
}
