// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameModeParent.h"
#include "GameMode_DefendCarrier.generated.h"

/**
 * 
 */
UCLASS()
class INF_PC_API AGameMode_DefendCarrier : public AGameModeParent
{
	GENERATED_BODY()
	
public:
	AGameMode_DefendCarrier(const FObjectInitializer& ObjectInitializer);

	void PostLogin(APlayerController* NewPlayer) override;
	void Logout(AController* Exiting) override;

private:
	TSubclassOf<UUserWidget> LobbyWidgetClass;
	class ULobbyWidget* LobbyWidget;
	class UINFGameInstance* INFGameInstance;

	uint32 NumberOfPlayers{ 0 };
};
