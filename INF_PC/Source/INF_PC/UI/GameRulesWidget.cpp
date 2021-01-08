// Fill out your copyright notice in the Description page of Project Settings.


#include "GameRulesWidget.h"
#include "UMG/Public/Components/Button.h"


bool UGameRulesWidget::Initialize()
{
	const bool Success = Super::Initialize();

	if (!Success) return false;

	if (!ensure(BtnListenServer != nullptr)) return false;
	if (!ensure(BtnDedicatedServer != nullptr)) return false;

	BtnListenServer->OnClicked.AddDynamic(this, &UGameRulesWidget::OnBtnListenServerClicked);
	BtnDedicatedServer->OnClicked.AddDynamic(this, &UGameRulesWidget::OnBtnDedicatedServerClicked);

	// TODO : Change this first when Dedicated Server code will be written.
	BtnDedicatedServer->SetIsEnabled(false);

	return true;
}

void UGameRulesWidget::OnBtnListenServerClicked()
{
	if (MenuInterfaceRef == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Menu Interface Reference is NULL AGAIN!!!"));
	}
	MenuInterfaceRef->Host();
}

void UGameRulesWidget::OnBtnDedicatedServerClicked()
{

}

void UGameRulesWidget::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);
	this->RemoveFromViewport();
}
