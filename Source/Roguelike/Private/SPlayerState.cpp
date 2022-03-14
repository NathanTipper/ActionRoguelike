// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"
#include "SSaveGame.h"

ASPlayerState::ASPlayerState() :
	PlayerCredits(0.0f)
{

}

bool ASPlayerState::ApplyCredits(float Amount)
{
	if (Amount < 0.0f && FMath::Abs(Amount) > PlayerCredits)
	{
		return false;
	}
	
	PlayerCredits += Amount;

	OnCreditsChanged.Broadcast(PlayerCredits, Amount);

	return true;
}

float ASPlayerState::GetPlayerCredits() const
{
	return PlayerCredits;
}

void ASPlayerState::SavePlayerState_Implementation(USSaveGame* SaveObject)
{
	if (SaveObject)
	{
		SaveObject->Credits = PlayerCredits;
	}
}

void ASPlayerState::LoadPlayerState_Implementation(USSaveGame* SaveObject)
{
	if (SaveObject)
	{
		PlayerCredits = SaveObject->Credits;
	}
}
