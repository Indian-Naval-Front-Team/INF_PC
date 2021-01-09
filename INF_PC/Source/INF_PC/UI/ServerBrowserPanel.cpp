// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerBrowserPanel.h"
#include "UMG/Public/Components/Button.h"
#include "ServerRow.h"

UServerBrowserPanel::UServerBrowserPanel(const FObjectInitializer& ObjectInitializer)
{
	const ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBP(TEXT("/Game/__Blueprints/Widgets/Multiplayer/WBP_ServerRow"));
	if (!ensure(ServerRowBP.Class != nullptr)) return;

	ServerRowClass = ServerRowBP.Class;
}

bool UServerBrowserPanel::Initialize()
{
	const bool Success = Super::Initialize();

	if (!Success) return false;

	if (!ensure(BtnServerRowTest != nullptr)) return false;

	BtnServerRowTest->OnClicked.AddDynamic(this, &UServerBrowserPanel::OnBtnServerRowTestClicked);

	return true;
}

void UServerBrowserPanel::OnBtnServerRowTestClicked()
{
	ServerRowWidget = CreateWidget<UServerRow>(this, ServerRowClass);
	if (!ensure(ServerRowWidget != nullptr)) return;

	if (!ensure(ServerList != nullptr)) return;
	ServerList->AddChild(ServerRowWidget);
}
