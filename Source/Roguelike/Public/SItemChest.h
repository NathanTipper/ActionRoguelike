// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGameplayInterface.h"
#include "SItemChest.generated.h"

UCLASS()
class ROGUELIKE_API ASItemChest : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()
	
public:	
	ASItemChest();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* LidMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TargetPitch;

	UPROPERTY(ReplicatedUsing="OnRep_LidOpened", SaveGame);
	bool bLidOpen;

	UFUNCTION()
	void OnRep_LidOpened();

public:
	void Interact_Implementation(APawn* InstigatorPawn) override;

	void OnActorLoaded_Implementation() override;

};
