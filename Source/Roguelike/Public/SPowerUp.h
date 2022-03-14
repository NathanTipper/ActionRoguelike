// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGameplayInterface.h"
#include "SPowerUp.generated.h"

class UStaticMeshComponent;
class USphereComponent;

UCLASS(Abstract)
class ROGUELIKE_API ASPowerUp : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()
	
public:	
	ASPowerUp();

	UFUNCTION(BlueprintCallable)
	float GetCreditDelta() const;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float InactiveTime;

	UFUNCTION(BlueprintCallable)
	virtual void ToggleActive(bool On);

	void InactiveTimeElapsed();

	bool bActive;

	UPROPERTY(EditDefaultsOnly)
	float CreditDelta;
};
