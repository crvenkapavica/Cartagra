// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CartagraGameModeBase.generated.h"


/**
 *
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

	UPROPERTY(EditAnywhere)
	uint32 MaxDepth = 39;

	UPROPERTY(EditAnywhere)
	uint32 MaxIntersections = 32;

	//Total Current Moves
	uint32 TotalMoves = 0;

	//Total Current Path Intersections
	uint32 TotalIntersections = 0;

	//Remaining Moves in Direction
	uint8 MovesInDirection;

	//Current Direction
	//enum EDirection Direction;

	uint8 Direction;


	bool bCrash = 0;

	///////////
	//Functions

	void InitBFS();

	void BFS(int X, int Y, int Move, uint32 Depth);

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

	void SpawnMinion();


	int MinionCount = 0;


public:
	ACartagraGameModeBase();
	virtual void BeginPlay() override;

};
