// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "INF_PC/UI/MenuInterface.h"
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

	virtual void Init();

	UFUNCTION(BlueprintCallable)
	void LoadMainMenu();

	UFUNCTION(BlueprintCallable)
	void Host();

	UFUNCTION(BlueprintCallable)
	void Join();

	UFUNCTION(BlueprintCallable)
	void Find();

private:
	TSubclassOf<class UUserWidget> MainMenuClass;
	class UMainMenu* MainMenu;
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	void OnCreateSessionsComplete(FName SessionName, bool Success);
	void OnDestroySessionsComplete(FName SessionName, bool Success);
	void OnFindSessionsComplete(bool Success);

	void CreateSession();
};
