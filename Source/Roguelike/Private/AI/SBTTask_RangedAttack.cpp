// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_RangedAttack.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SAttributeComponent.h"

USBTTask_RangedAttack::USBTTask_RangedAttack() :
	MaxBulletSpreadHorizontal(5.0f),
	MaxBulletSpreadDown(1.0f),
	MaxBulletSpreadUp(5.0f)
{
	
}

EBTNodeResult::Type USBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* pController = OwnerComp.GetAIOwner();
	if (ensure(pController))
	{
		ACharacter* pAIPawn = Cast<ACharacter>(pController->GetPawn());
		if (!pAIPawn)
		{
			return EBTNodeResult::Failed;
		}

		FVector MuzzleLocation = pAIPawn->GetMesh()->GetSocketLocation("Muzzle_01");
		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));

		if (!TargetActor)
		{
			return EBTNodeResult::Failed;
		}

		if (!USAttributeComponent::IsActorAlive(TargetActor))
		{
			return EBTNodeResult::Failed;
		}

		FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
		FRotator MuzzleRotation = Direction.Rotation();

		MuzzleRotation.Pitch += FMath::RandRange(MaxBulletSpreadDown, MaxBulletSpreadUp);
		MuzzleRotation.Yaw += FMath::RandRange(-MaxBulletSpreadHorizontal, MaxBulletSpreadHorizontal);

		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Params.Instigator = pAIPawn;

		AActor* NewProj = GetWorld()->SpawnActor<AActor>(ProjectileSpawnClass, MuzzleLocation, MuzzleRotation, Params);

		return NewProj ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}

	return EBTNodeResult::Failed;
}
