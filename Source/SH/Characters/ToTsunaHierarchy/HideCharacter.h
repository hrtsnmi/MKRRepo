// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CrouchCharacter.h"
#include "HideCharacter.generated.h"

/**
 * 
 */

void TraceToCover(AActor* Owner, UWorld* Context, const FVector& TraceStart, const FVector& TraceEnd,
	FHitResult& TraceHit, FVector& UpdateLocation, const FName& Tag, ECollisionChannel FindCollision);

UCLASS()
class SH_API AHideCharacter : public ACrouchCharacter
{
	GENERATED_BODY()
	
private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* BoxComp;

public:
	AHideCharacter();

protected:
	virtual void BeginPlay() override;

protected:
	/** Called for movement input */
	virtual const FVector ReturnDirection(const FRotator& YawRotation, EAxis::Type coord) const override;

private:
	void SutUpHideBorders(const FVector& CoverLocation);
	void UpdateBorderLocation(bool bUpdateRightBorder);

	void ToHide();
	void FromHide();

	UPROPERTY() FVector LeftEndPoint { FVector::UpVector };
	UPROPERTY() FVector RightEndPoint { FVector::UpVector };
	UPROPERTY() FVector Right;
	UPROPERTY() FVector CoverNormal;
	UPROPERTY() FVector DistanceToCover;
	bool bReachRightBorder{ false };
	bool bReachLeftBorder{ false };
	UPROPERTY(EditAnywhere, Category = "Player Movement|Hide") FVector AttachLocation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: Climbing", meta = (AllowPrivateAccess = "true"))
		TEnumAsByte<ECollisionChannel> FindCollision;

protected:
	UFUNCTION()
		void BoxComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void BoxComponentEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	bool bCanHide{ false };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Movement|Info")
		EHideStates HideState {
		EHideStates::Visible
	};
	bool bNeedToReachCover{ false };

	void GoToCoverWhileHit();

	UFUNCTION()
		void OnCapsuleHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY() AActor* BoxRight;
	UPROPERTY() AActor* BoxLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Movement|Hide")
		TSubclassOf<AActor> Border;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Movement|Hide") FName SeatchTag { "Cover" };

protected:
	virtual void HideSwitcher_Implementation(const FInputActionValue& Value) override;
	virtual void SetHideData_Implementation(EHideStates State) override;

public:
	virtual EHideStates ReciveHideData_Implementation() override;


	void Tick(float DeltaSeconds);

	virtual void Move(const FInputActionValue& Value) override;
};
