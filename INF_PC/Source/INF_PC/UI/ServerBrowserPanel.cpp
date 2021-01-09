// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerBrowserPanel.h"
#include "UMG/Public/Components/Button.h"
#include "ServerRow.h"
#include <INF_PC/Framework/INFGameInstance.h>

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

	if (!ensure(TestButton != nullptr)) return false;

	TestButton->OnClicked.AddDynamic(this, &UServerBrowserPanel::OnBtnServerRowTestClicked);

	UINFGameInstance* INFGameInstance = Cast<UINFGameInstance>(GetWorld()->GetGameInstance());
	if (!ensure(INFGameInstance != nullptr)) return false;

	INFGameInstance->SetServerBrowserPanel(this);

	return true;
}

void UServerBrowserPanel::SetServerList(TArray<FString> ServerNames)
{
	ServerList->ClearChildren();

	for (const FString& ServerName : ServerNames)
	{
		ServerRowWidget = CreateWidget<UServerRow>(this, ServerRowClass);
		if (!ensure(ServerRowWidget != nullptr)) return;

		ServerRowWidget->SetServerName(FText::FromString(ServerName));

		if (!ensure(ServerList != nullptr)) return;
		ServerList->AddChild(ServerRowWidget);
	}
}

void UServerBrowserPanel::OnBtnServerRowTestClicked()
{
	SetServerList({ "Test Server 1", "Test Server 2" });
}
