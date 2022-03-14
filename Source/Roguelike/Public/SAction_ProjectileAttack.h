// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "SProjectile.h"
#include "SAction_ProjectileAttack.generated.h"

class UParticleSystem;
class UAnimMontage;
class ASProjectile;

/**
 * 
 */
UCLASS()
class ROGUELIKE_API USAction_ProjectileAttack : public USAction
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<ASProjectile> ProjectileClass;

	UPROPERTY(VisibleAnywhere, Category = "Attack")
	FName HandSocketName;
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackAnimDelay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	UParticleSystem* CastingEffect;

	UFUNCTION()
	void AttackDelay_Elapsed(ACharacter* InstigatorCharacter);

public:
	virtual void StartAction_Implementation(AActor* Instigator);

	USAction_ProjectileAttack();
};
