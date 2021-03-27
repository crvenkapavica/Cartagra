#include "CartagraAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Defiler.h"

ACartagraAIController::ACartagraAIController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACartagraAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ACartagraAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

