// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QuestMaster.generated.h"

UENUM(BlueprintType)
enum class EObjectiveType: uint8
{
	None,
    Location,
    Kill,
    Decipher,
    Interact,
};

USTRUCT(BlueprintType)
struct FObjective
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective Setup")
	FText ObjectiveDescription;                             
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective Setup")
	EObjectiveType ObjectiveType;                           
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective Setup")
	AActor* LocationOrEnemyTarget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective Setup")
	int32 NumberOfTargets;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective Setup")
	TArray<AActor*> EntitiesToEnableAfterAchieved;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective Setup")
	float DelayBeforeNextObjective;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective Setup")
	float TimeGivenForObjective;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLocationReached, class ALocationTriggerMaster*, Location);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnemyKilled, class AVehicleMaster*, Enemy);

UCLASS()
class INF_PC_API AQuestMaster : public AActor
{
	GENERATED_BODY()
	
public:	
	AQuestMaster();

	UFUNCTION(BlueprintCallable, Category="Location Objective")
	void CallLocationObjectiveReached(ALocationTriggerMaster* Location);

	UFUNCTION(BlueprintCallable, Category="Location Objective")
    void CallEnemyKilledObjectiveAchieved(AVehicleMaster* Enemy);

	UFUNCTION(BlueprintCallable, Category="Quest Setup")
	TArray<FObjective> GetObjectives() const { return Objectives; }

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintImplementableEvent, Category="Location Objective")
    void OnLocationObjectiveReached(ALocationTriggerMaster* Location);
	UFUNCTION(BlueprintImplementableEvent, Category="Location Objective")
    void OnEnemyKilledObjectiveAchieved(AVehicleMaster* Enemy);

public:
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Setup", meta = (AllowPrivateAccess="true"))
	TArray<FObjective> Objectives;

	UPROPERTY(BlueprintAssignable, Category="Objective Delegates", meta=(AllowPrivateAccess="true"))
	FLocationReached LocationReachedDelegate;

	UPROPERTY(BlueprintAssignable, Category="Objective Delegates", meta=(AllowPrivateAccess="true"))
	FEnemyKilled EnemyKilledDelegate;

	UPROPERTY(BlueprintReadWrite, Category="Quest Setup", meta=(AllowPrivateAccess="true"))
	int32 CurrentKillCount;
};
