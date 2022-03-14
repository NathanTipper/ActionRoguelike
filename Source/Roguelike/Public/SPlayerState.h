// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCreditsChanged, float, NewCreditTotal, float, Delta);

class USSaveGame;

/**
 * 
 */
UCLASS()
class ROGUELIKE_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	ASPlayerState();

	UFUNCTION(BlueprintCallable, Category="Credits")
	bool ApplyCredits(float Amount);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetPlayerCredits() const;

	UPROPERTY(BlueprintAssignable)
	FOnCreditsChanged OnCreditsChanged;

	UFUNCTION(BlueprintNativeEvent)
	void SavePlayerState(USSaveGame* SaveObject);

	UFUNCTION(BlueprintNativeEvent)
	void LoadPlayerState(USSaveGame* SaveObject);

protected:
	
	float PlayerCredits; 
};
