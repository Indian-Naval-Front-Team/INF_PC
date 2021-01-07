// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HostServerPanel.generated.h"

/**
 * 
 */
UCLASS()
class INF_PC_API UHostServerPanel : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UHostServerPanel(const FObjectInitializer& ObjectInitializer);

private:
	class UTexture2D* ActiveTabTexture;
	class UTexture2D* NormalTabTexture;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* BtnSelectGameMode;

	UPROPERTY(meta = (BindWidget))
	class UButton* BtnSetGameRules;
};
