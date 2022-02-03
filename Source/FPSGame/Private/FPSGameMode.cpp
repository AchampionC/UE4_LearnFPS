// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
AFPSGameMode::AFPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPSHUD::StaticClass();
}

void AFPSGameMode::MissionComplete(APawn* InstigatorPawn)
{
	if (InstigatorPawn)
	{
		InstigatorPawn->DisableInput(nullptr);

		if (SpectatingViewPointClass) {
			TArray<AActor*> ReturnedActors;
			UGameplayStatics::GetAllActorsOfClass(this, SpectatingViewPointClass, ReturnedActors);

			// change viewtarget if any valid actor found
			if (ReturnedActors.Num() > 0)
			{
				AActor* NewViewTarget = ReturnedActors[0];

				APlayerController* PC = Cast< APlayerController>(InstigatorPawn->GetController());
				if (PC)
				{
					PC->SetViewTargetWithBlend(NewViewTarget, 0.5, EViewTargetBlendFunction::VTBlend_Cubic);
				}
			}
			else 
			{
				UE_LOG(LogTemp, Warning, TEXT("SpectatingViewpointclass is nullptr. Please update Gamemode class with valid subclass. Cannot change spectating view target"));
			}
		}
	}

	// 蓝图实现任务完成逻辑
	OnMissionComplete(InstigatorPawn);

}
