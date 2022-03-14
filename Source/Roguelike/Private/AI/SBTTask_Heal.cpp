// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_Heal.h"
#include "AIController.h"
#include "SAttributeComponent.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "GameFramework/Actor.h"

USBTTask_Heal::USBTTask_Heal() : 
	HealPerInterval(5.0f),
	HealingInterval(1.0f),
	InitialDelay(1.0f)
{
	
}

EBTNodeResult::Type USBTTask_Heal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* pAIController = OwnerComp.GetAIOwner();
	if (pAIController)
	{
		APawn* pAIPawn = pAIController->GetPawn();
		if (pAIPawn)
		{
			USAttributeComponent* Attributes = USAttributeComponent::GetAttributesFromActor(pAIPawn);
			if(ensure(Attributes) && Attributes->ApplyHealthChange(pAIPawn, HealPerInterval))
			{
				return EBTNodeResult::Succeeded;
			}
		}
	}

	return EBTNodeResult::Failed;
}
