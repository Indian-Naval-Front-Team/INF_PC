// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/Actor.h"
#include "CockpitMaster.generated.h"

UCLASS()
class INF_PC_API ACockpitMaster : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACockpitMaster();
	FVector GetCockpitCameraPos() const { return CockpitCamera->GetComponentLocation(); }
	FRotator GetCockpitCameraRot() const { return CockpitCamera->GetComponentRotation(); }
	class UCameraComponent* GetCockpitCamera() const;
	void ResetCockpitCamera() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	class UStaticMeshComponent* DashboardBase;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	class UStaticMeshComponent* Dashboard;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	class USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	class UCameraComponent* CockpitCamera;

private:
	FTransform OriginalCockpitCameraTransform;
};
