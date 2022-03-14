// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectile.h"
#include "STeleportProjectile.generated.h"


class UParticleSystemComponent;
/**
 * 
 */
UCLASS()
class ROGUELIKE_API ASTeleportProjectile : public ASProjectile
{
	GENERATED_BODY()
	ASTeleportProjectile();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UParticleSystemComponent* ExplosionEffect;

	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

protected:
	void OnExplodeTimerElapsed();

	void OnTeleportTimerElapsed();

	FTimerHandle OnExplodeTimerHandler;

	FTimerHandle OnTeleportTimerHandler;

private:
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
