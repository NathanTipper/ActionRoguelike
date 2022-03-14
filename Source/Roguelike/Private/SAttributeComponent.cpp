// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"
#include "GameFramework/Controller.h"
#include "SGameModeBase.h"
#include "Net/UnrealNetwork.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("su.DamageMultiplier"), 1.0f, TEXT("Global Damage Modifier for Attribute Component"), ECVF_Cheat);

USAttributeComponent::USAttributeComponent() :
	Health(100.0f),
	MaxHealth(Health),
	CreditWorth(20.f),
	MaxRage(100.f),
	DamageToRagePercentage(0.2f)
{
	SetIsReplicatedByDefault(true);
}

void USAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAttributeComponent, Health);
	DOREPLIFETIME(USAttributeComponent, MaxHealth);
}

void USAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth, float Delta)
{
	OnHealthChanged.Broadcast(InstigatorActor, this,  NewHealth, Delta);
}

bool USAttributeComponent::ApplyHealthChange(AActor* InstigatingActor, float Delta)
{
	if (!GetOwner()->CanBeDamaged() && Delta < 0.0f)
	{
		return false;
	}

	float OldHealth = Health;
	
	if(Delta < 0.f)
	{ 
		Delta *= CVarDamageMultiplier.GetValueOnGameThread();
	}

	float NewHealth = FMath::Clamp(Health + Delta, 0.f, MaxHealth);
	float ActualDelta = Health - OldHealth;

	if (GetOwner()->HasAuthority())
	{
		Health = NewHealth;
		if (ActualDelta != 0.0f)
		{
			MulticastHealthChanged(InstigatingActor, Health, ActualDelta);
		}

		if (ActualDelta < 0.0f && Health <= 0.0f)
		{
			ASGameModeBase* GM = GetWorld()->GetAuthGameMode<ASGameModeBase>();
			if (GM)
			{
				GM->OnActorKilled(GetOwner(), InstigatingActor);
			}
		}
	}

	if (ActualDelta < 0.0f)
	{
		ApplyRageChange(FMath::Floor(FMath::Abs(ActualDelta) * DamageToRagePercentage));
	}

	return ActualDelta != 0;
}

bool USAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool USAttributeComponent::HasFullHealth() const
{
	return Health == MaxHealth;
}

float USAttributeComponent::GetCurrentHealth() const
{
	return Health;
}

bool USAttributeComponent::Kill(AActor* Instigator)
{
	return ApplyHealthChange(Instigator, -MaxHealth);
}

float USAttributeComponent::GetCreditWorth() const
{
	return CreditWorth;
}

float USAttributeComponent::GetRage() const
{
	return Rage;
}

bool USAttributeComponent::ApplyRageChange(float RageDelta)
{
	float OldRage = Rage;
	float NewRage = FMath::Clamp(Rage + RageDelta, 0.f, MaxRage);
	float ActualDelta = Rage - OldRage;

	if(GetOwner()->HasAuthority())
	{ 
		Rage = FMath::Clamp(Rage + RageDelta, 0.f, MaxRage);
	}

	if (ActualDelta != 0.0f)
	{
		OnRageChanged.Broadcast(this, NewRage, ActualDelta);
	}

	return ActualDelta != 0.0f;
}

USAttributeComponent* USAttributeComponent::GetAttributesFromActor(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<USAttributeComponent>(FromActor->GetComponentByClass(USAttributeComponent::StaticClass()));
	}

	return nullptr;
}

USAttributeComponent* USAttributeComponent::GetAttributesFromController(AController* FromController)
{
	if (FromController)
	{
		return GetAttributesFromActor(FromController->GetPawn());
	}

	return nullptr;
}

bool USAttributeComponent::IsActorAlive(AActor* ActorInQuestion)
{
	USAttributeComponent* Attributes = GetAttributesFromActor(ActorInQuestion);

	if (Attributes)
	{
		return Attributes->IsAlive();
	}

	return false;
}
