// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode_DefendCarrier.h"
#include <INF_PC/TP_Flying/TP_FlyingPawn.h>

AGameMode_DefendCarrier::AGameMode_DefendCarrier() : AGameModeParent()
{
	DefaultPawnClass = ATP_FlyingPawn::StaticClass();
}
