// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSAIController.h"
#include "Engine/TargetPoint.h"
#include "Kismet/GameplayStatics.h"

void AFPSAIController::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), WayPoints);

	GoToRandomWayPoint();

}

void AFPSAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	GoToRandomWayPoint();
}

ATargetPoint* AFPSAIController::GetRandomPoint()
{
	int index = FMath::RandRange(0, WayPoints.Num() - 1);
	return Cast<ATargetPoint>(WayPoints[index]);
}

void AFPSAIController::GoToRandomWayPoint()
{
	MoveToActor(GetRandomPoint());
}
