// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <INF_PC/UI/MenuWidget.h>
#include "HostServerPanel.generated.h"

/**
 * 
 */
UCLASS()
class INF_PC_API UHostServerPanel : public UMenuWidget
{
	GENERATED_BODY()
	
public:
	UHostServerPanel(const FObjectInitializer& ObjectInitializer);

	void Setup();
	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;

private:
	class UTexture* ActiveTabTexture;
	class UTexture* NormalTabTexture;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* BtnSelectGameMode;
	UPROPERTY(meta = (BindWidget))
	class UButton* BtnSetGameRules;
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* PanelSwitcher;
	UPROPERTY(meta = (BindWidget))
	class UWidget* Tab_SelectGameMode;
	UPROPERTY(meta = (BindWidget))
	class UWidget* Tab_SetGameRules;

	UFUNCTION()
	void OnBtnSelectGameModeClicked();
	UFUNCTION()
	void OnBtnSetGameRulesClicked();

protected:
	bool Initialize() override;
};
