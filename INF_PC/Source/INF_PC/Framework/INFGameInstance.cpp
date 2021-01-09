// Fill out your copyright notice in the Description page of Project Settings.


#include "INFGameInstance.h"
#include <INF_PC/UI/MainMenu.h>

const static FName SESSION_NAME = TEXT("My Session Game");

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
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UINFGameInstance::OnFindSessionsComplete);
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UINFGameInstance::OnCreateSessionsComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UINFGameInstance::OnDestroySessionsComplete);

			// FIND SESSIONS
			/*SessionSearch = MakeShareable(new FOnlineSessionSearch);

			if (SessionSearch.IsValid())
			{
				UE_LOG(LogTemp, Warning, TEXT("Finding Sessions..."));
				SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
			}*/
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Found no Subsystem."));
	}
}

void UINFGameInstance::LoadMainMenu()
{
	if (!ensure(MainMenuClass != nullptr)) return;

	MainMenu = CreateWidget<UMainMenu>(this, MainMenuClass);
	if (!ensure(MainMenu != nullptr)) return;

	MainMenu->bIsFocusable = true;
	MainMenu->Setup();
	MainMenu->SetMenuInterface(this);
}

void UINFGameInstance::Host()
{
	if (SessionInterface.IsValid())
	{
		auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);

		if (ExistingSession != nullptr)
		{
			SessionInterface->DestroySession(SESSION_NAME);
		}
		else
		{
			CreateSession();
		}
	}
}

void UINFGameInstance::OnCreateSessionsComplete(FName SessionName, bool Success)
{
	if (!Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not create Session!"));
		return;
	}

	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 5.0f, FColor::Green, TEXT("Hosting..."));

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	World->ServerTravel("/Game/Maps/Workspace/MultiplayerTestMap");
}

void UINFGameInstance::OnDestroySessionsComplete(FName SessionName, bool Success)
{
	if (Success)
	{
		CreateSession();
	}
}

void UINFGameInstance::OnFindSessionsComplete(bool Success)
{	
	if (Success && SessionSearch.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Finished Finding Sessions!!!"));

		for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
		{
			UE_LOG(LogTemp, Warning, TEXT("Session found - %s with Ping %d"), *SearchResult.GetSessionIdStr(), SearchResult.PingInMs);
		}
	}
}

void UINFGameInstance::CreateSession()
{
	if (SessionInterface.IsValid())
	{
		FOnlineSessionSettings SessionSettings;
		SessionSettings.bIsLANMatch = true;
		SessionSettings.NumPublicConnections = 2;
		SessionSettings.bShouldAdvertise = true;

		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
	}
}

void UINFGameInstance::Join()
{

}

void UINFGameInstance::Find()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch);

	if (SessionSearch.IsValid())
	{
		// Add QueryParams here or in other words Filters to search for online sessions.
		SessionSearch->bIsLanQuery = true;	// look for LAN matches
		
		// The line below lets us create QuerySettings as per the target platform for Multiplayer being used. Eg. Steam or XBox Live or EOS
		//SessionSearch->QuerySettings.Set()

		UE_LOG(LogTemp, Warning, TEXT("Finding Sessions..."));
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}
