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

void UINFGameInstance::Init()
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();

	if (Subsystem != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found Online Subsystem - %s"), *Subsystem->GetSubsystemName().ToString());
		SessionInterface = Subsystem->GetSessionInterface();

		if (SessionInterface.IsValid())
		{	
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UINFGameInstance::OnCreateSessionsComplete);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Found no Subsystem."));
	}

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
	if (SessionInterface.IsValid())
	{
		FOnlineSessionSettings SessionSettings;
		SessionInterface->CreateSession(0, TEXT("My Session Game"), SessionSettings);
	}
}

void UINFGameInstance::OnCreateSessionsComplete(FName SessionName, bool Success)
{
	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 5.0f, FColor::Green, TEXT("Hosting..."));

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	World->ServerTravel("/Game/Maps/Workspace/MultiplayerTestMap");
}

void UINFGameInstance::Join()
{

}
