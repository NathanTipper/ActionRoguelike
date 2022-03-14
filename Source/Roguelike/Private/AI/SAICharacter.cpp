// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "SAttributeComponent.h"
#include "BrainComponent.h"
#include "UI/SWorldUserWidget.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SActionComponent.h"

ASAICharacter::ASAICharacter() :
	LifeSpanAfterDeath(10.0f),
	HitFlashParameterName("HitTime")
{
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");

	AttributeComponent = CreateDefaultSubobject<USAttributeComponent>("AttributeComponent");

	ActionComponent = CreateDefaultSubobject<USActionComponent>("ActionComponent");

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComponent->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);

	AttributeComponent->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChanged);
}



void ASAICharacter::SetTargetActor(AActor* NewTarget)
{
	AAIController* pAIController = Cast<AAIController>(GetController());

	if (pAIController)
	{
		AActor* CurrentTarget = Cast<AActor>(pAIController->GetBlackboardComponent()->GetValueAsObject("TargetActor"));
		
		if (!ActiveAwareWidget && ensure(AwareWidgetClass))
		{
			ActiveAwareWidget = CreateWidget<USWorldUserWidget>(GetWorld(), AwareWidgetClass);
			ActiveAwareWidget->AttachedActor = this;
		}

		if (ActiveAwareWidget && NewTarget != CurrentTarget)
		{
			if (!ActiveAwareWidget->IsInViewport())
			{
				ActiveAwareWidget->AddToViewport();
			}
		}

		pAIController->GetBlackboardComponent()->SetValueAsObject("TargetActor", NewTarget);
	}
}

void ASAICharacter::OnPawnSeen(APawn* Pawn)
{
	SetTargetActor(Pawn);
}

void ASAICharacter::OnHealthChanged(AActor* InstigatingActor, USAttributeComponent* OwningComponent, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		bool bIsAI = IsValid(Cast<ASAICharacter>(InstigatingActor));
		if (InstigatingActor != this && !bIsAI)
		{
			SetTargetActor(InstigatingActor);
		}
		
		if(!ActiveHealthBar)
		{ 
			ActiveHealthBar = CreateWidget<USWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
			if (ActiveHealthBar)
			{
				ActiveHealthBar->AttachedActor = this;
				ActiveHealthBar->AddToViewport();
			}
		}

		GetMesh()->SetScalarParameterValueOnMaterials(HitFlashParameterName, GetWorld()->TimeSeconds);

		if (NewHealth <= 0.f)
		{
			AAIController* pController = Cast<AAIController>(GetController());
			if (pController)
			{
				pController->GetBrainComponent()->StopLogic("Killed");
			}

			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");

			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();

			SetLifeSpan(LifeSpanAfterDeath);
		}
	}
}

