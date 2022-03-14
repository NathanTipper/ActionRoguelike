// Fill out your copyright notice in the Description page of Project Settings.


#include "STeleportProjectile.h"
#include "GameFramework/Actor.h"
#include "Particles/ParticleSystemComponent.h"
#include "SProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

ASTeleportProjectile::ASTeleportProjectile()
{
	ExplosionEffect = CreateDefaultSubobject<UParticleSystemComponent>("ExplosionEffect");
	ExplosionEffect->bAutoActivate = false;

	MovementComponent->InitialSpeed = 6000.f;
}

void ASTeleportProjectile::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(OnExplodeTimerHandler, this, &ASTeleportProjectile::OnExplodeTimerElapsed, 0.2f);
}

void ASTeleportProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASTeleportProjectile::OnOverlap);
}

void ASTeleportProjectile::OnExplodeTimerElapsed()
{
	ExplosionEffect->Activate();
	EffectComponent->Deactivate();
	MovementComponent->StopMovementImmediately();

	GetWorldTimerManager().SetTimer(OnTeleportTimerHandler, this, &ASTeleportProjectile::OnTeleportTimerElapsed, 0.2f);
}

void ASTeleportProjectile::OnTeleportTimerElapsed()
{
	FVector ForwardVector = GetActorForwardVector();
	GetInstigator()->TeleportTo(GetActorLocation(), GetInstigator()->GetActorRotation());

	Destroy();
}

void ASTeleportProjectile::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GetWorldTimerManager().ClearTimer(OnExplodeTimerHandler);
	GetWorldTimerManager().ClearTimer(OnTeleportTimerHandler);

	EffectComponent->Deactivate();
	ExplosionEffect->Activate();
	MovementComponent->StopMovementImmediately();

	GetWorldTimerManager().SetTimer(OnTeleportTimerHandler, this, &ASTeleportProjectile::OnTeleportTimerElapsed, 0.2f);
}
