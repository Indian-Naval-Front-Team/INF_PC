// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestComponent.h"


#include "INF_PC/Framework/INFPlayerController.h"
#include "INF_PC/Gameplay/QuestMaster.h"
#include "INF_PC/UI/MainHUD.h"
#include "INF_PC/UI/ObjectiveWidget.h"
#include "INF_PC/Vehicles/VehicleMaster.h"

// Sets default values for this component's properties
UQuestComponent::UQuestComponent()
{
}


// Called when the game starts
void UQuestComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OwningVehicle = Cast<AVehicleMaster>(GetOwner());
}

void UQuestComponent::InitializeQuestComponent()
{
	if (OwningVehicle)
	{
		UObjectiveWidget* ObjectiveWidget = OwningVehicle->GetPlayerControllerRef()->GetMainHUD()->GetObjectiveWidget();

		if (ObjectiveWidget)
		{
			TArray<FObjective> Objectives = ActiveQuest->GetObjectives();
			ObjectiveWidget->UpdateObjectiveText(Objectives[CurrentObjectiveIndex].ObjectiveDescription);
		}
	}
}

AVehicleMaster* UQuestComponent::GetOwningVehicle() const
{
	if (OwningVehicle)
	{
		return OwningVehicle;
	}

	return nullptr;
}
