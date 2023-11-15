// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FollowPatrolPathInterface.generated.h"

UENUM(BlueprintType)
enum class EPatrolStates : uint8
{
	Random UMETA(DisplayName = "Random"),
	Cycle UMETA(DisplayName = "Cycle"),
	FwdBwd UMETA(DisplayName = "FwdBwd"),
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UFollowPatrolPathInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SH_API IFollowPatrolPathInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
protected:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent) void SetPatrolData(EPatrolStates State);

public:
	UFUNCTION(BlueprintNativeEvent) FVector GoForwardBackwardPath();
	UFUNCTION(BlueprintNativeEvent) FVector GoCyclePath();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent) EPatrolStates RecivePatrolData();
};