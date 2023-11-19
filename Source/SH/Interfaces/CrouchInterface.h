// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CrouchInterface.generated.h"


UENUM(BlueprintType)
enum class ECrouchStates : uint8
{
	Stand	UMETA(DisplayName = "Standing"),
	Crouch	UMETA(DisplayName = "Crouching"),
};

UENUM(BlueprintType)
enum class EEntryExitStates : uint8
{
	Exit	UMETA(DisplayName = "Exit"),
	Enrty	UMETA(DisplayName = "Enrty"),
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UCrouchInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SH_API ICrouchInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
protected:
	UFUNCTION(BlueprintNativeEvent) void CrouchSwitcher(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent) void SetCrouchData(ECrouchStates State);

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent) ECrouchStates ReciveCrouchData();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent) void SetEntryExitStateCrouchData(EEntryExitStates State);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent) EEntryExitStates ReciveCrouchEntryData();
};
