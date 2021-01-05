// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class INF_PC_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetMenuInterface(IMenuInterface* MenuInterfaceVal);
	void Setup();

	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;

private:
	IMenuInterface* MenuInterfaceRef;

#pragma region Widget Component bindings for Main Menu.
	UPROPERTY(meta = (BindWidget))
	class UButton* BtnCampaign;

	UPROPERTY(meta = (BindWidget))
	class UButton* BtnMultiplayer;

	UPROPERTY(meta = (BindWidget))
	class UButton* BtnSettings;

	UPROPERTY(meta = (BindWidget))
	class UButton* BtnExit;

	UPROPERTY(meta = (BindWidget))
	class UButton* BtnBack_Campaign;

	UPROPERTY(meta = (BindWidget))
	class UButton* BtnBack_Multiplayer;

	UPROPERTY(meta = (BindWidget))
	class UButton* BtnBack_Settings;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* MainMenuSwitcher;

	UPROPERTY(meta = (BindWidget))
	class UWidget* MainScreen;
	
	UPROPERTY(meta = (BindWidget))
	class UWidget* Campaign;

	UPROPERTY(meta = (BindWidget))
	class UWidget* Multiplayer;

	UPROPERTY(meta = (BindWidget))
	class UWidget* Settings;
#pragma endregion Widget Component bindings

#pragma region Widget Component callbacks.
	UFUNCTION()
	void OnBtnCampaignClicked();

	UFUNCTION()
	void OnBtnMuliplayerClicked();

	UFUNCTION()
	void OnBtnSettingsClicked();

	UFUNCTION()
	void OnBtnExitClicked();

	UFUNCTION()
	void OnBtnBackClicked();
#pragma endregion Widget Component callbacks.

protected:
	bool Initialize() override;
};
