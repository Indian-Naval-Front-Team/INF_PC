// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "INF_PC/UI/MenuInterface.h"
#include <INF_PC/UI/MainMenu.h>
#include <INF_PC/Framework/GameModeParent.h>
#include <OnlineSubsystem.h>
#include <Interfaces/OnlineSessionInterface.h>
#include <OnlineSessionSettings.h>
#include <Templates/SharedPointer.h>
#include "INFGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class INF_PC_API UINFGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()
	
public:
	bool StartMatch{ false };
	uint32 NumberOfPlayers;

	UINFGameInstance(const FObjectInitializer& ObjectInitializer);
	UMainMenu* GetMainMenu() const { return MainMenu; };

	void SetServerBrowserPanel(class UServerBrowserPanel* Val) { ServerBrowserPanel = Val; };

	virtual void Init() override;

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

	UFUNCTION()
	void StartSession();

	void CreateSession();
	void CreateLobbyWidget(TSubclassOf<UUserWidget> LobbyWidgetClass, class AINFPlayerController* PC);
	void RemoveLobbyWidget();

private:
	TSubclassOf<class UUserWidget> MainMenuClass;
	UMainMenu* MainMenu;
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
	class UServerBrowserPanel* ServerBrowserPanel;
	FServerData ServerData;
	class ULobbyWidget* LobbyWidget;

	void OnCreateSessionsComplete(FName SessionName, bool Success);
	void OnDestroySessionsComplete(FName SessionName, bool Success);
	void OnFindSessionsComplete(bool Success);
	void OnJoinSessionsComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
};
