// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MenuInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMenuInterface : public UInterface
{
	GENERATED_BODY()
};

UENUM()
enum EMatchDuration
{
	FIFTEENMINUTES	  UMETA(DisplayName = "15 Minutes"),
	TWENTYMINUTES	  UMETA(DisplayName = "20 Minutes"),
	THIRTYMINUTES	  UMETA(DisplayName = "30 Minutes"),
};

UENUM()
enum ETicketsPerTeam
{
	THOUSANDTICKETS					  UMETA(DisplayName = "1000 Tickets"),
	FIFTEENHUNDREDTICKETS	          UMETA(DisplayName = "1500 Tickets"),
	TWOTHOUSANDTICKETS		          UMETA(DisplayName = "2000 Tickets"),
	TWOTHOUSANDFIVEHUNDREDTICKETS	  UMETA(DisplayName = "2500 Tickets"),
	THREETHOUSANDTICKETS			  UMETA(DisplayName = "3000 Tickets"),
	THREETHOUSANDFIVEHUNDREDTICKETS	  UMETA(DisplayName = "3500 Tickets"),
	FOURTHOUSANDTICKETS	              UMETA(DisplayName = "4000 Tickets"),
};

UENUM()
enum EServerSize
{
	TWOPLAYERS		  UMETA(DisplayName = "2 Players"),
	FOURPLAYERS		  UMETA(DisplayName = "4 Players"),
	SIXPLAYERS		  UMETA(DisplayName = "6 Players"),
	EIGHTPLAYERS	  UMETA(DisplayName = "8 Players"),
	TENPLAYERS		  UMETA(DisplayName = "10 Players"),
	TWELVEPLAYERS	  UMETA(DisplayName = "12 Players"),
	FOURTEENPLAYERS	  UMETA(DisplayName = "14 Players"),
	SIXTEENPLAYERS	  UMETA(DisplayName = "16 Players"),
	EIGHTEENPLAYERS	  UMETA(DisplayName = "18 Players"),
	TWENTYPLAYERS	  UMETA(DisplayName = "20 Players"),
};

UENUM()
enum EPlayersRequiredToStart
{
	TWOPLAYERSREQUIRED		  UMETA(DisplayName = "2 Players"),
	FOURPLAYERSREQUIRED		  UMETA(DisplayName = "4 Players"),
	SIXPLAYERSREQUIRED		  UMETA(DisplayName = "6 Players"),
	EIGHTPLAYERSREQUIRED	  UMETA(DisplayName = "8 Players"),
};

UENUM()
enum EServerRestartRules
{
	NEVERSHUTDOWN	  UMETA(DisplayName = "Never Shut Down"),
	ONEMATCH		  UMETA(DisplayName = "Shut down after 1 match"),
	THREEMATCH		  UMETA(DisplayName = "Shut down after 3 matches"),
	FIVEMATCH		  UMETA(DisplayName = "Shut down after 5 matches"),
};

USTRUCT()
struct FServerData
{
	GENERATED_BODY()

	FString ServerName;
	FString Password;
	float MapDuration;
	uint32 TicketsPerTeam;
	uint16 ServerSize;
	uint16 PlayersRequiredToStart;
	EServerRestartRules ServerRestartRule;
	bool IsLan;

	uint16 CurrentPlayers;
};

/**
 * 
 */
class INF_PC_API IMenuInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void Host(FServerData ServerData) = 0;
	virtual void Join(class UServerRow* ServerRow) = 0;
	virtual void Find() = 0;
	virtual void RefreshServerList() = 0;
};
