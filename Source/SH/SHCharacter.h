// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ClimbingSystemCharacter.h"
#include "Interfaces/WalkJogSwitcherInterface.h"
#include "Interfaces/GroundDistanceInterface.h"
#include "InputActionValue.h"
#include "SHCharacter.generated.h"



void UpdateMovementDependsOnState(FPlayerMovementInfo* SpeedDataRef, UCharacterMovementComponent* CharacterMovementRef);

UCLASS(config=Game)
class ASHCharacter : public AClimbingSystemCharacter,
	public IWalkJogSwitcherInterface,
	public IGroundDistanceInterface
{
	GENERATED_BODY()

public:
	ASHCharacter();
	
protected:

	/** Called for movement input */
	virtual FVector ReturnDirection(const FRotator& YawRotation, EAxis::Type coord);

public:
	/** Called for looking input */
	
	void WalkJogSwitcher_Implementation(const FInputActionValue& Value);

	virtual void Look(const FInputActionValue& Value) PURE_VIRTUAL(ASHCharacter::Look, ;);
	//virtual void Move(const FInputActionValue& Value) PURE_VIRTUAL(ASHCharacter::Move, ;);

protected:
	virtual FPlayerMovementInfo* GetPlayerMovementInfo();
	UPROPERTY(EditDefaultsOnly, Category = "Player Movement|Info") TMap<ESpeedStates, FPlayerMovementInfo> WalkJogSpeedData;
	UPROPERTY(EditAnywhere, Category = "Player Movement|Info") ESpeedStates CharacterSpeedState { ESpeedStates::Walking };
public:
	ESpeedStates ReciveGaitData_Implementation();
	void SetGaitData_Implementation(ESpeedStates State);

protected:
	virtual void BeginPlay() override;

public:
	float GetGroundDistance_Implementation() const;

};

