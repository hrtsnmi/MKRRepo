// Fill out your copyright notice in the Description page of Project Settings.


#include "TsunaController.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/TsunaCharacter.h"// to get delegate
#include "PPSwitcherWidget.h"//GetWidget

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void ATsunaController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		//E
		EnhancedInputComponent->BindAction(UIOpen, ETriggerEvent::Started, this, &ATsunaController::UICall);
	}
}

void ATsunaController::BeginPlay()
{
	Super::BeginPlay();

	UICall();
}

void ATsunaController::OnPauseCall(bool isPaused)
{
	if (isPaused)
	{
		//SetUpInputMode for menu
		//but FInputModeUIOnly leads to ignore all inputs for keyboard
		//SetInputMode(FInputModeUIOnly());

		//avoid ignoring key input:
		SetInputMode(FInputModeGameAndUI());

		if (!CurrentWidget)
		{
			//Create Widget
			UPPSwitcherWidget* NewWidget = CreateWidget<UPPSwitcherWidget>(this, PPSwitcherWidget);

			if (NewWidget)
			{
				//bind update params
				NewWidget->NewCharacterName.BindLambda([this](const FText& Text)
					{
						//SetNew Name for Character:
						CharacterName = Text;

						// triger Pawn to get new Vars
						//ToPawnTextSender.ExecuteIfBound(CharacterName);
					});


				NewWidget->NameVisibility.BindLambda([this](const bool VisibilityFromCheckBox)
					{
						bIsVisible = VisibilityFromCheckBox;

						// triget Pawn to get new Vars
						//On3DWidgetVisibleUpdate.ExecuteIfBound(bIsVisible);
					});

				NewWidget->EntryToActionPhase.BindUObject(this, &ATsunaController::Unpause);

				//set checkBox:
				NewWidget->SetCheckState(bIsVisible);

				//Safe ptr to widget
				CurrentWidget = NewWidget;
			}
		}

		//Add Widget to ViewPort:
		CurrentWidget->AddToViewport();
		//Unhandled exception at 0x0000000000000000 in UE4Editor - Win64 - DebugGame.exe: 0xC0000005 : Access violation executing location 0x0000000000000000.
		// fixed via adding UPROPERTY() to CurrentWidget
	}
	else
	{
		// remove Widget:
		if (CurrentWidget)
		{
			CurrentWidget->RemoveFromViewport();
		}

		//SetUpInputMode for Game
		SetInputMode(FInputModeGameOnly());

		// triger Pawn to get new Vars
		ToPawnTextSender.ExecuteIfBound(CharacterName);

		// triget Pawn to get new Vars
		On3DWidgetVisibleUpdate.ExecuteIfBound(bIsVisible);
	}

	//Set cursor visibility
	bShowMouseCursor = isPaused;
}


void ATsunaController::Unpause()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	OnPauseCall(false);
}

void ATsunaController::UICall()
{
	bIsUIOpen = !bIsUIOpen;

	OnPauseCall(bIsUIOpen);
	UGameplayStatics::SetGamePaused(GetWorld(), bIsUIOpen);
}
