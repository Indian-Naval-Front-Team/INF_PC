// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "INF_PC/UI/MenuInterface.h"
#include <INF_PC/UI/MainMenu.h>
#include <OnlineSubsystem.h>
#include <Interfaces/OnlineSessionInterface.h>
#include <OnlineSessionSettings.h>
#include <Templates/SharedPointer.h>
#include "INFGameInstance.generated.h"
//
//UENUM()
//enum EServerRestartRules
//{
//	NEVERSHUTDOWN	  UMETA(DisplayName = "Never Shut Down"),
//	ONEMATCH		  UMETA(DisplayName = "Shut down after 1 match"),
//	THREEMATCH		  UMETA(DisplayName = "Shut down after 3 matches"),
//	FIVEMATCH		  UMETA(DisplayName = "Shut down after 5 matches"),
//};
//
//USTRUCT()
//struct FServerData
//{
//	GENERATED_BODY()
//
//	FString ServerName;
//	FString Password;
//	float MapDuration;
//	uint32 TicketsPerTeam;
//	uint16 ServerSize;
//	uint16 PlayersRequiredToStart;
//	EServerRestartRules ServerRestartRule;
//	bool IsLan;
//
//	uint16 CurrentPlayers;
//};

/**
 * 
 */
UCLASS()
class INF_PC_API UINFGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()
	
public:
	UINFGameInstance(const FObjectInitializer& ObjectInitializer);
	UMainMenu* GetMainMenu() const { return MainMenu; };

	void SetServerBrowserPanel(class UServerBrowserPanel* Val) { ServerBrowserPanel = Val; };

	virtual void Init();

	UFUNCTION(BlueprintCallable)
	void LoadMainMenu();

	UFUNCTION()
	void Host(FServerData Val);

	UFUNCTION()
	void Join(class UServerRow* ServerRow);

	UFUNCTION()
	void Find();

	UFUNCTION(BlueprintCallable)
	void RefreshServerList();

private:
	TSubclassOf<class UUserWidget> MainMenuClass;
	UMainMenu* MainMenu;
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
	class UServerBrowserPanel* ServerBrowserPanel;
	FServerData ServerData;

	void OnCreateSessionsComplete(FName SessionName, bool Success);
	void OnDestroySessionsComplete(FName SessionName, bool Success);
	void OnFindSessionsComplete(bool Success);
	void OnJoinSessionsComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	void CreateSession();
};
