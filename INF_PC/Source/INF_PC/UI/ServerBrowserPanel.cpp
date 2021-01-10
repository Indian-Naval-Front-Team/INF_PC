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

void UServerBrowserPanel::NativeConstruct()
{
	Super::NativeConstruct();
	UE_LOG(LogTemp, Warning, TEXT("ServerBrowserPanel constructed!!"));

	INFGameInstance = Cast<UINFGameInstance>(GetWorld()->GetGameInstance());
	if (!ensure(INFGameInstance != nullptr)) return;

	INFGameInstance->SetServerBrowserPanel(this);
}

bool UServerBrowserPanel::Initialize()
{
	const bool Success = Super::Initialize();

	if (!Success) return false;

	if (!ensure(TestButton != nullptr)) return false;

	TestButton->OnClicked.AddDynamic(this, &UServerBrowserPanel::OnBtnServerRowTestClicked);

	return true;
}

void UServerBrowserPanel::SetServerList(TArray<FServerData> ServerDatum)
{
	ServerList->ClearChildren();
	
	uint32 Index = 0;
	for (const FServerData& ServerData : ServerDatum)
	{
		ServerRowWidget = CreateWidget<UServerRow>(this, ServerRowClass);
		if (!ensure(ServerRowWidget != nullptr)) return;
		
		ServerRowWidget->SetMenuInterface(INFGameInstance);
		ServerRowWidget->SetSessionIndex(Index++);
		ServerRowWidget->SetServerName(FText::FromString(ServerData.ServerName));
		FString PlayerCountText = FString::Printf(TEXT("%d / %d"), ServerData.CurrentPlayers, ServerData.ServerSize);
		ServerRowWidget->SetPlayerCountText(FText::FromString(PlayerCountText));
		
		if (!ensure(ServerList != nullptr)) return;
		ServerList->AddChild(ServerRowWidget);
	}

	Index = 0;
}

void UServerBrowserPanel::OnBtnServerRowTestClicked()
{
	//SetServerList({ "Test Server 1", "Test Server 2" });
}
