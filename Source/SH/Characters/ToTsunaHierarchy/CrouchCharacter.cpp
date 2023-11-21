// Fill out your copyright notice in the Description page of Project Settings.


#include "CrouchCharacter.h"


ACrouchCharacter::ACrouchCharacter()
{


	
}

void ACrouchCharacter::BeginPlay()
{
	Super::BeginPlay();

	FPlayerMovementInfo SpeedData;
	SpeedData.MaxWalkSpeed = 250.f;
	SpeedData.MaxAcceleration = 500.f;
	SpeedData.BrakingDeceleration = 500.f;
	CrouchSpeedData.Add(ESpeedStates::Joging, SpeedData);

	SpeedData.MaxWalkSpeed = 180.f;
	SpeedData.MaxAcceleration = 360.f;
	SpeedData.BrakingDeceleration = 360.f;
	CrouchSpeedData.Add(ESpeedStates::Walking, SpeedData);

	CharacterCrouchState == ECrouchStates::Stand ?
		EntryExitState = EEntryExitStates::Enrty : EntryExitState = EEntryExitStates::Exit;

}

void ACrouchCharacter::CrouchSwitcher_Implementation(const FInputActionValue& Value)
{
	ECrouchStates NewState = CharacterCrouchState;
	FPlayerMovementInfo* NewMovementInfo = nullptr;

	switch (CharacterCrouchState)
	{
	case ECrouchStates::Stand:
		NewState = ECrouchStates::Crouch;
		break;
	case ECrouchStates::Crouch:
		NewState = ECrouchStates::Stand;
		break;
	default:
		break;
	}
	SetCrouchData_Implementation(NewState);
}

void ACrouchCharacter::SetCrouchData_Implementation(ECrouchStates State)
{
	CharacterCrouchState = State;
	UpdateMovementDependsOnState(GetPlayerMovementInfo(), GetCharacterMovement());
}

FPlayerMovementInfo* ACrouchCharacter::GetPlayerMovementInfo()
{
	switch (CharacterCrouchState)
	{
	case ECrouchStates::Stand:
		return Super::GetPlayerMovementInfo();
	case ECrouchStates::Crouch:
		return CrouchSpeedData.Find(CharacterSpeedState);;
	default:
		return nullptr;
	}
}

ECrouchStates ACrouchCharacter::ReciveCrouchData_Implementation()
{
	return CharacterCrouchState;
}

void ACrouchCharacter::SetEntryExitStateCrouchData_Implementation(EEntryExitStates State)
{
	EntryExitState = State;
}

EEntryExitStates ACrouchCharacter::ReciveCrouchEntryData_Implementation()
{
	return EntryExitState;
}