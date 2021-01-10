// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"
#include "MenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class INF_PC_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void NativeConstruct() override;

	void Setup();
	IMenuInterface* GetMenuInterface() { return MenuInterfaceRef; };

	void SetMenuInterface(IMenuInterface* MenuInterface);

	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;
protected:
	IMenuInterface* MenuInterfaceRef;

};
