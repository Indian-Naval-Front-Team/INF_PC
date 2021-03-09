// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LocationTriggerMaster.h"
#include "BoxLocationTrigger.generated.h"

/**
 * 
 */
UCLASS()
class INF_PC_API ABoxLocationTrigger : public ALocationTriggerMaster
{
	GENERATED_BODY()
	
public:
	ABoxLocationTrigger();
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Location Trigger", meta=(AllowPrivateAccess="true"))
	class UBoxComponent* BoxCollider;
};
