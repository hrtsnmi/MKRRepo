// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WalkJogSwitcherInterface.generated.h"


USTRUCT(BlueprintType)
struct FPlayerMovementInfo
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, Category = "Player Movement|Info")
		float MaxWalkSpeed{450.f};

	UPROPERTY(BlueprintReadWrite, Category = "Player Movement|Info")
		float MaxAcceleration{ 800 };

	UPROPERTY(BlueprintReadWrite, Category = "Player Movement|Info")
		float BrakingDeceleration{ 800 };

	UPROPERTY(BlueprintReadWrite, Category = "Player Movement|Info")
		float BrakingFrictionFactor{ 1.f };

	UPROPERTY(BlueprintReadWrite, Category = "Player Movement|Info")
		bool bUseSeparateBrakingFriction{ true };

	UPROPERTY(BlueprintReadWrite, Category = "Player Movement|Info")
		float BrakingFriction{ 0.f };
};


UENUM(BlueprintType)
enum class ESpeedStates : uint8
{
	Walking UMETA(DisplayName = "Walking"),
	Joging UMETA(DisplayName = "Joging"),
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UWalkJogSwitcherInterface : public UInterface
{
	GENERATED_BODY()

};

/**
 * 
 */
class SH_API IWalkJogSwitcherInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
protected:
	UFUNCTION(BlueprintNativeEvent) void WalkJogSwitcher(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent) void SetGaitData(ESpeedStates State);

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent) ESpeedStates ReciveGaitData();
};
