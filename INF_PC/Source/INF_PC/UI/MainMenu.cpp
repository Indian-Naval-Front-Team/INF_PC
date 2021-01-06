// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "UMG/Public/Components/Button.h"
#include "UMG/Public/Components/WidgetSwitcher.h"
#include <INF_PC/UI/MultiplayerPanel.h>

bool UMainMenu::Initialize()
{
	const bool Success = Super::Initialize();

	if (!Success) return false;

	if (!ensure(BtnCampaign != nullptr)) return false;
	if (!ensure(BtnMultiplayer != nullptr)) return false;
	if (!ensure(BtnSettings != nullptr)) return false;
	if (!ensure(BtnExit != nullptr)) return false;

	BtnCampaign->OnClicked.AddDynamic(this, &UMainMenu::OnBtnCampaignClicked);
	BtnMultiplayer->OnClicked.AddDynamic(this, &UMainMenu::OnBtnMuliplayerClicked);
	BtnSettings->OnClicked.AddDynamic(this, &UMainMenu::OnBtnSettingsClicked);
	BtnExit->OnClicked.AddDynamic(this, &UMainMenu::OnBtnExitClicked);
	BtnBack_Campaign->OnClicked.AddDynamic(this, &UMainMenu::OnBtnBackClicked);
	BtnBack_Multiplayer->OnClicked.AddDynamic(this, &UMainMenu::OnBtnBackClicked);
	BtnBack_Settings->OnClicked.AddDynamic(this, &UMainMenu::OnBtnBackClicked);

	return true;
}


void UMainMenu::SetMenuInterface(IMenuInterface* MenuInterfaceVal)
{
	MenuInterfaceRef = MenuInterfaceVal;
}

void UMainMenu::Setup()
{
	this->AddToViewport();
	this->bIsFocusable = true;

	UWorld* World = GetWorld();

	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(this->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = true;
}

void UMainMenu::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);

	this->RemoveFromViewport();

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	const FInputModeGameOnly InputModeData;

	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = false;
}

void UMainMenu::OnBtnCampaignClicked()
{
	if (!ensure(MainMenuSwitcher != nullptr)) return;
	if (!ensure(Campaign != nullptr)) return;

	MainMenuSwitcher->SetActiveWidget(Campaign);
}

void UMainMenu::OnBtnMuliplayerClicked()
{
	if (!ensure(MainMenuSwitcher != nullptr)) return;
	if (!ensure(Multiplayer != nullptr)) return;
	
	MainMenuSwitcher->SetActiveWidget(Multiplayer);
}

void UMainMenu::OnBtnSettingsClicked()
{
	if (!ensure(MainMenuSwitcher != nullptr)) return;
	if (!ensure(Settings != nullptr)) return;

	MainMenuSwitcher->SetActiveWidget(Settings);
}

void UMainMenu::OnBtnBackClicked()
{
	if (!ensure(MainMenuSwitcher != nullptr)) return;
	if (!ensure(MainScreen != nullptr)) return;

	MainMenuSwitcher->SetActiveWidget(MainScreen);
}

void UMainMenu::OnBtnExitClicked()
{
}
