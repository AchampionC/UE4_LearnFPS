// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSBlackHole.h"
#include "Components/SphereComponent.h"

// Sets default values
AFPSBlackHole::AFPSBlackHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;

	BigSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Big Sphere"));
	BigSphere->SetupAttachment(RootComponent);

	SmallSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Small Sphere"));
	SmallSphere->SetupAttachment(RootComponent);
	SmallSphere->OnComponentBeginOverlap.AddDynamic(this, &AFPSBlackHole::OverlapSmallSphere);
}

void AFPSBlackHole::OverlapSmallSphere(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		OtherActor->Destroy();
	}
}

// Called every frame
void AFPSBlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<UPrimitiveComponent*> OverlappingComps;
	BigSphere->GetOverlappingComponents(OverlappingComps);

	for (int i = 0; i < OverlappingComps.Num(); i++)
	{
		UPrimitiveComponent* PrimComp = OverlappingComps[i];
		if (PrimComp && PrimComp->IsSimulatingPhysics())
		{
			PrimComp->AddRadialForce(GetActorLocation(), BigSphere->GetScaledSphereRadius(), -2000.0f, RIF_Linear, true);
		}
	}
}

void AFPSBlackHole::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (OtherActor)
	{
		OtherActor->Destroy();
	}
}

