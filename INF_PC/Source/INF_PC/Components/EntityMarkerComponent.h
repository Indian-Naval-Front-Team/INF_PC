// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "HealthComponent.h"
#include "Components/WidgetComponent.h"
#include "EntityMarkerComponent.generated.h"

static FORCEINLINE FString GetFloatAsStringWithPrecision(float TheFloat, int32 Precision, bool IncludeLeadingZero=true)
{
	//Round to integral if have something like 1.9999 within precision
	float Rounded = roundf(TheFloat);
	if(FMath::Abs(TheFloat - Rounded) < FMath::Pow(10,-1 * Precision))
	{       
		TheFloat = Rounded;
	}
	FNumberFormattingOptions NumberFormat;					//Text.h
	NumberFormat.MinimumIntegralDigits = (IncludeLeadingZero) ? 1 : 0;
	NumberFormat.MaximumIntegralDigits = 10000;
	NumberFormat.MinimumFractionalDigits = Precision;
	NumberFormat.MaximumFractionalDigits = Precision;
	return FText::AsNumber(TheFloat, &NumberFormat).ToString(); 
}

//Float as FText With Precision!
static FORCEINLINE FText GetFloatAsTextWithPrecision(float TheFloat, int32 Precision, bool IncludeLeadingZero=true)
{
	//Round to integral if have something like 1.9999 within precision
	float Rounded = roundf(TheFloat);
	if(FMath::Abs(TheFloat - Rounded) < FMath::Pow(10,-1 * Precision))
	{       
		TheFloat = Rounded;
	}
	FNumberFormattingOptions NumberFormat;					//Text.h
	NumberFormat.MinimumIntegralDigits = (IncludeLeadingZero) ? 1 : 0;
	NumberFormat.MaximumIntegralDigits = 10000;
	NumberFormat.MinimumFractionalDigits = Precision;
	NumberFormat.MaximumFractionalDigits = Precision;
	return FText::AsNumber(TheFloat, &NumberFormat); 
}

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class INF_PC_API UEntityMarkerComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UEntityMarkerComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	class UEntityCardWidget* GetEntityCardWidget() const { return EntityCardWidget; }

protected:
	UFUNCTION(BlueprintCallable, Category = "Entity Marker Functions")
	void ShowEntityCard();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Marker Setup", meta = (AllowPrivateAccess="true"))
	float MinDistanceForRenderInKms;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Marker Setup", meta = (AllowPrivateAccess="true"))
	float TickFrequency;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Marker Setup", meta = (AllowPrivateAccess="true"))
	FText EntityName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Marker Setup", meta = (AllowPrivateAccess="true"))
	EEntityType EntityType;

	UPROPERTY(BlueprintReadOnly, Category = "Entity Marker", meta = (AllowPrivateAccess="true"))
	class AVehicleMaster* PlayerPawn;
	UPROPERTY(BlueprintReadOnly, Category = "Entity Marker", meta = (AllowPrivateAccess="true"))
	class UEntityCardWidget* EntityCardWidget;

	float LastTickTime;
	const TCHAR* LLOCTEXT_NAMESPACE;
};
