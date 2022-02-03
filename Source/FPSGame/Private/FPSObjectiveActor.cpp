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
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // ����ײֻ����raycasts, sweeps, overlap, ��Щ��ѯͨ��
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore); // ��������ͨ������ײ
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); // spherecomp����ײͨ������Ϊ pawn,overlap
 	SphereComp->SetupAttachment(RootComponent);

	// ��Ҫ����ײ��ʱ�򴥷�PlayEffects
	// ��������ǰһ��addDynamic������
	// ע��AddDynamic()�󶨵ĺ�����һ��Ҫ��OnCollision(...) ...�в�������, ������벻ͨ��
	// SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSObjectiveActor::OnCollision);
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

	AFPSCharacter* MyCharacter = Cast<AFPSCharacter>(OtherActor);
	if (MyCharacter)
	{
		MyCharacter->bIsCarryingObjective = true;
		
		// ��������, ������屻���ʰȡ�ļ���
		Destroy();
	}
}

