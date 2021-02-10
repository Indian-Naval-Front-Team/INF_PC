// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "INFPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class INF_PC_API AINFPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AINFPlayerController();

	UFUNCTION()
	void ToggleLobbyWidget(bool ShowWidget = false);

private:
	TSubclassOf<UUserWidget> LobbyWidgetClass;
	class ULobbyWidget* LobbyWidget;
	
	UPROPERTY(ReplicatedUsing=OnRep_ShowLobbyWidget)
	bool bShowLobbyWidget;

	UFUNCTION()
	void OnRep_ShowLobbyWidget();
};
