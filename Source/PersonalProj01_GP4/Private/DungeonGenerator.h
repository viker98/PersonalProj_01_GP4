// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Procedural_Dungeon/MasterRoom.h"
#include "Components/StaticMeshComponent.h"
#include "CoinPickup.h"
#include "DungeonGenerator.generated.h"

UCLASS()
class ADungeonGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADungeonGenerator();

	UFUNCTION(BlueprintCallable)
	void StartGenerator(int roomNumber, int seed, int CoinAmount);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AMasterRoom> firstRoom;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AMasterRoom>> RoomList;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AMasterRoom>> SpecialRoomList;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AMasterRoom>> FinalRoomList;

	UPROPERTY(EditAnywhere)
	int RoomAmount;

	int totalRooms;

	UPROPERTY(EditAnywhere)
	float delayTimer;

	UPROPERTY(EditAnywhere)
	int Seed;

	UPROPERTY(EditAnywhere)
	int AmountOfCoins;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ACoinPickup> coinRef;

	
	UPROPERTY(VisibleAnywhere)
	TArray<USceneComponent*> ExitsArray;

	UPROPERTY(VisibleAnywhere)
	AMasterRoom* LatestRoom;

	TArray<USceneComponent*> OverlappedList;

	UPROPERTY(VisibleAnywhere)
	FRandomStream RandomStream;

	UPROPERTY()
	TArray<USceneComponent*> SpawnPointList;


	UFUNCTION()
	void SpawnStartRoom();
	
	UFUNCTION()
	void SpawnNextRoom();

	UFUNCTION()
	void CloseOffExits();

	UFUNCTION()
	bool CheckingForOverlapWithNewRoom(USceneComponent* chosenExit);

	UFUNCTION()
	void CheckForOverlap();
	UFUNCTION()
	void SetSeed();

	UFUNCTION()
	void AddFloorSpawnsToList();

	UFUNCTION()
	void SpawnCoinAtLocations();

	UFUNCTION()
	void SpawnFinalRooms();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
