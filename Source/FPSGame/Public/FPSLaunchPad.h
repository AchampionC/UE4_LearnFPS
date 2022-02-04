// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSLaunchPad.generated.h"

class UBoxComponent;
UCLASS()
class FPSGAME_API AFPSLaunchPad : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFPSLaunchPad();

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		UBoxComponent* BoxComp;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		UStaticMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		UDecalComponent* Decal1;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
		UDecalComponent* Decal2;
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
		UParticleSystem* ExplisionEffects;
protected:

	UFUNCTION()
		void OverlapHandler(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:

};
