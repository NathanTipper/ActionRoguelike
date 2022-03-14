// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPowerUp.h"
#include "GameplayTagContainer.h"
#include "SActionChest.generated.h"

class USAction;


UCLASS()
class ROGUELIKE_API ASActionChest : public ASPowerUp
{
	GENERATED_BODY()
public:
	ASActionChest();

	void Interact_Implementation(APawn* InstigatorPawn) override;

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USAction> ActionToGrant;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* ChestLid;

};
