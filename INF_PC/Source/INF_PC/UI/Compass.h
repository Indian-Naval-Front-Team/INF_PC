// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Compass.generated.h"

/**
 * 
 */
UCLASS()
class INF_PC_API UCompass : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	class UImage* GetCompassImage() const { return ImgCompass; }

protected:
	virtual void SetRotation() {};
	class AVehicleMaster* PlayerVehicle;

private:
	UPROPERTY(meta = (BindWidget))
	class UImage* ImgCompass;
};
