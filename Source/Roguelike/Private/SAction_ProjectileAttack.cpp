// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction_ProjectileAttack.h"
#include "GameFramework/Character.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

static TAutoConsoleVariable<bool> CVarDebugDrawAttackImpactPoint(TEXT("su.DebugDrawAttackImpactPoint"), false, TEXT("Draw debug line and sphere for hit point and direction of attack"), ECVF_Cheat);
static TAutoConsoleVariable<bool> CVarDebugDrawAttackRotationVector(TEXT("su.DebugDrawAttackRotationVector"), false, TEXT("Draw a line from the muzzle location to the calculated hit point"), ECVF_Cheat);

USAction_ProjectileAttack::USAction_ProjectileAttack() :
	HandSocketName("Muzzle_01"),
	AttackAnimDelay(0.2f)
{

}

void USAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	ACharacter* Character = Cast<ACharacter>(Instigator);
	if (Character)
	{
		Character->PlayAnimMontage(AttackAnim);

		UGameplayStatics::SpawnEmitterAttached(CastingEffect, Character->GetMesh(), HandSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);

		if(Character->HasAuthority())
		{ 
			FTimerHandle TimerHandle_AttackDelay;
			FTimerDelegate Delegate;

			Delegate.BindUFunction(this, "AttackDelay_Elapsed", Character);
			GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate, AttackAnimDelay, false);
		}
	}
}

void USAction_ProjectileAttack::AttackDelay_Elapsed(ACharacter* InstigatorCharacter)
{
	if(ensure(ProjectileClass))
	{ 
		FVector HandLocation = InstigatorCharacter->GetMesh()->GetSocketLocation(HandSocketName);
		FHitResult Hit;
		FVector Start = InstigatorCharacter->GetPawnViewLocation() + FVector(5.0f, 0.f, 0.f);
		FVector End = Start + (InstigatorCharacter->GetControlRotation().Vector() * 100000.0f);

		FCollisionShape Shape;
		Shape.SetSphere(20.0f);

		FCollisionObjectQueryParams QueryParams;
		QueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		QueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
		QueryParams.AddObjectTypesToQuery(ECC_Pawn);
		QueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);


		bool bHit = GetWorld()->SweepSingleByObjectType(Hit, Start, End, FQuat::Identity, QueryParams, Shape);

		FVector LookVector = bHit ? Hit.ImpactPoint - HandLocation : End - HandLocation;
		LookVector.Normalize();
		if (CVarDebugDrawAttackImpactPoint.GetValueOnGameThread())
		{
			DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 3.0f, 0, 2.0f);
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 5.0f, 26, FColor::Green, false, 5.0f, 0, 2.0f);
		}

		FRotator LookAtRotation = LookVector.Rotation();
		if (CVarDebugDrawAttackRotationVector.GetValueOnGameThread())
		{
			DrawDebugLine(GetWorld(), HandLocation, HandLocation + (LookVector * 1000.0f), FColor::Yellow, false, 3.0f, 0, 2.0f);
		}

		FTransform SpawnTM = FTransform(LookAtRotation, HandLocation + (LookVector * 10.0f));

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = InstigatorCharacter;

		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
	}

	StopAction(InstigatorCharacter);
}

