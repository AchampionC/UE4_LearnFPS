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
	BigSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // 将碰撞只用于raycasts, sweeps, overlap, 这些查询通道
	BigSphere->SetCollisionResponseToAllChannels(ECR_Ignore); // 忽略所有通道的碰撞
	BigSphere->SetCollisionResponseToChannel(ECC_Visibility, ECR_Overlap); // 将碰撞只用于raycasts, sweeps, overlap, 这些查询通道
	BigSphere->SetupAttachment(RootComponent);

	SmallSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Small Sphere"));
	SmallSphere->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly); // 将碰撞只用于raycasts, sweeps, overlap, 这些查询通道
	SmallSphere->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AFPSBlackHole::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFPSBlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TArray<AActor*> Actors;
	BigSphere->GetOverlappingActors(Actors);
	for (AActor* actor : Actors)
	{
		UStaticMeshComponent* meshcomp = Cast<UStaticMeshComponent>(actor->GetComponentByClass(UStaticMeshComponent::StaticClass()));
		if (meshcomp)
		{
			meshcomp->AddRadialForce(GetActorLocation(), BigSphere->GetScaledSphereRadius(), 100.0f, RIF_Linear);
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

