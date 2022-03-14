// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "SBTService_CheckAtCriticalHealth.generated.h"

class USAttributeComponent;

/**
 * 
 */
UCLASS()
class ROGUELIKE_API USBTService_CheckAtCriticalHealth : public UBTService
{
	GENERATED_BODY()

protected:
	virtual void OnSearchStart(FBehaviorTreeSearchData& SearchData) override;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	USAttributeComponent* OwnerAttributeComponent; 

	UPROPERTY(EditAnywhere)
	float CriticalHealthThreshold;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector CriticalHealthThresholdKey;
};
