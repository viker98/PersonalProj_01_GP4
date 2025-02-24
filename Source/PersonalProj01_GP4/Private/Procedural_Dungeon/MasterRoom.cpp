// Fill out your copyright notice in the Description page of Project Settings.


#include "Procedural_Dungeon/MasterRoom.h"

// Sets default values
AMasterRoom::AMasterRoom()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	Exits = CreateDefaultSubobject<USceneComponent>(TEXT("ExitList"));
	Exits->SetupAttachment(RootSceneComponent);

    FloorSpawns = CreateDefaultSubobject<USceneComponent>(TEXT("FloorSpawnList"));
	FloorSpawns->SetupAttachment(RootSceneComponent);

	Geometry = CreateDefaultSubobject<USceneComponent>(TEXT("GeometryFolder"));
	Geometry->SetupAttachment(RootSceneComponent);

	boxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	boxCollider->SetupAttachment(RootSceneComponent);
}

TArray<USceneComponent*> AMasterRoom::GetExitList()
{
	return ExitsList;
}

USceneComponent* AMasterRoom::GetBoxCollider()
{
	return boxCollider;
}


USceneComponent* AMasterRoom::GetFloorSpawnsFolder()
{
	return FloorSpawns;
}

// Called when the game starts or when spawned
void AMasterRoom::BeginPlay()
{
	Super::BeginPlay();
	Exits->GetChildrenComponents(false,ExitsList);
}

// Called every frame
void AMasterRoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

