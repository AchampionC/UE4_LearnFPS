// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSAIGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "FPSGameMode.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

// Sets default values
AFPSAIGuard::AFPSAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnSeenPawn);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AFPSAIGuard::OnHeardPawn);

	GuardState = EAIState::Idle;


}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();
	OriginRotator = GetActorRotation();

	if (bPatrol)
	{
		MoveToNextPatrolPoint();
	}
}

void AFPSAIGuard::OnSeenPawn(APawn* SeenPawn)
{
	if (SeenPawn == nullptr)
	{
		return;
	}

	DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 32.0f, 12, FColor::Red, false, 10.0f);

	AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		GM->MissionComplete(SeenPawn, false);
	}

	SetGuardState(EAIState::Altered);

	AController* Controller = GetController();
	if (Controller)
	{
		Controller->StopMovement();
	}
}

void AFPSAIGuard::OnHeardPawn(APawn* NoiseInstigator, const FVector& Location, float Volume)
{

	if (GuardState == EAIState::Altered)
	{
		return;
	}

	DrawDebugSphere(GetWorld(), Location, 32.0f, 12, FColor::Green, false, 10.0f);
	
	FVector Direction = Location - GetActorLocation();
	Direction.Normalize();
	FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
	// 为了让AIGuard正确的站立, 而不是斜着站立, 因为旋转向量有 Pitch, Yaw, Roll三个分量
	NewLookAt.Pitch = 0;
	NewLookAt.Roll = 0;
	SetActorRotation(NewLookAt);
	
	
	// 设置定时器的时候, 先删除上一个定时器
	GetWorldTimerManager().ClearTimer(TimerHandler_ResetOriginRotation);
	GetWorldTimerManager().SetTimer(TimerHandler_ResetOriginRotation, this, &AFPSAIGuard::ResetOriginRotation, 3.0f, false);

	SetGuardState(EAIState::Suspicious);

	AController* Controller = GetController();
	if (Controller)
	{
		Controller->StopMovement();
	}
}

void AFPSAIGuard::ResetOriginRotation()
{
	if (GuardState == EAIState::Altered)
	{
		return;
	}
	SetActorRotation(OriginRotator);
	SetGuardState(EAIState::Idle);

	if (bPatrol)
	{
		MoveToNextPatrolPoint();
	}
}

void AFPSAIGuard::SetGuardState(EAIState NewState)
{
	if (GuardState == NewState)
	{
		return;
	}

	GuardState = NewState;

	// 调用蓝图逻辑, 方便更改Text
	OnStateChanged(NewState);
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentPatrolPoint)
	{
		FVector Distance = GetActorLocation() - CurrentPatrolPoint->GetActorLocation();
		float DistanceToGoal = Distance.Size();
		UE_LOG(LogTemp, Warning, TEXT("Distance = %f"), DistanceToGoal);
		if (DistanceToGoal < 50)
		{
			MoveToNextPatrolPoint();
		}
	}
}

void AFPSAIGuard::MoveToNextPatrolPoint()
{
	if (CurrentPatrolPoint == nullptr || CurrentPatrolPoint == SecondPatrolPoint)
	{
		CurrentPatrolPoint = FirstPatrolPoint;
	}
	else
	{
		CurrentPatrolPoint = SecondPatrolPoint;
	}

	UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), CurrentPatrolPoint);
}

