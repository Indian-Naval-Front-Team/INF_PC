// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <INF_PC/UI/MenuWidget.h>
#include "GameModesWidget.generated.h"

/**
 * 
 */
UCLASS()
class INF_PC_API UGameModesWidget : public UMenuWidget
{
	GENERATED_BODY()

public:
	UGameModesWidget(const FObjectInitializer& ObjectInitializer);

	void Setup();
	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;

private:
	class UTexture* ActiveTabTexture;
	class UTexture* NormalTabTexture;

	UPROPERTY(meta = (BindWidget))
		class UButton* BtnSortie;
	UPROPERTY(meta = (BindWidget))
		class UButton* BtnAmbush;
	UPROPERTY(meta = (BindWidget))
		class UButton* BtnPortCapture;
	UPROPERTY(meta = (BindWidget))
		class UButton* BtnTeamDeathmatch;
	UPROPERTY(meta = (BindWidget))
		class UWidgetSwitcher* PanelSwitcher;
	UPROPERTY(meta = (BindWidget))
		class UWidget* Tab_Sortie;
	UPROPERTY(meta = (BindWidget))
		class UWidget* Tab_Ambush;
	UPROPERTY(meta = (BindWidget))
		class UWidget* Tab_PortCapture;
	UPROPERTY(meta = (BindWidget))
		class UWidget* Tab_TeamDeathmatch;

	UFUNCTION()
		void OnBtnSortieClicked();
	UFUNCTION()
		void OnBtnAmbushClicked();
	UFUNCTION()
		void OnBtnPortCaptureClicked();
	UFUNCTION()
		void OnBtnTeamDeathmatchClicked();

protected:
	bool Initialize() override;
};
