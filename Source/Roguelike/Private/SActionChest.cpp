// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionChest.h"
#include "SActionComponent.h"
#include "SAction.h"

ASActionChest::ASActionChest()
{
	ChestLid = CreateDefaultSubobject<UStaticMeshComponent>("ChestLid");
	ChestLid->SetupAttachment(StaticMeshComponent);
}

void ASActionChest::Interact_Implementation(APawn* InstigatorPawn)
{
	Super::Interact_Implementation(InstigatorPawn);
	if (!InstigatorPawn)
	{
		return;
	}

	USActionComponent* ActionComp = Cast<USActionComponent>(InstigatorPawn->GetComponentByClass(USActionComponent::StaticClass()));
	if (ActionComp && ensure(ActionToGrant))
	{
		if(!ActionComp->HasAction(ActionToGrant))
		{
			ActionComp->AddAction(InstigatorPawn, ActionToGrant);
			ToggleActive(false);
		}
	}
}
