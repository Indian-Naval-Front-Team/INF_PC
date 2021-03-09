// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxLocationTrigger.h"

#include "Components/BoxComponent.h"

ABoxLocationTrigger::ABoxLocationTrigger()
{
	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	SetRootComponent(BoxCollider);
}
