// Fill out your copyright notice in the Description page of Project Settings.


#include "INFPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Engine/DemoNetDriver.h"
#include "INF_PC/UI/LobbyWidget.h"

AINFPlayerController::AINFPlayerController()
{
	const ConstructorHelpers::FClassFinder<UUserWidget>LobbyWidgetBP(TEXT("/Game/__Blueprints/Widgets/Multiplayer/WBP_LobbyWidget"));
	if (!ensure(LobbyWidgetBP.Class != nullptr)) return;

	LobbyWidgetClass = LobbyWidgetBP.Class;
}

void AINFPlayerController::ToggleLobbyWidget(bool ShowWidget)
{
	bShowLobbyWidget = ShowWidget;
}

void AINFPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AINFPlayerController, bShowLobbyWidget);
}

void AINFPlayerController::OnRep_ShowLobbyWidget()
{
	UE_LOG(LogTemp, Warning, TEXT("bLobbyWidget = %d"), bShowLobbyWidget);
	if (bShowLobbyWidget)
	{
		LobbyWidget = CreateWidget<ULobbyWidget>(this, LobbyWidgetClass);
		LobbyWidget->Setup();
	}
	else
	{
		LobbyWidget->RemoveFromParent();
	}
}
