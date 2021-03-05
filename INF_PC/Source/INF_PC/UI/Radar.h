// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Radar.generated.h"

enum class EEntityType : uint8;
/**
 * 
 */
UCLASS()
class INF_PC_API URadar : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
private:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UUserWidget* RadarWidget;
	
	UPROPERTY(BlueprintReadWrite, Category = "Radar", meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> AllRadarActors;
	UPROPERTY(BlueprintReadWrite, Category = "Radar", meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> AllFriendActors;
	UPROPERTY(BlueprintReadWrite, Category = "Radar", meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> AllFoeActors;
	UPROPERTY(BlueprintReadWrite, Category = "Radar", meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> AllNeutralActors;
	
	UPROPERTY(BlueprintReadWrite, Category = "Radar", meta = (AllowPrivateAccess = "true"))
	class AVehicleMaster* SelectedVehicleRef;

	UFUNCTION(BlueprintPure, meta = (AllowPrivateAccess = "true"))
	bool GetAllActorsForRadar();
	
	void ClearAllTables();
};
