// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "SPowerUp.h"
#include "SGameModeBase.generated.h"

class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class UCurveFloat;
class ASPowerUp;
class USSaveGame;

/**
 * 
 */
UCLASS(Abstract)
class ROGUELIKE_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	public:
		ASGameModeBase();

		void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

		virtual void StartPlay() override;

		virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);

		UFUNCTION(Exec)
		void KillAllAI();

		UFUNCTION(BlueprintCallable, Category="Save Game")
		void WriteSaveGame();

		void LoadSaveGame();

	protected:
		UPROPERTY(EditDefaultsOnly, Category="AI")
		UEnvQuery* SpawnBotQuery;

		UPROPERTY(EditDefaultsOnly, Category = "Powerups")
		UEnvQuery* SpawnPowerupsQuery;

		UPROPERTY(EditDefaultsOnly, Category="AI")
		TSubclassOf<AActor> MinionClass;

		UPROPERTY(EditDefaultsOnly, Category="AI")
		UCurveFloat* DifficultyCurve;

		UPROPERTY(EditDefaultsOnly, Category = "Powerups")
		TArray<TSubclassOf<ASPowerUp>> PowerupClasses;

		UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		bool bDisableSpawning;

		UPROPERTY(EditDefaultsOnly, Category = "Respawning")
		float RespawnDelay;

		UFUNCTION()
		void SpawnBotTimerElapsed();

		UFUNCTION()
		void OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

		UFUNCTION()
		void RespawnPlayerElapsed(AController* Controller);

		UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		float SpawnInterval;

		UPROPERTY(EditDefaultsOnly, Category = "Powerups")
		int32 PowerupsToSpawn;

		UPROPERTY(EditDefaultsOnly, Category="Powerups")
		FVector PowerupSpawnOffset;

		void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

		UPROPERTY()
		USSaveGame* CurrentSaveGame;

		FString SlotName;

	private:
		UFUNCTION()
		void SpawnPowerups();

		UFUNCTION()
		void OnSpawnPowerupQueryComplete(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

		int32 PowerupsSpawned;
};
