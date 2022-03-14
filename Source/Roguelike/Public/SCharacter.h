// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SProjectile.h"
#include "SCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USInteractionComponent;
class UAnimMontage;
class USAttributeComponent;
class USActionComponent;

UCLASS()
class ROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category="Attack")
	TSubclassOf<ASProjectile> PrimaryAttackProjectileClass;

	UPROPERTY(EditAnywhere, Category="Attack")
	TSubclassOf<ASProjectile> SecondaryAttackProjectileClass;

	UPROPERTY(EditAnywhere, Category="Movement")
	TSubclassOf<ASProjectile> MovementProjectileClass;

	UPROPERTY(EditAnywhere, Category="Attack")
	UAnimMontage* AttackAnim;

	UPROPERTY(VisibleAnywhere, Category="Attack")
	FName HandSocketName;

	UPROPERTY(VisibleAnywhere, Category="OnHit")
	FName HitFlashParameterName;

	UPROPERTY(EditAnywhere, Category="Attack")
	float AttackDelay;

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	virtual void PostInitializeComponents() override;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere)
	USInteractionComponent* InteractionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USAttributeComponent* AttributeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USActionComponent* ActionComponent;


	void MoveForward(float Value);

	void MoveRight(float Value);

	void SprintStart();

	void SprintStop();

	void PrimaryAttack();

	void PrimaryInteract();

	void SecondaryAttack();

	void UseTeleport();

	void Parry();

	UFUNCTION()
	virtual void OnHealthChanged(AActor* InstigatingActor, USAttributeComponent* OwningComponent, float NewHealth, float Delta);

	FVector	GetPawnViewLocation() const override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UFUNCTION(Exec)
	void HealSelf(float Amount = 100);
};
