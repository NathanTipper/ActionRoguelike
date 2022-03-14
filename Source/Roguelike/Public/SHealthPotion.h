// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPowerUp.h"
#include "SHealthPotion.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKE_API ASHealthPotion : public ASPowerUp
{
	GENERATED_BODY()
	
	ASHealthPotion();

public:
	void Interact_Implementation(APawn* InstigatorPawn) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float HealthGain;

};
