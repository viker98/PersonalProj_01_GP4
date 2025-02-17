// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonGenerator.h"
#include "Kismet/GameplayStatics.h" 
#include "Kismet/KismetArrayLibrary.h"
#include "Math/RandomStream.h"
#include "Components/BoxComponent.h"

// Sets default values
ADungeonGenerator::ADungeonGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADungeonGenerator::BeginPlay()
{
	Super::BeginPlay();
	SetSeed();
	SpawnStartRoom();
	SpawnNextRoom();

}

void ADungeonGenerator::SpawnStartRoom()
{
	FActorSpawnParameters SpawnParams;
	AActor* Dungeon_Room = GetWorld()->SpawnActor<AMasterRoom>(firstRoom, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	AMasterRoom* roomRef = Cast<AMasterRoom>(Dungeon_Room);
	ExitsArray = roomRef->GetExitList();
}

void ADungeonGenerator::SpawnNextRoom()
{

	USceneComponent* chosenExit;
	
	int exitNum = RandomStream.RandRange(0, ExitsArray.Num() - 1);
	
	chosenExit = ExitsArray[exitNum];
	
	

	ExitsArray.RemoveAt(exitNum);

	if(RoomAmount % 10 == 0)
	{
		UE_LOG(LogTemp,Warning, TEXT("Making Special Room"))

		FActorSpawnParameters SpawnParams;
		AActor* nextRoom = GetWorld()->SpawnActor<AMasterRoom>(
			SpecialRoomList[RandomStream.RandRange(0, SpecialRoomList.Num() - 1)],
			chosenExit->GetComponentLocation(),
			chosenExit->GetComponentRotation(),
			SpawnParams);
		LatestRoom = Cast<AMasterRoom>(nextRoom);

	}
	else
	{
		FActorSpawnParameters SpawnParams;
		AActor* nextRoom = GetWorld()->SpawnActor<AMasterRoom>(
			RoomList[RandomStream.RandRange(0, RoomList.Num() - 1)],
			chosenExit->GetComponentLocation(),
			chosenExit->GetComponentRotation(),
			SpawnParams);
		LatestRoom = Cast<AMasterRoom>(nextRoom);
		AddFloorSpawnsToList();

	}
	FTimerHandle delayTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(delayTimerHandle, this, &ADungeonGenerator::CheckForOverlap, delayTimer , false);
	//CheckForOverlap();

}

void ADungeonGenerator::AddOverlappingRoomsToList()
{
	USceneComponent* boxColliderFolder = LatestRoom->FindComponentByTag<USceneComponent>(FName("OverlapList"));
	TArray<USceneComponent*> colliderArray;
	boxColliderFolder->GetChildrenComponents(true, colliderArray);

	TArray<UPrimitiveComponent*> ArrayOverlap;

	for (int i = 0; i < colliderArray.Num(); ++i)
	{
		Cast<UBoxComponent>(colliderArray[i])->GetOverlappingComponents(ArrayOverlap);

		OverlappedList.Append(ArrayOverlap);
	}
}

void ADungeonGenerator::CheckForOverlap()
{
	AddOverlappingRoomsToList();
	// does not have a overlap 
	if (OverlappedList.IsEmpty()) 
	{
		OverlappedList.Empty();
		RoomAmount--;
		ExitsArray.Append(LatestRoom->GetExitList());
		if (RoomAmount <= 0) 
		{

			UE_LOG(LogTemp, Warning, TEXT("Dungeon Done")); 
			SpawnCoinAtLocations();
			return;
		}
		SpawnNextRoom();
		return;
		
	}

	// has a overlap
	else
	{
		OverlappedList.Empty();
		LatestRoom->Destroy();
		if (RoomAmount <= 0) 
		{

			UE_LOG(LogTemp, Warning, TEXT("DungeonDone"));
			SpawnCoinAtLocations();
			return;
		}
		SpawnNextRoom();
	}

}

void ADungeonGenerator::SetSeed()
{
	if (Seed == -1)
	{
		RandomStream.Initialize(RandomStream.RandRange(0, 100));
	}
	else
	{
		RandomStream = Seed;
	}
}

void ADungeonGenerator::AddFloorSpawnsToList()
{
	USceneComponent* spawnPointFolder = LatestRoom->FindComponentByTag<USceneComponent>(FName("SpawnPointList"));
	TArray<USceneComponent*> spawnPoints;
	spawnPointFolder->GetChildrenComponents(true, spawnPoints);

	SpawnPointList.Append(spawnPoints);
}

void ADungeonGenerator::SpawnCoinAtLocations()
{
	if(AmountOfCoins >= 1)
	{
		if(SpawnPointList.IsEmpty())
		{
			return;
		}
		USceneComponent* SelectedFloorSpawnLocation;

		SelectedFloorSpawnLocation = (SpawnPointList[RandomStream.RandRange(0,SpawnPointList.Num() - 1)]);
	
		FActorSpawnParameters SpawnParams;
		AActor* nextRoom = GetWorld()->SpawnActor<ACoinPickup>(
			coinRef,
			SelectedFloorSpawnLocation->GetComponentLocation(),
			SelectedFloorSpawnLocation->GetComponentRotation(),
			SpawnParams);

		SpawnPointList.Remove(SelectedFloorSpawnLocation);
		AmountOfCoins--;
		SpawnCoinAtLocations();
	}
}

// Called every frame
void ADungeonGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

