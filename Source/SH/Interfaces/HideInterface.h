// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HideInterface.generated.h"


UENUM(BlueprintType)
enum class EHideStates : uint8
{
	Hide UMETA(DisplayName = "Hide"),
	Visible UMETA(DisplayName = "Visible"),
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UHideInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SH_API IHideInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
protected:
	UFUNCTION(BlueprintNativeEvent) void HideSwitcher(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent) void SetHideData(EHideStates State);

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent) EHideStates ReciveHideData();
};
