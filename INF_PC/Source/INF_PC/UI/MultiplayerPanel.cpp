// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerPanel.h"
#include "UMG/Public/Components/Button.h"
#include "UMG/Public/Components/WidgetSwitcher.h"
#include "Runtime/Engine/Classes/Engine/Texture2D.h"


UMultiplayerPanel::UMultiplayerPanel(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	const ConstructorHelpers::FObjectFinder<UTexture2D>ActiveTabTextureTemp(TEXT("/Game/_Assets/UI/GenericButton_Hovered"));
	const ConstructorHelpers::FObjectFinder<UTexture2D>NormalTabTextureTemp(TEXT("/Game/_Assets/UI/GenericButton_Normal"));
	
	if (!ensure(ActiveTabTextureTemp.Object != nullptr)) return;
	if (!ensure(NormalTabTextureTemp.Object != nullptr)) return;

	ActiveTabTexture = ActiveTabTextureTemp.Object;
	NormalTabTexture = NormalTabTextureTemp.Object;
}

bool UMultiplayerPanel::Initialize()
{
	const bool Success = Super::Initialize();

	if (!Success) return false;

	if (!ensure(BtnServerBrowser != nullptr)) return false;
	if (!ensure(BtnHostServer != nullptr)) return false;
	if (!ensure(BtnGameModes != nullptr)) return false;

	BtnServerBrowser->OnClicked.AddDynamic(this, &UMultiplayerPanel::OnBtnServerBrowserClicked);
	BtnHostServer->OnClicked.AddDynamic(this, &UMultiplayerPanel::OnBtnHostServerClicked);
	BtnGameModes->OnClicked.AddDynamic(this, &UMultiplayerPanel::OnBtnGameModesClicked);

	return true;
}

void UMultiplayerPanel::Setup()
{
	/*FTimerHandle OutTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(OutTimerHandle, 0.15f, false);*/

	if (!ensure(PanelSwitcher != nullptr)) return;
	if (!ensure(Tab_ServerBrowser != nullptr)) return;
	PanelSwitcher->SetActiveWidget(Tab_ServerBrowser);
	
	if (!ensure(BtnServerBrowser != nullptr)) return;
	BtnServerBrowser->WidgetStyle.Normal.SetResourceObject(ActiveTabTexture);
	BtnServerBrowser->WidgetStyle.Normal.SetImageSize(FVector2D(ActiveTabTexture->MaxTextureSize));
}

void UMultiplayerPanel::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);
	this->RemoveFromViewport();
}

void UMultiplayerPanel::OnBtnServerBrowserClicked()
{
	if (!ensure(PanelSwitcher != nullptr)) return;
	if (!ensure(Tab_ServerBrowser != nullptr)) return;

	PanelSwitcher->SetActiveWidget(Tab_ServerBrowser);

	BtnServerBrowser->WidgetStyle.Normal.SetResourceObject(ActiveTabTexture);
	BtnServerBrowser->WidgetStyle.Normal.SetImageSize(FVector2D(ActiveTabTexture->MaxTextureSize));
	BtnHostServer->WidgetStyle.Normal.SetResourceObject(NormalTabTexture);
	BtnHostServer->WidgetStyle.Normal.SetImageSize(FVector2D(NormalTabTexture->MaxTextureSize));
	BtnGameModes->WidgetStyle.Normal.SetResourceObject(NormalTabTexture);
	BtnGameModes->WidgetStyle.Normal.SetImageSize(FVector2D(NormalTabTexture->MaxTextureSize));
}

void UMultiplayerPanel::OnBtnHostServerClicked()
{
	if (!ensure(PanelSwitcher != nullptr)) return;
	if (!ensure(Tab_HostServer != nullptr)) return;

	PanelSwitcher->SetActiveWidget(Tab_HostServer);

	BtnHostServer->WidgetStyle.Normal.SetResourceObject(ActiveTabTexture);
	BtnHostServer->WidgetStyle.Normal.SetImageSize(FVector2D(ActiveTabTexture->MaxTextureSize));
	BtnServerBrowser->WidgetStyle.Normal.SetResourceObject(NormalTabTexture);
	BtnServerBrowser->WidgetStyle.Normal.SetImageSize(FVector2D(NormalTabTexture->MaxTextureSize));
	BtnGameModes->WidgetStyle.Normal.SetResourceObject(NormalTabTexture);
	BtnGameModes->WidgetStyle.Normal.SetImageSize(FVector2D(NormalTabTexture->MaxTextureSize));
}

void UMultiplayerPanel::OnBtnGameModesClicked()
{
	if (!ensure(PanelSwitcher != nullptr)) return;
	if (!ensure(Tab_GameModes != nullptr)) return;

	PanelSwitcher->SetActiveWidget(Tab_GameModes);

	BtnGameModes->WidgetStyle.Normal.SetResourceObject(ActiveTabTexture);
	BtnGameModes->WidgetStyle.Normal.SetImageSize(FVector2D(ActiveTabTexture->MaxTextureSize));
	BtnServerBrowser->WidgetStyle.Normal.SetResourceObject(NormalTabTexture);
	BtnServerBrowser->WidgetStyle.Normal.SetImageSize(FVector2D(NormalTabTexture->MaxTextureSize));
	BtnHostServer->WidgetStyle.Normal.SetResourceObject(NormalTabTexture);
	BtnHostServer->WidgetStyle.Normal.SetImageSize(FVector2D(NormalTabTexture->MaxTextureSize));
}
