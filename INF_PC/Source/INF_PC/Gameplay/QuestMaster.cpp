// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestMaster.h"

// Sets default values
AQuestMaster::AQuestMaster()
{
}

// Called when the game starts or when spawned
void AQuestMaster::BeginPlay()
{
	Super::BeginPlay();
	LocationReachedDelegate.AddDynamic(this, &AQuestMaster::OnLocationObjectiveReached);
	EnemyKilledDelegate.AddDynamic(this, &AQuestMaster::OnEnemyKilledObjectiveAchieved);
}

// Called every frame
void AQuestMaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AQuestMaster::CallLocationObjectiveReached(ALocationTriggerMaster* Location)
{
	LocationReachedDelegate.Broadcast(Location);
}

void AQuestMaster::CallEnemyKilledObjectiveAchieved(AVehicleMaster* Enemy)
{
	EnemyKilledDelegate.Broadcast(Enemy);
}

