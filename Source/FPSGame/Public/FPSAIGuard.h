// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSAIGuard.generated.h"

class UPawnSensingComponent;

UENUM(BlueprintType)
enum class EAIState : uint8
{
	Idle, 
	Suspicious,
	Altered
};

UCLASS()
class FPSGAME_API AFPSAIGuard : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSAIGuard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "PawnSensing")
	UPawnSensingComponent* PawnSensingComp;

	UFUNCTION()
	void OnSeenPawn(APawn* SeenPawn);

	UFUNCTION()
	void OnHeardPawn(APawn* NoiseInstigator, const FVector& Location, float Volume);

	FRotator OriginRotator;

	UFUNCTION()
		void ResetOriginRotation();

	FTimerHandle TimerHandler_ResetOriginRotation;

	EAIState GuardState;

	void SetGuardState(EAIState NewState);

	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void OnStateChanged(EAIState NewState);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
