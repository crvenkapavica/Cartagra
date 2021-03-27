// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CartagraGameModeBase.generated.h"


/**
 * Basically this class generated a procedural map; it is not finished it is a work in project prototype
 * It would be implemented as deciding whether to go random or whether to use pre-generated segments; or a combination of both
 * Segments can also be generated using the algorithm and stored
 * Then the Basic tiling path can be replaced by either open world segments or ofcourse any kinds of assets
 */
UCLASS()
class CARTAGRA_API ACartagraGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

private:
	/////////////////////////
	//Map Generation Privates

	uint8 Map[333][333] = { 0 };

	//Store Position X and Y
	TArray<int32> PositionX;
	TArray<int32> PositionY;


	//Directions for X and Y
	int8 dX[4] = { 0, 1, 0, -1 };
	int8 dY[4] = { 1, 0, -1, 0 };

	//Max Moves for Map
	UPROPERTY(EditAnywhere)
	uint32 MaxMoves = 99;

	//Max Depth for Map Generation Algorithm
	//These can be very neatly adjusted for specific procedural maps
	UPROPERTY(EditAnywhere)
	uint32 MaxDepth = 39;

	//Max number of intersecting paths
	UPROPERTY(EditAnywhere)
	uint32 MaxIntersections = 32;

	//Total Current Moves
	uint32 TotalMoves = 0;

	//Total Current Path Intersections
	uint32 TotalIntersections = 0;

	//Remaining Moves in Direction
	uint8 MovesInDirection;

	//Current Direction
	//TODO implement directions as Enums
	//enum EDirection Direction;

	uint8 Direction;

	//Have ve crashed ? Can also be manipulated to get a desired type of maze generated
	bool bCrash = 0;

	///////////
	//Functions

	void InitBFS();

	//Actually A modified version of DFS, probably a combination of both
	void BFS(int X, int Y, int Move, uint32 Depth);

	//Peek if the Next move is going to be an intersection or a dead end, and decide the next move considering the Max variables set, accordingly
	bool PeekNext(int X, int Y);

	//Acquire New Direction
	uint8 AcquireDirection();

	//Acquire new number of moves in Direction
	uint8 AcquireMoves();

	/////////
	//Objects
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> PathBPClass;

	AActor* SpawnedActor;

	void SpawnMap();
	///////////////////////////////////////////////////////////////////////
	
	class ADefiler* Defiler;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ADefiler> DefilerBPClass;

	FTimerHandle DefilerSpawnHandle;

	float DefilerSpawnDelay = 1.5f;

	//the minions are just being spawned up to a limit of five; for testing purposes
	void SpawnMinion();

	int MinionCount = 0;


public:
	ACartagraGameModeBase();
	virtual void BeginPlay() override;

};
