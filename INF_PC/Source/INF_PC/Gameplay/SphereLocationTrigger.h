// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LocationTriggerMaster.h"
#include "SphereLocationTrigger.generated.h"

/**
 * 
 */
UCLASS()
class INF_PC_API ASphereLocationTrigger : public ALocationTriggerMaster
{
	GENERATED_BODY()

public:
	ASphereLocationTrigger();
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Location Trigger", meta=(AllowPrivateAccess="true"))
	class USphereComponent* SphereCollider;
};
