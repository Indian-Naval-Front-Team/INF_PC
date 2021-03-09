// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectiveWidget.h"

#include "Components/TextBlock.h"

void UObjectiveWidget::UpdateObjectiveText(FText ObjectiveText)
{
	TxtCurrentObjective->SetText(ObjectiveText);
}
