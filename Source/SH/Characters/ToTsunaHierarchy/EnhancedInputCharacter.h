// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../SHCharacter.h"
#include "../../Interfaces/CrouchInterface.h"
#include "../../Interfaces/HideInterface.h"
#include "InputActionValue.h"
#include "EnhancedInputCharacter.generated.h"

/**
 * 
 */
UCLASS()
class SH_API AEnhancedInputCharacter : public ASHCharacter,
	public ICrouchInterface,
	public IHideInterface
{
	GENERATED_BODY()
	
private:
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
	
	//Crouch
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* CrouchAction;

	//Hide
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* HideAction;

	//Throw
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* ThrowAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* AimAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* KnifeBackAction;

	//montage
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* EAction;


protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// To add mapping context
	virtual void BeginPlay();

public:
	AEnhancedInputCharacter();


	virtual void Move(const FInputActionValue& Value) override;
	virtual void Look(const FInputActionValue& Value) override;

	//Crouch
protected:
	virtual void CrouchSwitcher_Implementation(const FInputActionValue& Value) PURE_VIRTUAL(AEnhancedInputCharacter::CrouchSwitcher_Implementation, ;);
	virtual void SetCrouchData_Implementation(ECrouchStates State) PURE_VIRTUAL(AEnhancedInputCharacter::SetCrouchData_Implementation, ;);

public:
	virtual ECrouchStates ReciveCrouchData_Implementation()
		PURE_VIRTUAL(AEnhancedInputCharacter::ReciveCrouchData_Implementation, return ECrouchStates::Crouch;);

	virtual void SetEntryExitStateCrouchData_Implementation(EEntryExitStates State)
		PURE_VIRTUAL(AEnhancedInputCharacter::SetEntryExitStateCrouchData_Implementation, ;);

	virtual EEntryExitStates ReciveCrouchEntryData_Implementation()
		PURE_VIRTUAL(AEnhancedInputCharacter::ReciveCrouchEntryData_Implementation, return EEntryExitStates::Enrty;);
	
	//Hide
protected:
	virtual void HideSwitcher_Implementation(const FInputActionValue& Value) PURE_VIRTUAL(AEnhancedInputCharacter::HideSwitcher_Implementation, ;);
	virtual void SetHideData_Implementation(EHideStates State) PURE_VIRTUAL(AEnhancedInputCharacter::SetHideData_Implementation, ;);

public:
	virtual EHideStates ReciveHideData_Implementation()
		PURE_VIRTUAL(AEnhancedInputCharacter::ReciveHideData_Implementation, return EHideStates::Hide;);

	//Throw
protected:
	/** Called for movement input */
	virtual void Throw(const FInputActionValue& Value)
		PURE_VIRTUAL(AEnhancedInputCharacter::Throw, ;);

	virtual void RemoveParticlePath(const FInputActionValue& Value)
		PURE_VIRTUAL(AEnhancedInputCharacter::RemoveParticlePath, ;);

	/** Called for movement input */
	virtual void ShowParticlePath(const FInputActionValue& Value)
		PURE_VIRTUAL(AEnhancedInputCharacter::ShowParticlePath, ;);

	/** Called for movement input */
	virtual void SpawnKnifeBack(const FInputActionValue& Value)
		PURE_VIRTUAL(AEnhancedInputCharacter::SpawnKnifeBack, ;);


protected:
	virtual void Interactive(const FInputActionValue & Value)
		PURE_VIRTUAL(AEnhancedInputCharacter::Interactive, ;);
};
