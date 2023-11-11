// Copyright Epic Games, Inc. All Rights Reserved.

#include "SHCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


//////////////////////////////////////////////////////////////////////////
// ASHCharacter

ASHCharacter::ASHCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	//Walk Jog speed info
	FPlayerMovementInfo SpeedData;
	WalkJogSpeedData.Add(ESpeedStates::Joging, SpeedData);
	
	SpeedData.MaxWalkSpeed = 200.f;
	SpeedData.MaxAcceleration = 400.f;
	SpeedData.BrakingDeceleration = 400.f;
	WalkJogSpeedData.Add(ESpeedStates::Walking, SpeedData);
}

void ASHCharacter::BeginPlay()
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

//////////////////////////////////////////////////////////////////////////
// Input

void ASHCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASHCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASHCharacter::Look);
		EnhancedInputComponent->BindAction(WalkJogSwitcherAction, ETriggerEvent::Started, this, &ASHCharacter::WalkJogSwitcher_Implementation);
		EnhancedInputComponent->BindAction(WalkJogSwitcherAction, ETriggerEvent::Completed, this, &ASHCharacter::WalkJogSwitcher_Implementation);

	}

}

void ASHCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ASHCharacter::Look(const FInputActionValue& Value)
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

void ASHCharacter::WalkJogSwitcher_Implementation(const FInputActionValue& Value)
{
	ESpeedStates NewState = CharacterSpeedState;
	switch (CharacterSpeedState)
	{
	case ESpeedStates::Walking:
		NewState = ESpeedStates::Joging;
		break;
	case ESpeedStates::Joging:
		NewState = ESpeedStates::Walking;
		break;
	default:
		break;
	}

	SetGaitData_Implementation(NewState);
}

void ASHCharacter::SetGaitData_Implementation(ESpeedStates State)
{
	CharacterSpeedState = State;
	UpdateMovementDependsOnState(WalkJogSpeedData.Find(CharacterSpeedState), GetCharacterMovement());
}

ESpeedStates ASHCharacter::ReciveGaitData_Implementation()
{
	return CharacterSpeedState;
}

void UpdateMovementDependsOnState(FPlayerMovementInfo* SpeedDataRef, UCharacterMovementComponent* CharacterMovementRef)
{
	if (!SpeedDataRef) return;

	if (!CharacterMovementRef) return;

	CharacterMovementRef->MaxWalkSpeed = SpeedDataRef->MaxWalkSpeed;
	CharacterMovementRef->MaxAcceleration = SpeedDataRef->MaxAcceleration;
	CharacterMovementRef->BrakingFrictionFactor = SpeedDataRef->BrakingFrictionFactor;
	CharacterMovementRef->BrakingDecelerationWalking = SpeedDataRef->BrakingDeceleration;
	CharacterMovementRef->BrakingFriction = SpeedDataRef->BrakingFriction;
	CharacterMovementRef->bUseSeparateBrakingFriction = SpeedDataRef->bUseSeparateBrakingFriction;
}
