// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "INF_PC/UI/MenuInterface.h"
#include <INF_PC/UI/MainMenu.h>
#include <INF_PC/UI/ServerBrowserPanel.h>
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
	UINFGameInstance(const FObjectInitializer& ObjectInitializer);
	UMainMenu* GetMainMenu() const { return MainMenu; };

	void SetServerBrowserPanel(UServerBrowserPanel* Val) { ServerBrowserPanel = Val; };

	virtual void Init();

	UFUNCTION(BlueprintCallable)
	void LoadMainMenu();

	UFUNCTION(BlueprintCallable)
	void Host();

	UFUNCTION(BlueprintCallable)
	void Join(class UServerRow* ServerRow);

	UFUNCTION(BlueprintCallable)
	void Find();

	UFUNCTION(BlueprintCallable)
	void RefreshServerList();

private:
	TSubclassOf<class UUserWidget> MainMenuClass;
	UMainMenu* MainMenu;
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
	UServerBrowserPanel* ServerBrowserPanel;

	void OnCreateSessionsComplete(FName SessionName, bool Success);
	void OnDestroySessionsComplete(FName SessionName, bool Success);
	void OnFindSessionsComplete(bool Success);
	void OnJoinSessionsComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	void CreateSession();
};
