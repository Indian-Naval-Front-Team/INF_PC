// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "INF_PC/Components/HealthComponent.h"

#include "EntityCardWidget.generated.h"

/**
 * 
 */
UCLASS()
class INF_PC_API UEntityCardWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UEntityCardWidget(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;
	void SetupEntityCard(EEntityType EntityType, FText EntityName, FText InitialDistance, class UEntityMarkerComponent* EntityMarkerCompRef);
	void UpdateEntityCardHealth(float DamagePercentage);
	void UpdateEntityCardDistance(const FText Distance);
	
private:
	UPROPERTY(meta = (BindWidget))
	class UImage* EntityMarker;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TxtEntityName;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TxtDistance;
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;
	
	class UTexture* EntityMarker_Enemy;
	class UTexture* EntityMarker_Friend;
	class UTexture* EntityMarker_Neutral;
};
