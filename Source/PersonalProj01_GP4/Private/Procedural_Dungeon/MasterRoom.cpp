// Fill out your copyright notice in the Description page of Project Settings.


#include "Procedural_Dungeon/MasterRoom.h"

// Sets default values
AMasterRoom::AMasterRoom()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

TArray<USceneComponent*> AMasterRoom::GetExitList()
{
	return ExitsList;
}

// Called when the game starts or when spawned
void AMasterRoom::BeginPlay()
{
	Super::BeginPlay();
	exits = FindComponentByTag<USceneComponent>(FName("ExitList"));
	exits->GetChildrenComponents(true,ExitsList);
	
}

// Called every frame
void AMasterRoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

