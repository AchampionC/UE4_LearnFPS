// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "FPSAIController.generated.h"

/**
 * 
 */

class ATargetPoint;

UCLASS()
class FPSGAME_API AFPSAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	void BeginPlay() override;

	void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

private:

	UPROPERTY()
		TArray<AActor*> WayPoints;

	UFUNCTION()
		ATargetPoint* GetRandomPoint();

	UFUNCTION()
		void GoToRandomWayPoint();
};
