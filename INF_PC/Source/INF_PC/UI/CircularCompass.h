// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Compass.h"
#include "CircularCompass.generated.h"

/**
 * 
 */
UCLASS()
class INF_PC_API UCircularCompass : public UCompass
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
protected:
	virtual void SetRotation() override;
};
