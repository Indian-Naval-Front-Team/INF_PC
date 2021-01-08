// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <INF_PC/UI/MenuWidget.h>
#include <INF_PC/UI/MainMenu.h>
#include "GameRulesWidget.generated.h"

/**
 * 
 */
UCLASS()
class INF_PC_API UGameRulesWidget : public UMenuWidget
{
	GENERATED_BODY()

public:
	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* BtnListenServer;
	UPROPERTY(meta = (BindWidget))
	class UButton* BtnDedicatedServer;
	
	UFUNCTION()
	void OnBtnListenServerClicked();
	UFUNCTION()
	void OnBtnDedicatedServerClicked();

protected:
	virtual bool Initialize();
};
