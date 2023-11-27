// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CustomMovementComponent.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
namespace ECustomMovementMode
{
	enum Type
	{
		MOVE_Climb UMETA(DisplayName = "Climb Mode")
	};
}

class UAnimMontage;
class UAnimInstance;
class AClimbingSystemCharacter;

UCLASS()
class SH_API UCustomMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
protected:

#pragma region OverridenFunctions
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;

	virtual void PhysCustom(float deltaTime, int32 Iterations) override;

	virtual FVector ConstrainAnimRootMotionVelocity(const FVector& RootMotionVelocity, const FVector& CurrentVelocity) const override;

	virtual float GetMaxSpeed() const override;
	virtual float GetMaxAcceleration() const override;

#pragma endregion

private:

#pragma region ClimbTraces
	void SetOnwnerControlRotationYaw(bool bControlRotationYaw);

	void PhysClimb(float deltaTime, int32 Iterations);
	
	void ProcessClimableSurfaceInfo();
	
	bool CheckShouldStopClimbing();

	bool CheckHasReachedFloor();

	FQuat GetClimbRotation(float DeltaTime);

	bool TraceClimbableSurfaces();

	void SnapMovementToClimableSurfaces(float DeltaTime);

	bool CheckHasReachedLedge();

	void TryStartVaulting();

	bool CanStartVaulting(FVector& OutVaultStartPosition, FVector& OutVaultMidPosition, FVector& OutVaultLandPosition);

	void PlayClimbMontage(UAnimMontage* MontageToPlay);

	UFUNCTION()
		void OnClimbMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	void SetMotionWarpTarget(const FName& InWarpTargetName, const FVector& InTargetPosition);
	
	TArray<FHitResult> DoCapsuleTraceMultiByObject(const FVector& Start, const FVector& End, bool bShowDebugShape = false, bool bDrawPersistantShapes = false);
	
	FHitResult DoLineTraceSingleByObject(const FVector& Start, const FVector& End, bool bShowDebugShape = false, bool bDrawPersistantShapes = false);

#pragma endregion

#pragma region ClimbCore

	FHitResult TraceFromEyeHeight(float TraceDistance, float TraceStartOffset = 0.f);

	bool CanStartClimbing();

	bool CanClimbDownLedge();

	void StartClimbing();

	void StopClimbing();

#pragma endregion

#pragma region ClimbCoreVariables

	TArray<FHitResult> ClimbableSurfacesTracedResults;

	UPROPERTY()
		UAnimInstance* OwningPlayerAnimInstance; 

	UPROPERTY()
		AClimbingSystemCharacter* OwningPlayerCharacter;
	
	FVector CurrentClimbableSurfaceLocation;

	FVector CurrentClimbableSurfaceNormal;

#pragma endregion

#pragma region ClimbBPVariables

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: Climbing", meta = (AllowPrivateAccess = "true"))
		TArray<TEnumAsByte<EObjectTypeQuery> > ClimbableSurfaceTraceTypes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: Climbing", meta = (AllowPrivateAccess = "true"))
		float ClimbCapsuleTraceRadius = 50.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: Climbing", meta = (AllowPrivateAccess = "true"))
		float ClimbCapsuleTraceHalfHeight = 72.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: Climbing", meta = (AllowPrivateAccess = "true"))
		float MaxBreakClimbDeceleration = 400.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: Climbing", meta = (AllowPrivateAccess = "true"))
		float MaxClimbSpeed = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: Climbing", meta = (AllowPrivateAccess = "true"))
		float MaxClimbAcceleration = 300.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: Climbing", meta = (AllowPrivateAccess = "true"))
		float ClimbDownWalkableSurfaceTraceOffset = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: Climbing", meta = (AllowPrivateAccess = "true"))
		float ClimbDownLedgeTraceOffset = 50.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: Climbing", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* IdleToClimbMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: Climbing", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* ClimbToIdleMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: Climbing", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* ClimbToTopMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: Climbing", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* ClimbDownLedgeMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: Climbing", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* VaultMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: Climbing", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* TurnMontage;

#pragma endregion

public:
	void ToggleClimbing(bool bEnableClimb);
	bool IsClimbing() const;

	FORCEINLINE FVector GetClimbableSurfaceNormal() const { return CurrentClimbableSurfaceNormal; }

	void PlayTurnMontage(const FName& MontageSlot = "Default");
	bool IsActiveMontage() const;

	FVector GetUnrotatedClimbVelocity() const;
};
