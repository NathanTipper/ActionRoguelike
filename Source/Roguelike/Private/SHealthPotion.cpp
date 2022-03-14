// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPotion.h"
#include "SAttributeComponent.h"
#include "SPlayerState.h"

ASHealthPotion::ASHealthPotion() : 
	ASPowerUp(),
	HealthGain(20.0f)
{
	
}

void ASHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (InstigatorPawn && bActive)
	{
		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (ensure(AttributeComp))
		{
			ASPlayerState* PS = Cast<ASPlayerState>(InstigatorPawn->GetPlayerState());
			if(PS && PS->ApplyCredits(CreditDelta))
			{ 
				if (!AttributeComp->HasFullHealth() && AttributeComp->ApplyHealthChange(this, HealthGain))
				{
					ToggleActive(false);
				}
			}
		}
	}
}
