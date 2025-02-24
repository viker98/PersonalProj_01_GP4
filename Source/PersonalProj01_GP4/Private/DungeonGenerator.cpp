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
	totalRooms = RoomAmount;
}


void ADungeonGenerator::StartGenerator(int roomNumber, int seed, int CoinAmount)
{
	UE_LOG(LogTemp, Warning, TEXT("GHES"));
	RoomAmount = roomNumber;
	Seed = seed;
	AmountOfCoins = CoinAmount;



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
	roomRef->GetBoxCollider()->GetChildrenComponents(false,OverlappedList);
	FTimerHandle delayTimerHandle;
}

void ADungeonGenerator::SpawnNextRoom()
{

	USceneComponent* chosenExit;
	
	int exitNum = RandomStream.RandRange(0, ExitsArray.Num() - 1);
	
	chosenExit = ExitsArray[exitNum];


	if(CheckingForOverlapWithNewRoom(chosenExit))
	{
		CheckForOverlap();
		return;
	}



	if(RoomAmount % 10 == 0 && RoomAmount != totalRooms)
	{
		UE_LOG(LogTemp, Warning, TEXT("Making Special Room"))
		FActorSpawnParameters SpawnParams;
		AMasterRoom* nextRoom = GetWorld()->SpawnActor<AMasterRoom>(
			SpecialRoomList[RandomStream.RandRange(0, SpecialRoomList.Num() - 1)],
			chosenExit->GetComponentLocation(),
			chosenExit->GetComponentRotation(),
			SpawnParams);
		LatestRoom = Cast<AMasterRoom>(nextRoom);
		--RoomAmount;
		ExitsArray.Append(LatestRoom->GetExitList());
		OverlappedList.Add(LatestRoom->GetBoxCollider());
		ExitsArray.RemoveAt(exitNum);
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
		--RoomAmount;
		AddFloorSpawnsToList();
		ExitsArray.Append(LatestRoom->GetExitList());
		OverlappedList.Add(LatestRoom->GetBoxCollider());
		ExitsArray.RemoveAt(exitNum);

	}
	FTimerHandle delayTimerHandle;
	GetWorldTimerManager().SetTimer(delayTimerHandle, this, &ADungeonGenerator::CheckForOverlap, delayTimer, false);
	//CheckForOverlap();

}
void ADungeonGenerator::CloseOffExits()
{
	for(USceneComponent* exit : ExitsArray)
	{
		USceneComponent* exitWall = exit->GetChildComponent(0)->GetChildComponent(0);
		UE_LOG(LogTemp, Warning, TEXT("This is exit wall: %s"), *exitWall->GetName());
		Cast<UStaticMeshComponent>(exitWall)->SetVisibility(true);
	}
}
bool ADungeonGenerator::CheckingForOverlapWithNewRoom(USceneComponent* chosenExit)
{
	USceneComponent* exitCollider = chosenExit->GetChildComponent(0);

	TArray<AActor*> OverlappedItems;

	UBoxComponent* exitboxCollider = Cast<UBoxComponent>(exitCollider);

	exitboxCollider->GetOverlappingActors(OverlappedItems);

		for (AActor* actors : OverlappedItems)
		{
			if (actors)
			{
				UE_LOG(LogTemp, Warning, TEXT("Overlapping with: %s"), *actors->GetName());
			}
		}

	if (OverlappedItems.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("This mean no overlap"))
			return false;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("This means overlap"))
			return true;
	}

}
void ADungeonGenerator::CheckForOverlap()
{

	if(RoomAmount <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Dungeon Done"));
		SpawnFinalRooms();
		CloseOffExits();
		SpawnCoinAtLocations();
		return;
	}
	else
	{
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
	USceneComponent* spawnPointFolder = LatestRoom->GetFloorSpawnsFolder();
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

void ADungeonGenerator::SpawnFinalRooms()
{

	for(TSubclassOf<AMasterRoom> roomToSpawn : FinalRoomList)
	{
		USceneComponent* chosenExit;

		int exitNum = RandomStream.RandRange(0, ExitsArray.Num() - 1);

		chosenExit = ExitsArray[exitNum];

		if (CheckingForOverlapWithNewRoom(chosenExit))
		{
			SpawnFinalRooms();
			return;
		}

		FActorSpawnParameters SpawnParams;
		AActor* nextRoom = GetWorld()->SpawnActor<AMasterRoom>(
			roomToSpawn,
			chosenExit->GetComponentLocation(),
			chosenExit->GetComponentRotation(),
			SpawnParams);

		FinalRoomList.Remove(roomToSpawn);
		ExitsArray.RemoveAt(exitNum);
	}
}

// Called every frame
void ADungeonGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

