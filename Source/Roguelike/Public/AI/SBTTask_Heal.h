// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SBTTask_Heal.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKE_API USBTTask_Heal : public UBTTaskNode
{
	GENERATED_BODY()
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	public:
		USBTTask_Heal();


	protected:
		UPROPERTY(EditAnywhere)
		float HealPerInterval;

		UPROPERTY(EditAnywhere)
		float HealingInterval;

		UPROPERTY(EditAnywhere)
		float InitialDelay;

		FTimerHandle LoopHandle;
};
