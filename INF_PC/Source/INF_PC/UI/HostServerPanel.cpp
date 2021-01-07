// Fill out your copyright notice in the Description page of Project Settings.


#include "HostServerPanel.h"
#include "UMG/Public/Components/Button.h"
#include "UMG/Public/Components/WidgetSwitcher.h"
#include "Runtime/Engine/Classes/Engine/Texture2D.h"

UHostServerPanel::UHostServerPanel(const FObjectInitializer& ObjectInitializer)
{
	const ConstructorHelpers::FObjectFinder<UTexture2D>ActiveTabTextureTemp(TEXT("/Game/_Assets/UI/GenericButton_Hovered"));
	const ConstructorHelpers::FObjectFinder<UTexture2D>NormalTabTextureTemp(TEXT("/Game/_Assets/UI/GenericButton_Normal"));

	if (!ensure(ActiveTabTextureTemp.Object != nullptr)) return;
	if (!ensure(NormalTabTextureTemp.Object != nullptr)) return;

	ActiveTabTexture = ActiveTabTextureTemp.Object;
	NormalTabTexture = NormalTabTextureTemp.Object;
}
