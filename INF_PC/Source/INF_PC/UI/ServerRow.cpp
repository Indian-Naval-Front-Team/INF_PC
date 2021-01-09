// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerRow.h"
#include "../Framework/INFGameInstance.h"


bool UServerRow::Initialize()
{
	const bool Success = Super::Initialize();

	if (!Success) return false;

	if (!ensure(BtnJoin != nullptr)) return false;

	BtnJoin->OnClicked.AddDynamic(this, &UServerRow::OnBtnJoinClicked);

	UINFGameInstance* INFGameInstance = Cast<UINFGameInstance>(GetWorld()->GetGameInstance());
	if (!ensure(INFGameInstance != nullptr)) return false;

	this->SetMenuInterface(INFGameInstance->GetMainMenu()->GetMenuInterface());

	return true;
}

void UServerRow::SetServerName(FText Val)
{
	if (!Val.IsEmpty())
	{
		TxtServerName->SetText(Val);
	}
}

void UServerRow::SetGameModeText(FText Val)
{
	if (!Val.IsEmpty())
	{
		TxtGameMode->SetText(Val);
	}
}

void UServerRow::SetPlayerCountText(FText Val)
{
	if (!Val.IsEmpty())
	{
		TxtPlayerCount->SetText(Val);
	}
}

void UServerRow::SetPingText(FText Val)
{
	if (!Val.IsEmpty())
	{
		TxtPing->SetText(Val);
	}
}

void UServerRow::SetLocked(bool IsLocked)
{

}

void UServerRow::SetSessionIndex(uint32 Val)
{
	if (Val < 0)
		Val = 0;

	SessionIndex = Val;
}

void UServerRow::OnBtnJoinClicked()
{
	if (MenuInterfaceRef != nullptr)
	{
		UINFGameInstance* INFGameInstance = Cast<UINFGameInstance>(GetWorld()->GetGameInstance());
		if (!ensure(INFGameInstance != nullptr)) return;

		MenuInterfaceRef->Join(this);
	}
}
