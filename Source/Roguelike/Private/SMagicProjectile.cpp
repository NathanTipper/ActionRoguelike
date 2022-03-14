// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"
#include "SAttributeComponent.h"
#include "SProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "SGameplayFunctionLibrary.h"
#include "SActionComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "SActionEffect.h"

ASMagicProjectile::ASMagicProjectile()
{
	AudioComponent = CreateDefaultSubobject<UAudioComponent>("AudioComponent");
	AudioComponent->SetupAttachment(SphereComponent);
}

void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		USActionComponent* ActionComp = Cast<USActionComponent>(OtherActor->GetComponentByClass(USActionComponent::StaticClass()));
		if (ActionComp && ActionComp->ActiveGameplayTags.HasTag(ParryTag))
		{
			MovementComponent->Velocity = -MovementComponent->Velocity;

			SetInstigator(Cast<APawn>(OtherActor));
			return;
		}

		if (USGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, Damage, SweepResult))
		{
			CreateExplosionEffect();
			UGameplayStatics::PlayWorldCameraShake(this, CameraShake, GetActorLocation(), InnerRadius, OuterRadius);
			if (ActionComp && EffectClass && HasAuthority())
			{
				ActionComp->AddAction(GetInstigator(), EffectClass);
			}

			if(!IsPendingKill())
			{ 
				Destroy();
			}
		}
	}
}

void ASMagicProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && GetInstigator() != OtherActor)
	{
		CreateExplosionEffect();

		UGameplayStatics::PlayWorldCameraShake(this, CameraShake, GetActorLocation(), InnerRadius, OuterRadius);

		if(!IsPendingKill())
		{ 
			Destroy();
		}
	}
}

void ASMagicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnActorOverlap);
	SphereComponent->OnComponentHit.AddDynamic(this, &ASMagicProjectile::OnHit);
}

void ASMagicProjectile::CreateExplosionEffect()
{
	if(ExplodeParticleEffect)
	{ 
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplodeParticleEffect, GetActorLocation(), GetActorRotation());
	}
	if (ExplosionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());
	}
}
