// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckAtCriticalHealth.h"
#include "SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

void USBTService_CheckAtCriticalHealth::OnSearchStart(FBehaviorTreeSearchData& SearchData)
{
	AAIController* pAIController = SearchData.OwnerComp.GetAIOwner();
	OwnerAttributeComponent = USAttributeComponent::GetAttributesFromController(pAIController);
}

void USBTService_CheckAtCriticalHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (ensure(OwnerAttributeComponent))
	{
		if (OwnerAttributeComponent->GetCurrentHealth() <= CriticalHealthThreshold)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(CriticalHealthThresholdKey.SelectedKeyName, true);
		}
		else 
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(CriticalHealthThresholdKey.SelectedKeyName, false);
		}
	}
}
