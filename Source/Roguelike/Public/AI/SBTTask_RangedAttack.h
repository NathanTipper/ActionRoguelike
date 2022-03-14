// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SProjectile.h"
#include "SBTTask_RangedAttack.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKE_API USBTTask_RangedAttack : public UBTTaskNode
{
	GENERATED_BODY()
public:
	USBTTask_RangedAttack();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:

	UPROPERTY(EditAnywhere, Category="AI")
	float MaxBulletSpreadHorizontal;

	UPROPERTY(EditAnywhere, Category = "AI")
	float MaxBulletSpreadDown;

	UPROPERTY(EditAnywhere, Category = "AI")
	float MaxBulletSpreadUp;


	UPROPERTY(EditAnywhere, Category="AI")
	TSubclassOf<ASProjectile> ProjectileSpawnClass;
};
