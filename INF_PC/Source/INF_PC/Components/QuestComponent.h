// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuestComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INF_PC_API UQuestComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UQuestComponent();

	UFUNCTION(BlueprintCallable)
	int32 GetCurrrentObjectiveIndex() const { return CurrentObjectiveIndex; }
	UFUNCTION(BlueprintCallable)
	void SetCurrentObjectiveIndex(const int32 Index) { CurrentObjectiveIndex = Index; }

	UFUNCTION(BlueprintCallable)
	class AQuestMaster* GetActiveQuest() const { return ActiveQuest; }
	UFUNCTION(BlueprintCallable)
	void SetActiveQuest(AQuestMaster* Quest) { ActiveQuest = Quest; }
	UFUNCTION(BlueprintCallable)
	void InitializeQuestComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	class AQuestMaster* ActiveQuest;
	int32 CurrentObjectiveIndex;
	UPROPERTY()
	class AVehicleMaster* OwningVehicle;

	UFUNCTION(BlueprintCallable, meta=(AllowPrivateAccess="true"))
	class AVehicleMaster* GetOwningVehicle() const;
};
