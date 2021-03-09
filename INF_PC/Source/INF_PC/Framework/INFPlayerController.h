// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "INFPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class INF_PC_API AINFPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	class UMainHUD* GetMainHUD() const { return MainHUDRef; }
	UFUNCTION(BlueprintCallable)
	void SetMainHUD(class UMainHUD* Value) { MainHUDRef = Value; }
	
private:
	class UMainHUD* MainHUDRef;

protected:
	virtual void BeginPlay() override;;
};
