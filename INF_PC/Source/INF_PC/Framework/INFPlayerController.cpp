// Fill out your copyright notice in the Description page of Project Settings.


#include "INFPlayerController.h"

#include "INF_PC/Components/QuestComponent.h"
#include "INF_PC/Gameplay/QuestMaster.h"
#include "INF_PC/Vehicles/VehicleMaster.h"
#include "Kismet/GameplayStatics.h"

void AINFPlayerController::BeginPlay()
{
	Super::BeginPlay();
	TArray<AActor*> AllQuests;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AQuestMaster::StaticClass(), AllQuests);

	AQuestMaster* ActiveQuest = Cast<AQuestMaster>(AllQuests[0]);

	if (ActiveQuest)
	{
		class UQuestComponent* QuestComp = Cast<AVehicleMaster>(UGameplayStatics::GetPlayerPawn(this, 0))->GetQuestComponent();

		if (QuestComp)
		{
			QuestComp->SetActiveQuest(ActiveQuest);
		}
	}
}
