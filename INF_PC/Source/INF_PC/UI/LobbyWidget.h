// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget.generated.h"

/**
 * 
 */
UCLASS()
class INF_PC_API ULobbyWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Setup();
	
};
