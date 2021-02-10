// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode_DefendCarrier.h"
#include <INF_PC/UI/LobbyWidget.h>
#include <INF_PC/Vehicles/JetMaster.h>
#include <INF_PC/Framework/INFGameInstance.h>


#include "INFGameState.h"
#include "INFPlayerController.h"

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

	UEngine* Engine = GetGameInstance()->GetEngine();
	if (!ensure(Engine != nullptr)) return;

	++NumberOfPlayers;

	Engine->AddOnScreenDebugMessage(0, 10.0f, FColor::Green, FString::Printf(TEXT("Num players joined : %d"), NumberOfPlayers));

	if (!LobbyWidget)
	{
		// Show the Lobby Widget on the Server.
		LobbyWidget = CreateWidget<ULobbyWidget>(GetWorld()->GetFirstPlayerController(), LobbyWidgetClass);
		LobbyWidget->Setup();
	}

	// Show the Lobby Widget on the Clients.
	AINFPlayerController* PC = Cast<AINFPlayerController>(NewPlayer);
	PC->ToggleLobbyWidget(true);

	// Collect the Player Controllers in the Table.
	PlayerControllerTable.AddUnique(PC);
	
	if (NumberOfPlayers >= 2)
	{
		Engine->AddOnScreenDebugMessage(0, 5.0f, FColor::Green, FString::Printf(TEXT("2 Players Joined!!")));
		UE_LOG(LogTemp, Warning, TEXT("Reached 2 players..."));

		GetWorldTimerManager().SetTimer(GameStartTimer, this, &AGameMode_DefendCarrier::StartGame, 5.0f);
	}

	// if (HasAuthority())
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("Player Logged IN %d!!"), INFGameInstance->NumberOfPlayers);
	// }
}

void AGameMode_DefendCarrier::StartGame()
{
	auto GameInstanceRef = Cast<UINFGameInstance>(GetGameInstance());

	if (GameInstanceRef == nullptr)
	{
		return;
	}

	// Remove the Lobby Widget from the Server.
	LobbyWidget->RemoveFromParent();

	// Remove the Lobby Widgets from all the Clients.
	for (auto PC : PlayerControllerTable)
	{
		PC->ToggleLobbyWidget();
	}

	PlayerControllerTable.Empty();
	
	UEngine* Engine = GetGameInstance()->GetEngine();
	if (!ensure(Engine != nullptr)) return;
	Engine->AddOnScreenDebugMessage(0, 5.0f, FColor::Green, FString::Printf(TEXT("Game started!!")));
	
	GameInstanceRef->StartSession();

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	bUseSeamlessTravel = true;
	bGameStarted = true;
	World->ServerTravel("/Game/Maps/Workspace/MultiplayerTestMap?listen");
}

void AGameMode_DefendCarrier::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	--NumberOfPlayers;
}
