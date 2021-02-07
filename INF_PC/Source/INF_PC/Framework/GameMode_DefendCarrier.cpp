// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode_DefendCarrier.h"
#include <INF_PC/UI/LobbyWidget.h>
#include <INF_PC/Vehicles/JetMaster.h>
#include <INF_PC/Framework/INFGameInstance.h>

AGameMode_DefendCarrier::AGameMode_DefendCarrier(const FObjectInitializer& ObjectInitializer) : AGameModeParent()
{
	const ConstructorHelpers::FClassFinder<AJetMaster> SeahawkBP(TEXT("/Game/__Blueprints/Vehicles/Indian/Jets/BP_Seahawk"));
	if (!ensure(SeahawkBP.Class != nullptr)) return;

	DefaultPawnClass = SeahawkBP.Class;

	const ConstructorHelpers::FClassFinder<UUserWidget> LobbyWidgetBP(TEXT("/Game/__Blueprints/Widgets/Multiplayer/WBP_LobbyWidget"));
	if (!ensure(LobbyWidgetBP.Class != nullptr)) return;

	LobbyWidgetClass = LobbyWidgetBP.Class;
}

void AGameMode_DefendCarrier::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	INFGameInstance = Cast<UINFGameInstance>(GetWorld()->GetGameInstance());
	if (!ensure(INFGameInstance != nullptr)) return;

	++NumberOfPlayers;

	if (NumberOfPlayers >= 2)
	{
		UE_LOG(LogTemp, Warning, TEXT("Reached 2 players..."));
		LobbyWidget->RemoveFromParent();

		INFGameInstance->StartMatch = true;

		UWorld* World = GetWorld();
		if (!ensure(World != nullptr)) return;

		bUseSeamlessTravel = true;
		World->ServerTravel("/Game/Maps/Workspace/MultiplayerTestMap?listen");
	}
	else
	{
		LobbyWidget = CreateWidget<ULobbyWidget>(GetWorld()->GetFirstPlayerController(), LobbyWidgetClass);
		LobbyWidget->Setup();
	}

	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Logged IN %d!!"), INFGameInstance->NumberOfPlayers);
	}
}

void AGameMode_DefendCarrier::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	--NumberOfPlayers;
}
