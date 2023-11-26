// Copyright Epic Games, Inc. All Rights Reserved.

#include "SHCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
//#include "Components/InputComponent.h"


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

	WalkJogSwitcher_Implementation(false);
}


FVector ASHCharacter::ReturnDirection(const FRotator& YawRotation, EAxis::Type coord)
{
	// TODO: insert return statement here
	switch (coord)
	{
	case EAxis::X:
	case EAxis::Y:
		return FRotationMatrix(YawRotation).GetUnitAxis(coord);
	default:
		return FVector::ZeroVector;
		break;
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
	UpdateMovementDependsOnState(GetPlayerMovementInfo(), GetCharacterMovement());
}

FPlayerMovementInfo* ASHCharacter::GetPlayerMovementInfo()
{
	return WalkJogSpeedData.Find(CharacterSpeedState);
}

ESpeedStates ASHCharacter::ReciveGaitData_Implementation()
{
	return CharacterSpeedState;
}

void UpdateMovementDependsOnState(FPlayerMovementInfo* SpeedDataRef, UCharacterMovementComponent* CharacterMovementRef)
{
	if (CharacterMovementRef && SpeedDataRef)
	{
		CharacterMovementRef->MaxWalkSpeed = SpeedDataRef->MaxWalkSpeed;
		CharacterMovementRef->MaxAcceleration = SpeedDataRef->MaxAcceleration;
		CharacterMovementRef->BrakingFrictionFactor = SpeedDataRef->BrakingFrictionFactor;
		CharacterMovementRef->BrakingDecelerationWalking = SpeedDataRef->BrakingDeceleration;
		CharacterMovementRef->BrakingFriction = SpeedDataRef->BrakingFriction;
		CharacterMovementRef->bUseSeparateBrakingFriction = SpeedDataRef->bUseSeparateBrakingFriction;
	}
}


float ASHCharacter::GetGroundDistance_Implementation() const
{
	FFindFloorResult OutFloorRes;

	GetCharacterMovement()->FindFloor(GetActorLocation(), OutFloorRes, false);
	
	return OutFloorRes.FloorDist;
}