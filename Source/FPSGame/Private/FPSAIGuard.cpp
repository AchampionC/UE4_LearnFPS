// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSAIGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "FPSGameMode.h"

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
	// Ϊ����AIGuard��ȷ��վ��, ������б��վ��, ��Ϊ��ת������ Pitch, Yaw, Roll��������
	NewLookAt.Pitch = 0;
	NewLookAt.Roll = 0;
	SetActorRotation(NewLookAt);
	
	
	// ���ö�ʱ����ʱ��, ��ɾ����һ����ʱ��
	GetWorldTimerManager().ClearTimer(TimerHandler_ResetOriginRotation);
	GetWorldTimerManager().SetTimer(TimerHandler_ResetOriginRotation, this, &AFPSAIGuard::ResetOriginRotation, 3.0f, false);

	SetGuardState(EAIState::Suspicious);
}

void AFPSAIGuard::ResetOriginRotation()
{
	if (GuardState == EAIState::Altered)
	{
		return;
	}
	SetActorRotation(OriginRotator);
	SetGuardState(EAIState::Idle);
}

void AFPSAIGuard::SetGuardState(EAIState NewState)
{
	if (GuardState == NewState)
	{
		return;
	}

	GuardState = NewState;

	// ������ͼ�߼�, �������Text
	OnStateChanged(NewState);
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

