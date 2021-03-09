// Fill out your copyright notice in the Description page of Project Settings.


#include "SphereLocationTrigger.h"

#include "Components/SphereComponent.h"

ASphereLocationTrigger::ASphereLocationTrigger()
{
	SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Box Collider"));
	SetRootComponent(SphereCollider);
}
