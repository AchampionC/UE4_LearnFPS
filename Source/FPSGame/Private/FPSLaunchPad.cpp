// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSLaunchPad.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "FPSCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFPSLaunchPad::AFPSLaunchPad()
{
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetBoxExtent(FVector(75,75,20));
	BoxComp->SetupAttachment(MeshComp);
	RootComponent = BoxComp;
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSLaunchPad::OverlapHandler);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(BoxComp);

	LaunchStrength = 1500.0f;
	LaunchPitchAngle = 35.0f;
}

void AFPSLaunchPad::OverlapHandler(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FRotator LaunchDirection = GetActorRotation();
	LaunchDirection.Pitch += LaunchPitchAngle;
	FVector LanchVelocity = LaunchDirection.Vector() * LaunchStrength;

	if (OtherActor)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ExplisionEffects, GetActorLocation());
		AFPSCharacter* myPlayer = Cast<AFPSCharacter>(OtherActor);
		if (myPlayer)
		{
			myPlayer->LaunchCharacter(LanchVelocity, true, true); // true 表示覆盖character原有的速度

			UGameplayStatics::SpawnEmitterAtLocation(this, ExplisionEffects, GetActorLocation());
		}
		else if (OtherComp && OtherComp->IsSimulatingPhysics())
		{
			OtherComp->AddImpulse(LanchVelocity, NAME_None, true);
			UGameplayStatics::SpawnEmitterAtLocation(this, ExplisionEffects, GetActorLocation());
		}
	}
}


