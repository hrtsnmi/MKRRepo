// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "TsunaController.generated.h"

/**
 * 
 */
class UUserWidget;
class UPPSwitcherWidget;


//delefates for sharing data to pawn
DECLARE_DELEGATE_OneParam(ToSendNewName, const FText&);

DECLARE_DELEGATE_OneParam(ToSend3DwidgetVis, bool);


UCLASS()
class SH_API ATsunaController : public APlayerController
{
	GENERATED_BODY()
	
public:
	//ATsunaController();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* UIOpen;

	UPROPERTY(VisibleAnywhere, Category = "Widget Pause")
		UUserWidget* CurrentWidget;

	UPROPERTY(EditAnywhere, Category = "Pause Widget")
		TSubclassOf<UPPSwitcherWidget> PPSwitcherWidget;

	//vars from widget:
	UPROPERTY()
		FText CharacterName = FText::FromString(TEXT("Player"));

	bool bIsVisible{ false };
	virtual void SetupInputComponent();

	virtual void BeginPlay() override;

	//Function is called via delegate
	UFUNCTION()
		void OnPauseCall(bool isPaused);

	//Get response from PlayButtdon
	void Unpause();

	void UICall();
	bool bIsUIOpen{ false };

public:
	ToSendNewName ToPawnTextSender;

	ToSend3DwidgetVis On3DWidgetVisibleUpdate;

};
