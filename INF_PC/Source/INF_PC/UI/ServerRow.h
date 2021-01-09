// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <INF_PC/UI/MenuWidget.h>
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Components/SizeBox.h"
#include "UMG/Public/Components/Button.h"
#include "ServerRow.generated.h"

/**
 * 
 */
UCLASS()
class INF_PC_API UServerRow : public UMenuWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void SetServerName(FText Val);
	UFUNCTION()
	void SetGameModeText(FText Val);
	UFUNCTION()
	void SetPlayerCountText(FText Val);
	UFUNCTION()
	void SetPingText(FText Val);
	UFUNCTION()
	void SetLocked(bool IsLocked);

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TxtServerName;
	UPROPERTY(meta = (BindWidget))
	USizeBox* Locked;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TxtGameMode;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TxtPlayerCount;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TxtPing;
	UPROPERTY(meta = (BindWidget))
	UButton* BtnJoin;

	UFUNCTION()
	void OnBtnJoinClicked();

protected:
	bool Initialize();
};
