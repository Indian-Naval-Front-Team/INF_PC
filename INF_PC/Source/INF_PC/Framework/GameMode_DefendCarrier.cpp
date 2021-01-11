// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode_DefendCarrier.h"
#include <INF_PC/UI/LobbyWidget.h>
#include <INF_PC/TP_Flying/TP_FlyingPawn.h>

AGameMode_DefendCarrier::AGameMode_DefendCarrier(const FObjectInitializer& ObjectInitializer) : AGameModeParent()
{
	DefaultPawnClass = ATP_FlyingPawn::StaticClass();

	const ConstructorHelpers::FClassFinder<UUserWidget> LobbyWidgetBP(TEXT("/Game/__Blueprints/Widgets/Multiplayer/WBP_LobbyWidget"));
	if (!ensure(LobbyWidgetBP.Class != nullptr)) return;

	LobbyWidgetClass = LobbyWidgetBP.Class;
}

void AGameMode_DefendCarrier::PostLogin(APlayerController* NewPlayer)
{
	if (NumberOfPlayers >= 2)
	{
		UE_LOG(LogTemp, Warning, TEXT("Reached 2 players..."));
	}
	else
	{
		++NumberOfPlayers;
		LobbyWidget = CreateWidget<ULobbyWidget>(GetWorld()->GetFirstPlayerController(), LobbyWidgetClass);
		LobbyWidget->Setup();
	}
}

void AGameMode_DefendCarrier::Logout(AController* Exiting)
{
	--NumberOfPlayers;
}
