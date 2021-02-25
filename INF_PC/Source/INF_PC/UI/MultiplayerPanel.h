// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <INF_PC/UI/MenuWidget.h>
#include "MultiplayerPanel.generated.h"


/**
 * 
 */
UCLASS()
class INF_PC_API UMultiplayerPanel : public UMenuWidget
{
	GENERATED_BODY()

public:
	UMultiplayerPanel(const FObjectInitializer& ObjectInitializer);

	void Setup();
	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;

	
private:
	class UTexture* ActiveTabTexture;
	class UTexture* NormalTabTexture;

	UPROPERTY(meta = (BindWidget))
	class UButton* BtnServerBrowser;
	UPROPERTY(meta = (BindWidget))
	class UButton* BtnHostServer;
	UPROPERTY(meta = (BindWidget))
	class UButton* BtnTrainingIsland;
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* PanelSwitcher;
	UPROPERTY(meta = (BindWidget))
	class UWidget* Tab_ServerBrowser;
	UPROPERTY(meta = (BindWidget))
	class UWidget* Tab_HostServer;
	UPROPERTY(meta = (BindWidget))
	class UWidget* Tab_TrainingIsland;


	UFUNCTION()
	void OnBtnServerBrowserClicked();
	UFUNCTION()
	void OnBtnHostServerClicked();
	UFUNCTION()
	void OnBtnTrainingIslandClicked();

protected:
	virtual bool Initialize() override;
};
