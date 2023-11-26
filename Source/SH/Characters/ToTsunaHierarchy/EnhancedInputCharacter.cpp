// Fill out your copyright notice in the Description page of Project Settings.


#include "EnhancedInputCharacter.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


AEnhancedInputCharacter::AEnhancedInputCharacter()
{
}

void AEnhancedInputCharacter::Move(const FInputActionValue& Value)
{
	Super::Move(Value);

	// find out which way is forward
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector ForwardDirection = ReturnDirection(YawRotation, EAxis::X);

	// get right vector 
	const FVector RightDirection = ReturnDirection(YawRotation, EAxis::Y);

	// add movement 
	AddMovementInput(ForwardDirection, Value.Get<FVector2D>().Y);
	AddMovementInput(RightDirection, Value.Get<FVector2D>().X);
}

void AEnhancedInputCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}


//////////////////////////////////////////////////////////////////////////
// Input

void AEnhancedInputCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		//Jumping
		EnhancedInputComponent->BindAction(ClimbAction, ETriggerEvent::Started, this, &AClimbingSystemCharacter::OnClimbActionStarted);
		EnhancedInputComponent->BindAction(ClimbAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AEnhancedInputCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AEnhancedInputCharacter::Look);
		EnhancedInputComponent->BindAction(WalkJogSwitcherAction, ETriggerEvent::Started, this, &ASHCharacter::WalkJogSwitcher_Implementation);
		EnhancedInputComponent->BindAction(WalkJogSwitcherAction, ETriggerEvent::Completed, this, &ASHCharacter::WalkJogSwitcher_Implementation);


		//Crouch
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AEnhancedInputCharacter::CrouchSwitcher_Implementation);
		//EnhancedInputComponent->BindAction(HideAction, ETriggerEvent::Started, this, &ACrouchCharacter::HideSwitcher_Implementation);

		EnhancedInputComponent->BindAction(HideAction, ETriggerEvent::Started, this, &AEnhancedInputCharacter::HideSwitcher_Implementation);

		//Throwing
		EnhancedInputComponent->BindAction(ThrowAction, ETriggerEvent::Completed, this, &AEnhancedInputCharacter::Throw);
		//Throwing
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Triggered, this, &AEnhancedInputCharacter::ShowParticlePath);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &AEnhancedInputCharacter::RemoveParticlePath);
		//Throwing
		EnhancedInputComponent->BindAction(KnifeBackAction, ETriggerEvent::Started, this, &AEnhancedInputCharacter::SpawnKnifeBack);

		//E
		EnhancedInputComponent->BindAction(EAction, ETriggerEvent::Started, this, &AEnhancedInputCharacter::Interactive);
		EnhancedInputComponent->BindAction(EAction, ETriggerEvent::Completed, this, &AEnhancedInputCharacter::Interactive);
	
	}
}


void AEnhancedInputCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	WalkJogSwitcher_Implementation(false);
}