// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "ClimbingSystemCharacter.generated.h"


class UCustomMovementComponent;

UCLASS()
class SH_API AClimbingSystemCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		UCustomMovementComponent* CustomMovementComponent;

public:
	// Sets default values for this character's properties
	AClimbingSystemCharacter();
	AClimbingSystemCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void HandleClimbMovementInput(const FInputActionValue& Value);

public:	

	virtual void Move(const FInputActionValue& Value);

	void OnClimbActionStarted(const FInputActionValue& Value);


	FORCEINLINE UCustomMovementComponent* GetCustomMovementComponent() const { return CustomMovementComponent; }
};
