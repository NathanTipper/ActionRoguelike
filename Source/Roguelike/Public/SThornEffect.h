// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SActionEffect.h"
#include "SThornEffect.generated.h"

class USAttributeComponent;

UCLASS()
class ROGUELIKE_API USThornEffect : public USActionEffect
{
	GENERATED_BODY()
public:
	USThornEffect();


	void StartAction_Implementation(AActor* Instigator) override;

	void StopAction_Implementation(AActor* Instigator) override;

protected:

	UPROPERTY(EditDefaultsOnly, Category="Effect", meta=(ClampMin=0.0f, ClampMax=1.0f))
	float PercentageReturned;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatingActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);
};
