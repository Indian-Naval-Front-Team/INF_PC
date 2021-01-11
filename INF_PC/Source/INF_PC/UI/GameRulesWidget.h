// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "INF_PC/UI/MenuInterface.h"
#include <INF_PC/UI/MenuWidget.h>
#include <INF_PC/UI/MainMenu.h>
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Components/EditableTextBox.h"
#include "UMG/Public/Components/ComboBoxString.h"
#include "UMG/Public/Components/CheckBox.h"
#include "GameRulesWidget.generated.h"

/**
 * 
 */
UCLASS()
class INF_PC_API UGameRulesWidget : public UMenuWidget
{
	GENERATED_BODY()

public:
	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TxtSelectedGameMode;
	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* Input_ServerName;
	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* Input_Password;
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* Input_MapDuration;
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* Input_TicketsPerTeam;
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* Input_ServerSize;
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* Input_PlayersRequiredToStart;
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* Input_MatchFormat;
	UPROPERTY(meta = (BindWidget))
	UCheckBox* CBox_LAN;
	UPROPERTY(meta = (BindWidget))
	UButton* BtnListenServer;
	UPROPERTY(meta = (BindWidget))
	UButton* BtnDedicatedServer;
	
	UFUNCTION()
	void OnBtnListenServerClicked();
	UFUNCTION()
	void OnBtnDedicatedServerClicked();

	FServerData ServerData;

protected:
	virtual bool Initialize();
};
