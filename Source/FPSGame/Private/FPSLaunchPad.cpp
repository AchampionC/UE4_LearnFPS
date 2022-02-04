// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSLaunchPad.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "FPSCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFPSLaunchPad::AFPSLaunchPad()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetBoxExtent(FVector(200));
	BoxComp->SetupAttachment(MeshComp);

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSLaunchPad::OverlapHandler);

	Decal1 = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal1"));
	Decal1->SetupAttachment(MeshComp);
	Decal1->DecalSize = (FVector(200));
	Decal2 = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal2"));
	Decal2->DecalSize = (FVector(200));
	Decal2->SetupAttachment(MeshComp);
}

void AFPSLaunchPad::OverlapHandler(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ExplisionEffects, GetActorLocation());
		AFPSCharacter* myPlayer = Cast<AFPSCharacter>(OtherActor);
		if (myPlayer)
		{
			myPlayer->LaunchCharacter(FVector(2000.0f, 2000.0f, 2000.0f), false, false);
		}

		if (OtherActor)
		{
			TSubclassOf<UStaticMeshComponent>aa;
			UStaticMeshComponent* meshComp = Cast<UStaticMeshComponent>(OtherActor->GetComponentByClass(aa));
			if (meshComp)
			{
				meshComp->AddForce(FVector(10000, 10000, 10000));
			}
		}
	}
}


