// Fill out your copyright notice in the Description page of Project Settings.


#include "HostServerPanel.h"
#include "UMG/Public/Components/Button.h"
#include "UMG/Public/Components/WidgetSwitcher.h"
#include "Runtime/Engine/Classes/Engine/Texture2D.h"

UHostServerPanel::UHostServerPanel(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	const ConstructorHelpers::FObjectFinder<UTexture2D>ActiveTabTextureTemp(TEXT("/Game/_Assets/UI/GenericButton_Hovered"));
	const ConstructorHelpers::FObjectFinder<UTexture2D>NormalTabTextureTemp(TEXT("/Game/_Assets/UI/GenericButton_Normal"));

	if (!ensure(ActiveTabTextureTemp.Object != nullptr)) return;
	if (!ensure(NormalTabTextureTemp.Object != nullptr)) return;

	ActiveTabTexture = ActiveTabTextureTemp.Object;
	NormalTabTexture = NormalTabTextureTemp.Object;
}

bool UHostServerPanel::Initialize()
{
	const bool Success = Super::Initialize();

	if (!Success) return false;

	if (!ensure(BtnSelectGameMode != nullptr)) return false;
	if (!ensure(BtnSetGameRules != nullptr)) return false;

	BtnSelectGameMode->OnClicked.AddDynamic(this, &UHostServerPanel::OnBtnSelectGameModeClicked);
	BtnSetGameRules->OnClicked.AddDynamic(this, &UHostServerPanel::OnBtnSetGameRulesClicked);

	return true;
}

void UHostServerPanel::Setup()
{
	if (!ensure(PanelSwitcher != nullptr)) return;
	if (!ensure(Tab_SelectGameMode != nullptr)) return;
	PanelSwitcher->SetActiveWidget(Tab_SelectGameMode);

	if (!ensure(BtnSelectGameMode != nullptr)) return;
	BtnSelectGameMode->WidgetStyle.Normal.SetResourceObject(ActiveTabTexture);
	BtnSelectGameMode->WidgetStyle.Normal.SetImageSize(FVector2D(ActiveTabTexture->MaxTextureSize));
}

void UHostServerPanel::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);
	this->RemoveFromViewport();
}

void UHostServerPanel::OnBtnSelectGameModeClicked()
{
	if (!ensure(PanelSwitcher != nullptr)) return;
	if (!ensure(Tab_SelectGameMode != nullptr)) return;

	PanelSwitcher->SetActiveWidget(Tab_SelectGameMode);

	BtnSelectGameMode->WidgetStyle.Normal.SetResourceObject(ActiveTabTexture);
	BtnSelectGameMode->WidgetStyle.Normal.SetImageSize(FVector2D(ActiveTabTexture->MaxTextureSize));
	BtnSetGameRules->WidgetStyle.Normal.SetResourceObject(NormalTabTexture);
	BtnSetGameRules->WidgetStyle.Normal.SetImageSize(FVector2D(NormalTabTexture->MaxTextureSize));
}

void UHostServerPanel::OnBtnSetGameRulesClicked()
{
	if (!ensure(PanelSwitcher != nullptr)) return;
	if (!ensure(Tab_SetGameRules != nullptr)) return;

	PanelSwitcher->SetActiveWidget(Tab_SetGameRules);

	BtnSetGameRules->WidgetStyle.Normal.SetResourceObject(ActiveTabTexture);
	BtnSetGameRules->WidgetStyle.Normal.SetImageSize(FVector2D(ActiveTabTexture->MaxTextureSize));
	BtnSelectGameMode->WidgetStyle.Normal.SetResourceObject(NormalTabTexture);
	BtnSelectGameMode->WidgetStyle.Normal.SetImageSize(FVector2D(NormalTabTexture->MaxTextureSize));
}
