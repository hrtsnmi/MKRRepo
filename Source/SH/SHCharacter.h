// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Interfaces/WalkJogSwitcherInterface.h"
#include "Interfaces/GroundDistanceInterface.h"
#include "SHCharacter.generated.h"



void UpdateMovementDependsOnState(FPlayerMovementInfo* SpeedDataRef, UCharacterMovementComponent* CharacterMovementRef);

UCLASS(config=Game)
class ASHCharacter : public ACharacter,
	public IWalkJogSwitcherInterface,
	public IGroundDistanceInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* WalkJogSwitcherAction;

public:
	ASHCharacter();
	

protected:

	/** Called for movement input */
	virtual const FVector ReturnDirection(const FRotator& YawRotation, EAxis::Type coord) const;
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

protected:
	void WalkJogSwitcher_Implementation(const FInputActionValue& Value);
	
	virtual FPlayerMovementInfo* GetPlayerMovementInfo();
	UPROPERTY(EditDefaultsOnly, Category = "Player Movement|Info") TMap<ESpeedStates, FPlayerMovementInfo> WalkJogSpeedData;
	UPROPERTY(EditAnywhere, Category = "Player Movement|Info") ESpeedStates CharacterSpeedState { ESpeedStates::Walking };
public:
	ESpeedStates ReciveGaitData_Implementation();
	void SetGaitData_Implementation(ESpeedStates State);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

public:
	float GetGroundDistance_Implementation() const;

};

