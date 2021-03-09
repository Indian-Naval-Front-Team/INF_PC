// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ObjectiveWidget.generated.h"

/**
 * 
 */
UCLASS()
class INF_PC_API UObjectiveWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	class UTextBlock* TxtCurrentObjective;

public:
	UFUNCTION(BlueprintCallable, meta=(AllowPrivateAccess="true"))
	void UpdateObjectiveText(FText ObjectiveText);
};


