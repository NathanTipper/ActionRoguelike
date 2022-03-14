// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"
#include "Components/ActorComponent.h"
#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "../Roguelike.h"
#include "Net/UnrealNetwork.h"

USAction::USAction() :
	RageCost(0.0f),
	RepData({false, nullptr})
{

}

void USAction::Initialize(USActionComponent* OwningComponent)
{
	OwningActionComponent = OwningComponent;
}

bool USAction::CanStart_Implementation(AActor* Instigator) const
{
	
	USActionComponent* Comp = GetOwningComponent();

	if(Comp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}

	USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(Comp->GetOwner()->GetComponentByClass(USAttributeComponent::StaticClass()));

	if (AttributeComp)
	{
		if (AttributeComp->GetRage() < RageCost)
		{
			return false;
		}
		else
		{
			AttributeComp->ApplyRageChange(-RageCost);
		}
	}

	return true;
}

void USAction::StartAction_Implementation(AActor* Instigator)
{
	if (IsRunning())
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));
	//LogOnScreen(this, FString::Printf(TEXT("Started: %s"), *ActionName.ToString()), FColor::Green);

	USActionComponent* Comp = GetOwningComponent();

	Comp->ActiveGameplayTags.AppendTags(GrantsTags);

	RepData.bIsRunning = true;
	RepData.Instigator = Instigator;
}

void USAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));
	//LogOnScreen(this, FString::Printf(TEXT("Stopped: %s"), *ActionName.ToString()), FColor::Red);


	USActionComponent* Comp = GetOwningComponent();

	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);

	RepData.bIsRunning = false;
	RepData.Instigator = Instigator;
}

UWorld* USAction::GetWorld() const
{
	AActor* Actor = Cast<AActor>(GetOuter());
	if (Actor)
	{
		return Actor->GetWorld();
	}

	return nullptr;
}

USActionComponent* USAction::GetOwningComponent() const
{
	return OwningActionComponent;
}

void USAction::OnRep_RepData()
{
	if (RepData.bIsRunning)
	{
		StartAction(RepData.Instigator);
	}
	else
	{
		StopAction(RepData.Instigator);
	}
}

bool USAction::IsRunning() const
{
	return RepData.bIsRunning;
}

void USAction::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAction, RepData);
	DOREPLIFETIME(USAction, OwningActionComponent);
}

