// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <INF_PC/UI/MenuWidget.h>
#include "ServerBrowserPanel.generated.h"

/**
 * 
 */
UCLASS()
class INF_PC_API UServerBrowserPanel : public UMenuWidget
{
	GENERATED_BODY()
	
public:
	UServerBrowserPanel(const FObjectInitializer& ObjectInitializer);
	void SetServerList(TArray<FString> ServerNames);

private:
	UPROPERTY(meta = (BindWidget))
	class UPanelWidget* ServerList;
	UPROPERTY(meta = (BindWidget))
	class UButton* TestButton;

	TSubclassOf<UUserWidget> ServerRowClass;
	class UServerRow* ServerRowWidget;
	class UINFGameInstance* INFGameInstance;

	UFUNCTION()
	void OnBtnServerRowTestClicked();

protected:
	bool Initialize();
};
