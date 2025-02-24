// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "MasterRoom.generated.h"

UCLASS()
class AMasterRoom : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMasterRoom();
	
	TArray<USceneComponent*> GetExitList();
	USceneComponent* GetBoxCollider();

	USceneComponent* GetFloorSpawnsFolder();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


private:
	UPROPERTY(VisibleAnywhere, Category = "Dungeon")
	USceneComponent* Exits;

	UPROPERTY(VisibleAnywhere, Category = "Dungeon")
	TArray<USceneComponent*> ExitsList;

	UPROPERTY(VisibleAnywhere, Category = "Dungeon")
	UBoxComponent* boxCollider;

	UPROPERTY(VisibleAnywhere, Category = "Dungeon")
	TArray<USceneComponent*> ExitCollidersList;

	UPROPERTY(VisibleAnywhere, Category = "Dungeon")
	USceneComponent* FloorSpawns;

	UPROPERTY(VisibleAnywhere, Category = "Dungeon")
	USceneComponent* Geometry;

	UPROPERTY(VisibleAnywhere, Category = "Dungeon")
	USceneComponent* RootSceneComponent;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
