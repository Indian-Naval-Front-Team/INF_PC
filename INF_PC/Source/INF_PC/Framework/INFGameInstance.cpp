// Fill out your copyright notice in the Description page of Project Settings.


#include "INFGameInstance.h"
#include <INF_PC/UI/ServerBrowserPanel.h>
#include <INF_PC/UI/ServerRow.h>

const static FName SESSION_NAME = TEXT("My Session Game");
const static FName SERVER_NAME_SETTINGS_KEY = TEXT("ServerName");

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
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UINFGameInstance::OnJoinSessionsComplete);
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

void UINFGameInstance::Host(FServerData Val)
{
	ServerData = Val;

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

	World->ServerTravel("/Game/Maps/Workspace/WaterMap?listen");
}

void UINFGameInstance::OnDestroySessionsComplete(FName SessionName, bool Success)
{
	if (Success)
	{
		CreateSession();
	}
}

void UINFGameInstance::RefreshServerList()
{
	Find();
}

void UINFGameInstance::OnFindSessionsComplete(bool Success)
{	
	if (Success && SessionSearch.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Finished Finding Sessions!!!"));

		TArray<FServerData> ServerDatum;

		for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
		{
			UE_LOG(LogTemp, Warning, TEXT("Session found - %s with Ping %d"), *SearchResult.GetSessionIdStr(), SearchResult.PingInMs);
			FServerData Data;
			Data.ServerSize = SearchResult.Session.SessionSettings.NumPublicConnections;
			Data.CurrentPlayers = Data.ServerSize - SearchResult.Session.NumOpenPublicConnections;

			FString ValueData;
			if (SearchResult.Session.SessionSettings.Get(SERVER_NAME_SETTINGS_KEY, ValueData))
			{
				UE_LOG(LogTemp, Warning, TEXT("Data found in Settings %s"), *ValueData);
				Data.ServerName = ValueData;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed to fetch expected data!!"));
				Data.ServerName = "Could not find Server Name.";
			}
			//ServerNames.Add(SearchResult.GetSessionIdStr());
			ServerDatum.Add(Data);
		}

		if (ServerBrowserPanel != nullptr)
		{
			ServerBrowserPanel->SetServerList(ServerDatum);
		}
	}
}

void UINFGameInstance::CreateSession()
{
	if (SessionInterface.IsValid())
	{
		bool LanMatchTemp = false;
		if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
		{
			LanMatchTemp = true;
		}

		FOnlineSessionSettings SessionSettings;
		SessionSettings.bIsLANMatch = LanMatchTemp;
		SessionSettings.NumPublicConnections = 3;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bUsesPresence = true;
		SessionSettings.Set(SERVER_NAME_SETTINGS_KEY, ServerData.ServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
	}
}

void UINFGameInstance::Find()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch);

	if (SessionSearch.IsValid())
	{
		// TODO : Take off the line below when we stop using the SpaceWars AppID(480) and use the INF AppID instead.
		SessionSearch->MaxSearchResults = 100;
		
		// Add QueryParams here or in other words Filters to search for online sessions.
		//SessionSearch->bIsLanQuery = true;	// look for LAN matches
		// The line below lets us create QuerySettings as per the target platform for Multiplayer being used. Eg. Steam or XBox Live or EOS
			// For Steam Lobbies - The Set function invokes a Dictionary like operation to check whether Presence is turned on or not.
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

		UE_LOG(LogTemp, Warning, TEXT("Finding Sessions..."));
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

void UINFGameInstance::OnJoinSessionsComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (!SessionInterface.IsValid()) return;

	FString Address;
	if (!SessionInterface->GetResolvedConnectString(SessionName, Address))
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not got ConnectString!"));
		return;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Address %s"), *Address);
	}

	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 5.0f, FColor::Blue, FString::Printf(TEXT("Joining...%s"), *Address));

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

void UINFGameInstance::Join(UServerRow* ServerRow)
{
	if (!SessionInterface.IsValid())
	{
		return;
	}

	if (!SessionSearch.IsValid())
	{
		return;
	}

	// Join the Session by the Session Index.
	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[ServerRow->GetSessionIndex()]);
}
