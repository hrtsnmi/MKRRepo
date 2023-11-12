// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ThrowCharacter.h"
#include "../../Interfaces/CrouchInterface.h"
#include "../../Interfaces/HideInterface.h"
#include "CrouchCharacter.generated.h"

/**
 * 
 */
UCLASS()
class SH_API ACrouchCharacter : public AThrowCharacter,
	public ICrouchInterface,
	public IHideInterface
{
	GENERATED_BODY()
	
private:
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* CrouchAction;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
			class UInputAction* HideAction;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* BoxComp;

public:
	ACrouchCharacter();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BeginPlay();

protected:
	void CrouchSwitcher_Implementation(const FInputActionValue& Value);
	void SetCrouchData_Implementation(ECrouchStates State);
	virtual FPlayerMovementInfo* GetPlayerMovementInfo() override;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Movement|Info") TMap<ESpeedStates, FPlayerMovementInfo> CrouchSpeedData;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Movement|Info") ECrouchStates CharacterCrouchState { ECrouchStates::Stand };

protected:
	UFUNCTION()
		void BoxComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void BoxComponentEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	bool bCanHide{ false };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Movement|Info")
	EHideStates HideState{EHideStates::Visible };

protected:
	void HideSwitcher_Implementation(const FInputActionValue& Value);
	void SetHideData_Implementation(EHideStates State);

public:
	EHideStates ReciveHideData_Implementation();

public:
	ECrouchStates ReciveCrouchData_Implementation();
};
