// Fill out your copyright notice in the Description page of Project Settings.


#include "INFGameInstance.h"
#include <INF_PC/UI/MainMenu.h>

UINFGameInstance::UINFGameInstance(const FObjectInitializer& ObjectInitializer)
{
	UE_LOG(LogTemp, Warning, TEXT("GameInstance Constructor"));

	const ConstructorHelpers::FClassFinder<UUserWidget> MainMenuBPClass(TEXT("/Game/__Blueprints/Widgets/WBP_MainMenu"));
	if (!ensure(MainMenuBPClass.Class != nullptr)) return;

	MainMenuClass = MainMenuBPClass.Class;
}

void UINFGameInstance::Initialize()
{
	UE_LOG(LogTemp, Warning, TEXT("Found Main Menu Class - %s"), *MainMenuClass->GetName());
}

void UINFGameInstance::LoadMainMenu()
{
	if (!ensure(MainMenuClass != nullptr)) return;

	UMainMenu* MainMenu = CreateWidget<UMainMenu>(this, MainMenuClass);
	if (!ensure(MainMenu != nullptr)) return;

	MainMenu->Setup();
	MainMenu->SetMenuInterface(this);
}

void UINFGameInstance::Host()
{

}

void UINFGameInstance::Join()
{

}
