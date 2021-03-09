// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBase.h"
#include "LocationTriggerMaster.generated.h"

/**
 * 
 */
UCLASS()
class INF_PC_API ALocationTriggerMaster : public ATriggerBase
{
	GENERATED_BODY()

public:
	ALocationTriggerMaster();
	
private:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	bool bIsObjectiveTrigger;
};
