// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TsunaPlayerController.generated.h"

/**
 * 
 */
class UUserWidget;
class UPauseMenu;


//delefates for sharing data to pawn
DECLARE_DELEGATE_OneParam(ToSendNewName, const FText&);

DECLARE_DELEGATE_OneParam(ToSend3DwidgetVis, bool);


UCLASS()
class SH_API ATsunaPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, Category = "Widget Pause")
		UUserWidget* CurrentWidget;

	UPROPERTY(EditAnywhere, Category = "Pause Widget")
		TSubclassOf<UPauseMenu> PauseMenuWidget;

	//vars from widget:
	UPROPERTY()
		FText CharacterName = FText::FromString(TEXT("Player"));

	bool bIsVisible{ false };

protected:
	void OpenPauseMenu();
	bool IsPaused = false;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* UIOpen;
	virtual void SetupInputComponent() override;

	//Function is called via delegate
	UFUNCTION()
		void OnPauseCall(bool isPaused);

	//Get response from PlayButtdon
	void Unpause();

public:
	ToSendNewName ToPawnTextSender;

	ToSend3DwidgetVis On3DWidgetVisibleUpdate;
};
