// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "INF_PC/UI/MenuInterface.h"
#include "INFGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class INF_PC_API UINFGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()
	
public:
	UINFGameInstance(const FObjectInitializer& ObjectInitializer);

	virtual void Initialize();

	UFUNCTION(BlueprintCallable)
	void LoadMainMenu();

	UFUNCTION(BlueprintCallable)
	void Host();

	UFUNCTION(BlueprintCallable)
	void Join();

private:
	TSubclassOf<class UUserWidget> MainMenuClass;
};
