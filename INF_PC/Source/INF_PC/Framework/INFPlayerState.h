// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "INFPlayerState.generated.h"

UENUM(BlueprintType)
enum class EPlayerStatus : uint8
{
	W_InsideMenu				UMETA(DisplayName = "Inside some Menu"),
	W_InsideGame				UMETA(DisplayName = "Inside the Game"),
	W_PausedGame				UMETA(DisplayName = "Inside Pause Menu"),
	W_MainVehicleView			UMETA(DisplayName = "Main Vehicle View"),
    W_ZoomedInWeapon			UMETA(DisplayName = "Zoomed in to Weapon"),
    W_ZoomedInBinocular			UMETA(DisplayName = "Zoomed in to Binocular"),
	W_FreeLookOn				UMETA(DisplayName = "Free Look On"),
    W_AAGun						UMETA(DisplayName = "Manning an AA Gun"),
    W_Cannon					UMETA(DisplayName = "Manning a Cannon"),
    W_Missile					UMETA(DisplayName = "Manning Missile Pods"),
    W_ShipTorpedo				UMETA(DisplayName = "Manning Ship Torpedos"),
    W_Jet						UMETA(DisplayName = "Flying a Jet"),
    W_Chopper					UMETA(DisplayName = "Flying a Chopper")
};

UCLASS()
class INF_PC_API AINFPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AINFPlayerState();
	
	void SetCurrentPlayerStatus(EPlayerStatus PlayerStatus) { CurrentPlayerStatus = PlayerStatus; }
	EPlayerStatus GetCurrentPlayerStatus() const { return CurrentPlayerStatus; }
	
private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	EPlayerStatus CurrentPlayerStatus;
};
