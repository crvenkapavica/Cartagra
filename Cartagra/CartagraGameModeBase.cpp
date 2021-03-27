#include "CartagraGameModeBase.h"
#include "CartagraPlayerController.h"
#include "CartagraCharacter.h"
#include "Defiler.h"
#include "CartagraAIController.h"

enum EDirection
{
	Front = 1,
	Back = 3,
	Left = 2,
	Right = 0,
};


ACartagraGameModeBase::ACartagraGameModeBase()
{
	PlayerControllerClass = ACartagraPlayerController::StaticClass();
}

void ACartagraGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	InitBFS();
	BFS(5, 5, 0, 0);
	SpawnMap();

	
	GetWorldTimerManager().SetTimer(DefilerSpawnHandle, this, &ACartagraGameModeBase::SpawnMinion, DefilerSpawnDelay, true, 0.f);
}

void ACartagraGameModeBase::InitBFS()
{
	Direction = AcquireDirection();
}

void ACartagraGameModeBase::BFS(int X, int Y, int Move, uint32 Depth)
{
	if (X < 0 || X >= 333) return;
	if (Y < 0 || Y >= 333) return;
	if (TotalMoves >= MaxMoves) return;
	if (Depth > MaxDepth) return;
	if (Map[X][Y] && PeekNext(X, Y)) return;

	PositionX.Add(X);
	PositionY.Add(Y);
	Map[X][Y] = TotalMoves;

	TotalMoves++;

	if (!--MovesInDirection)	
	{
		Direction = AcquireDirection();
		Depth = 0;
	}

	for (int i = 0; i < 4; i++)
	{
		Direction = (Direction + i) % 4;
		BFS(X + dX[Direction], Y + dY[Direction], TotalMoves + 1, Depth + 1);
	}
}

bool ACartagraGameModeBase::PeekNext(int X, int Y)
{
	if (++TotalIntersections > MaxIntersections) return 0;

	X += dX[Direction];
	Y += dY[Direction];
	return Map[X][Y] ? 1 : 0;
}

uint8 ACartagraGameModeBase::AcquireDirection()
{
	MovesInDirection = AcquireMoves();
	uint8 NewDirection;

	do {
		NewDirection = FMath::RandRange(0, 3);
	} while (Direction == NewDirection);

	/*switch (NewDirection)
	{
	case 0:
		return EDirection::Right;
	case 1:
		return EDirection::Front;
	case 2:
		return EDirection::Left;
	case 3:
		return EDirection::Back;
	default:
		return EDirection::Front;
	}*/

	return NewDirection;
}

uint8 ACartagraGameModeBase::AcquireMoves()
{
	return FMath::RandRange(4, 12);
}

void ACartagraGameModeBase::SpawnMap()
{
	for (int Index = 0; Index < PositionX.Num(); Index++)
	{
		FVector Location{ PositionX[Index] * 100.f, PositionY[Index] * 100.f, 0.f };
		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(PathBPClass, Location, FRotator::ZeroRotator);
	}
}	

void ACartagraGameModeBase::SpawnMinion()
{
	float X = FMath::RandRange(-250, 250);
	float Y = FMath::RandRange(-250, 250);

	FVector Location{ X, Y, 125.f };
	ADefiler* Defiler = GetWorld()->SpawnActor<ADefiler>(DefilerBPClass, Location, FRotator::ZeroRotator);

	if (++MinionCount == 5)
	{
		GetWorldTimerManager().ClearTimer(DefilerSpawnHandle);
	}
}