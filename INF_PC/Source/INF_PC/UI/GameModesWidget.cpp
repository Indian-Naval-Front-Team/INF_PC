// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModesWidget.h"
#include "UMG/Public/Components/Button.h"
#include "UMG/Public/Components/WidgetSwitcher.h"
#include "Runtime/Engine/Classes/Engine/Texture2D.h"

UGameModesWidget::UGameModesWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	const ConstructorHelpers::FObjectFinder<UTexture>ActiveTabTextureTemp(TEXT("/Game/_Assets/UI/GenericButton_Hovered"));
	const ConstructorHelpers::FObjectFinder<UTexture>NormalTabTextureTemp(TEXT("/Game/_Assets/UI/GenericButton_Normal"));

	if (!ensure(ActiveTabTextureTemp.Object != nullptr)) return;
	if (!ensure(NormalTabTextureTemp.Object != nullptr)) return;

	ActiveTabTexture = ActiveTabTextureTemp.Object;
	NormalTabTexture = NormalTabTextureTemp.Object;
}

bool UGameModesWidget::Initialize()
{
	const bool Success = Super::Initialize();

	if (!Success) return false;

	if (!ensure(BtnSortie != nullptr)) return false;
	if (!ensure(BtnAmbush != nullptr)) return false;
	if (!ensure(BtnPortCapture != nullptr)) return false;
	if (!ensure(BtnTeamDeathmatch != nullptr)) return false;

	BtnSortie->OnClicked.AddDynamic(this, &UGameModesWidget::OnBtnSortieClicked);
	BtnAmbush->OnClicked.AddDynamic(this, &UGameModesWidget::OnBtnAmbushClicked);
	BtnPortCapture->OnClicked.AddDynamic(this, &UGameModesWidget::OnBtnPortCaptureClicked);
	BtnTeamDeathmatch->OnClicked.AddDynamic(this, &UGameModesWidget::OnBtnTeamDeathmatchClicked);

	return true;
}

void UGameModesWidget::Setup()
{

}

void UGameModesWidget::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);
	this->RemoveFromViewport();
}

void UGameModesWidget::OnBtnSortieClicked()
{
	if (!ensure(PanelSwitcher != nullptr)) return;
	if (!ensure(Tab_Sortie != nullptr)) return;

	PanelSwitcher->SetActiveWidget(Tab_Sortie);

	BtnSortie->WidgetStyle.Normal.SetResourceObject(ActiveTabTexture);
	BtnSortie->WidgetStyle.Normal.SetImageSize(FVector2D(ActiveTabTexture->MaxTextureSize));
	BtnAmbush->WidgetStyle.Normal.SetResourceObject(NormalTabTexture);
	BtnAmbush->WidgetStyle.Normal.SetImageSize(FVector2D(NormalTabTexture->MaxTextureSize));
	BtnPortCapture->WidgetStyle.Normal.SetResourceObject(NormalTabTexture);
	BtnPortCapture->WidgetStyle.Normal.SetImageSize(FVector2D(NormalTabTexture->MaxTextureSize));
	BtnTeamDeathmatch->WidgetStyle.Normal.SetResourceObject(NormalTabTexture);
	BtnTeamDeathmatch->WidgetStyle.Normal.SetImageSize(FVector2D(NormalTabTexture->MaxTextureSize));
}

void UGameModesWidget::OnBtnAmbushClicked()
{
	if (!ensure(PanelSwitcher != nullptr)) return;
	if (!ensure(Tab_Ambush != nullptr)) return;

	PanelSwitcher->SetActiveWidget(Tab_Ambush);

	BtnAmbush->WidgetStyle.Normal.SetResourceObject(ActiveTabTexture);
	BtnAmbush->WidgetStyle.Normal.SetImageSize(FVector2D(ActiveTabTexture->MaxTextureSize));
	BtnSortie->WidgetStyle.Normal.SetResourceObject(NormalTabTexture);
	BtnSortie->WidgetStyle.Normal.SetImageSize(FVector2D(NormalTabTexture->MaxTextureSize));
	BtnPortCapture->WidgetStyle.Normal.SetResourceObject(NormalTabTexture);
	BtnPortCapture->WidgetStyle.Normal.SetImageSize(FVector2D(NormalTabTexture->MaxTextureSize));
	BtnTeamDeathmatch->WidgetStyle.Normal.SetResourceObject(NormalTabTexture);
	BtnTeamDeathmatch->WidgetStyle.Normal.SetImageSize(FVector2D(NormalTabTexture->MaxTextureSize));
}

void UGameModesWidget::OnBtnPortCaptureClicked()
{
	if (!ensure(PanelSwitcher != nullptr)) return;
	if (!ensure(Tab_PortCapture != nullptr)) return;

	PanelSwitcher->SetActiveWidget(Tab_PortCapture);

	BtnPortCapture->WidgetStyle.Normal.SetResourceObject(ActiveTabTexture);
	BtnPortCapture->WidgetStyle.Normal.SetImageSize(FVector2D(ActiveTabTexture->MaxTextureSize));
	BtnSortie->WidgetStyle.Normal.SetResourceObject(NormalTabTexture);
	BtnSortie->WidgetStyle.Normal.SetImageSize(FVector2D(NormalTabTexture->MaxTextureSize));
	BtnAmbush->WidgetStyle.Normal.SetResourceObject(NormalTabTexture);
	BtnAmbush->WidgetStyle.Normal.SetImageSize(FVector2D(NormalTabTexture->MaxTextureSize));
	BtnTeamDeathmatch->WidgetStyle.Normal.SetResourceObject(NormalTabTexture);
	BtnTeamDeathmatch->WidgetStyle.Normal.SetImageSize(FVector2D(NormalTabTexture->MaxTextureSize));
}

void UGameModesWidget::OnBtnTeamDeathmatchClicked()
{
	if (!ensure(PanelSwitcher != nullptr)) return;
	if (!ensure(Tab_TeamDeathmatch != nullptr)) return;

	PanelSwitcher->SetActiveWidget(Tab_TeamDeathmatch);

	BtnTeamDeathmatch->WidgetStyle.Normal.SetResourceObject(ActiveTabTexture);
	BtnTeamDeathmatch->WidgetStyle.Normal.SetImageSize(FVector2D(ActiveTabTexture->MaxTextureSize));
	BtnSortie->WidgetStyle.Normal.SetResourceObject(NormalTabTexture);
	BtnSortie->WidgetStyle.Normal.SetImageSize(FVector2D(NormalTabTexture->MaxTextureSize));
	BtnAmbush->WidgetStyle.Normal.SetResourceObject(NormalTabTexture);
	BtnAmbush->WidgetStyle.Normal.SetImageSize(FVector2D(NormalTabTexture->MaxTextureSize));
	BtnPortCapture->WidgetStyle.Normal.SetResourceObject(NormalTabTexture);
	BtnPortCapture->WidgetStyle.Normal.SetImageSize(FVector2D(NormalTabTexture->MaxTextureSize));
}