// Fill out your copyright notice in the Description page of Project Settings.


#include "SCoin.h"
#include "SPlayerState.h"

void ASCoin::Interact_Implementation(APawn* InstigatorPawn)
{
	if (InstigatorPawn && bActive)
	{
		ASPlayerState* PlayerState = Cast<ASPlayerState>(InstigatorPawn->GetPlayerState());
		if (PlayerState && PlayerState->ApplyCredits(CreditDelta))
		{
			ToggleActive(false);
		}
	}
}
