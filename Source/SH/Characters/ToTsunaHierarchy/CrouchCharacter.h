// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ThrowCharacter.h"
#include "CrouchCharacter.generated.h"

/**
 * 
 */

UCLASS()
class SH_API ACrouchCharacter : public AThrowCharacter
	
{
	GENERATED_BODY()

public:
	ACrouchCharacter();

protected:
	virtual void BeginPlay() override;

protected:
	virtual void CrouchSwitcher_Implementation(const FInputActionValue& Value) override;
	virtual void SetCrouchData_Implementation(ECrouchStates State) override;
	virtual FPlayerMovementInfo* GetPlayerMovementInfo() override;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Movement|Info") TMap<ESpeedStates, FPlayerMovementInfo> CrouchSpeedData;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Movement|Info") ECrouchStates CharacterCrouchState { ECrouchStates::Stand };

public:
	virtual ECrouchStates ReciveCrouchData_Implementation() override;

protected:
	UPROPERTY() EEntryExitStates EntryExitState;

public:
	virtual void SetEntryExitStateCrouchData_Implementation(EEntryExitStates State) override;
	virtual EEntryExitStates ReciveCrouchEntryData_Implementation() override;
};
