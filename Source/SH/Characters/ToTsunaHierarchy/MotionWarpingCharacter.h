// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HideCharacter.h"
#include "MotionWarpingCharacter.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMoveToTargetSignature, FVector, Location);

UCLASS()
class SH_API AMotionWarpingCharacter : public AHideCharacter
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = "Warping Component", meta = (AllowPrivateAccess = "true"))
		class UMotionWarpingComponent* WarpingComponent;
	//Timer
	FTimerHandle AfterVaultTH;

	UPROPERTY(EditAnywhere, Category = "Warping Component", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* KillMontage;
	UPROPERTY(EditAnywhere, Category = "Warping Component", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* VaultMontage;

protected:
	virtual void Throw(const FInputActionValue& Value) override;
	virtual void Interactive(const FInputActionValue& Value) override;

	bool Vault(FVector& VStart, FVector& VMid, FVector& End);
	void AfterVault();

	virtual void Jump() override;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bEButtonPressed{ false };

protected:

	void Attack(AActor* Enemy);

public:
	AMotionWarpingCharacter();
};
