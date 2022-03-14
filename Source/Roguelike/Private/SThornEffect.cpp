// Fill out your copyright notice in the Description page of Project Settings.


#include "SThornEffect.h"
#include "SAction.h"
#include "SActionComponent.h"
#include "SAttributeComponent.h"

USThornEffect::USThornEffect() :
	PercentageReturned(0.1)
{
	Duration = 0.0f;
	Period = 0.0f;
}

void USThornEffect::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	AActor* Owner = GetOwningComponent()->GetOwner();
	if (ensure(Owner))
	{
		USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributesFromActor(Owner);
		if (ensure(AttributeComponent))
		{
			AttributeComponent->OnHealthChanged.AddDynamic(this, &USThornEffect::OnHealthChanged);
		}
	}

	GrantsTags.AddTag(FGameplayTag::RequestGameplayTag("Status.Thorns"));
}

void USThornEffect::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);

	AActor* Owner = GetOwningComponent()->GetOwner();
	if (ensure(Owner))
	{
		USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributesFromActor(Owner);
		if (ensure(AttributeComponent))
		{
			AttributeComponent->OnHealthChanged.RemoveDynamic(this, &USThornEffect::OnHealthChanged);
		}
	}
}

void USThornEffect::OnHealthChanged(AActor* InstigatingActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (InstigatingActor)
	{
		USAttributeComponent* InstigatingComp = USAttributeComponent::GetAttributesFromActor(InstigatingActor);
		if (InstigatingComp && Delta < 0.0f)
		{
			InstigatingComp->ApplyHealthChange(GetOwningComponent()->GetOwner(), FMath::CeilToFloat(Delta * PercentageReturned));
		}
	}
}
