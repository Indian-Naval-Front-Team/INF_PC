// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MultiplayerPanel.generated.h"

/**
 * 
 */
UCLASS()
class INF_PC_API UMultiplayerPanel : public UUserWidget
{
	GENERATED_BODY()

public:
	UMultiplayerPanel(const FObjectInitializer& ObjectInitializer);

	void Setup();
	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;

	
private:
	class UTexture2D* ActiveTabTexture;
	class UTexture2D* NormalTabTexture;

	UPROPERTY(meta = (BindWidget))
	class UButton* BtnServerBrowser;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* BtnHostServer;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* BtnGameModes;
	
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* PanelSwitcher;

	UPROPERTY(meta = (BindWidget))
	class UWidget* Tab_ServerBrowser;

	UPROPERTY(meta = (BindWidget))
	class UWidget* Tab_HostServer;

	UPROPERTY(meta = (BindWidget))
	class UWidget* Tab_GameModes;


	UFUNCTION()
	void OnBtnServerBrowserClicked();

	UFUNCTION()
	void OnBtnHostServerClicked();

	UFUNCTION()
	void OnBtnGameModesClicked();

protected:
	bool Initialize() override;
};
