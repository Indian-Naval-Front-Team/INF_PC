// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include <INF_PC/Framework/GameModeParent.h>
#include "MenuInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMenuInterface : public UInterface
{
	GENERATED_BODY()
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
