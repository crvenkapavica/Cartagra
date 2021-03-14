#include "Defiler.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/InterpolationComponent.h"
#include "CartagraAIController.h"
#include "GameFramework/CharacterMovementComponent.h"

ADefiler::ADefiler()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADefiler::BeginPlay()
{
	Super::BeginPlay();

	SpawnDefaultController();

	StartLocation = GetActorLocation();

	MoveComp = Cast<UCharacterMovementComponent>(GetComponentByClass(UCharacterMovementComponent::StaticClass()));
	MoveComp->MaxWalkSpeed = MovementSpeed;
}

// Called every frame
void ADefiler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}