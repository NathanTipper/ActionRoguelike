// Fill out your copyright notice in the Description page of Project Settings.

#include "SPowerUp.h"
#include "SAttributeComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"

ASPowerUp::ASPowerUp() : 
	InactiveTime(10.0f),
	bActive(true),
	CreditDelta(0.0f)
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = StaticMeshComponent;

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	//SphereComponent->SetCollisionProfileName("Powerup");
	SphereComponent->SetupAttachment(StaticMeshComponent);

	SetReplicates(true);
}

float ASPowerUp::GetCreditDelta() const
{
	return CreditDelta;
}

void ASPowerUp::ToggleActive(bool On)
{
	StaticMeshComponent->SetVisibility(On, true);
	SetActorEnableCollision(On);
	bActive = On;

	if (!On)
	{
		FTimerHandle InactiveHandle;
		GetWorldTimerManager().SetTimer(InactiveHandle, this, &ASPowerUp::InactiveTimeElapsed, InactiveTime);
	}
}

void ASPowerUp::InactiveTimeElapsed()
{
	ToggleActive(true);
}