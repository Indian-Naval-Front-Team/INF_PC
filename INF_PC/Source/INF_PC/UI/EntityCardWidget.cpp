// Fill out your copyright notice in the Description page of Project Settings.


#include "EntityCardWidget.h"


#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "INF_PC/Components/EntityMarkerComponent.h"

UEntityCardWidget::UEntityCardWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	const ConstructorHelpers::FObjectFinder<UTexture> EntityMarker_Enemy_Temp(TEXT("/Game/_Assets/UI/EnemyMarker"));
	const ConstructorHelpers::FObjectFinder<UTexture> EntityMarker_Friend_Temp(TEXT("/Game/_Assets/UI/FriendlyMarker"));
	const ConstructorHelpers::FObjectFinder<UTexture> EntityMarker_Neutral_Temp(TEXT("/Game/_Assets/UI/NeutralMarker"));
	
	if (!ensure(EntityMarker_Enemy_Temp.Object != nullptr)) return;
	if (!ensure(EntityMarker_Friend_Temp.Object != nullptr)) return;
	if (!ensure(EntityMarker_Neutral_Temp.Object != nullptr)) return;
	
	EntityMarker_Enemy = EntityMarker_Enemy_Temp.Object;
	EntityMarker_Friend = EntityMarker_Friend_Temp.Object;
	EntityMarker_Neutral = EntityMarker_Neutral_Temp.Object;
}

void UEntityCardWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UEntityCardWidget::SetupEntityCard(const EEntityType EntityType, const FText EntityName, const FText InitialDistance, class UEntityMarkerComponent* EntityMarkerCompRef)
{
	TxtEntityName->SetText(EntityName);
	TxtDistance->SetText(InitialDistance);
	
	switch (EntityType)
	{
		case EEntityType::W_Friend:
			EntityMarker->SetBrushResourceObject(EntityMarker_Friend);
			break;
		case EEntityType::W_Foe:
			EntityMarker->SetBrushResourceObject(EntityMarker_Enemy);
			break;
		case EEntityType::W_Neutral:
			EntityMarker->SetBrushResourceObject(EntityMarker_Neutral);
			break;
		default: ;
	}

	EntityMarkerCompRef->SetComponentTickEnabled(true);
}

void UEntityCardWidget::UpdateEntityCardHealth(const float DamagePercentage)
{
	HealthBar->SetPercent(1.0f - DamagePercentage);
}

void UEntityCardWidget::UpdateEntityCardDistance(const FText Distance)
{
	TxtDistance->SetText(Distance);
}
