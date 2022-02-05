// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGameState.h"
#include "FPSPlayerController.h"

void AFPSGameState::MulticastOnMissionComplete_Implementation(APawn* InstigatorPawn, bool IsMissionComplete)
{

	for (FConstPlayerControllerIterator it = GetWorld()->GetPlayerControllerIterator(); it; it++)
	{
		AFPSPlayerController* PC = Cast<AFPSPlayerController>(it->Get());
		if (PC && PC->IsLocalController())
		{
			PC->OnMissionComplete(InstigatorPawn, IsMissionComplete);
		}

		APawn* Pawn = PC->GetPawn();
		if (Pawn && Pawn->IsLocallyControlled())
		{
			Pawn->DisableInput(PC);
		}
	}
}
