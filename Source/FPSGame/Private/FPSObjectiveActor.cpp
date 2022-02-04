// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSObjectiveActor.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FPSCharacter.h"
// Sets default values
AFPSObjectiveActor::AFPSObjectiveActor()
{

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // 将碰撞只用于raycasts, sweeps, overlap, 这些查询通道
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore); // 忽略所有通道的碰撞
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); // spherecomp的碰撞通道设置为 pawn,overlap
 	SphereComp->SetupAttachment(RootComponent);

	// 需要在碰撞的时候触发PlayEffects
	// 可以像以前一样addDynamic待补充
	// 注意AddDynamic()绑定的函数，一定要像OnCollision(...) ...中参数对齐, 否则编译不通过
	// SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSObjectiveActor::OnCollision);

	SetReplicates(true);
}

// Called when the game starts or when spawned
void AFPSObjectiveActor::BeginPlay()
{
	Super::BeginPlay();
	
	PlayEffects();
}

void AFPSObjectiveActor::PlayEffects()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, PickupFX, GetActorLocation());
}

//void AFPSObjectiveActor::OnCollision(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	PlayEffects();
//}


void AFPSObjectiveActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	PlayEffects();

	if (GetLocalRole() == ROLE_Authority)
	{
		AFPSCharacter* MyCharacter = Cast<AFPSCharacter>(OtherActor);
		if (MyCharacter)
		{
			MyCharacter->bIsCarryingObjective = true;

			// 销毁物体, 造成物体被玩家拾取的假象
			Destroy();
		}
	}

}

