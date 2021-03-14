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

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ACartagraAIController::Move_Idle, 2.2f, true, 0.3f);
}

void ACartagraAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACartagraAIController::FaceLocation(FVector Location)
{
	FRotator LookAtRotation = ((Location - GetPawn()->GetActorLocation()).Rotation());
	LookAtRotation.Pitch = 0.f;

	GetPawn()->SetActorRotation(LookAtRotation);
}

void ACartagraAIController::Move_Idle()
{
	float X = GetPawn()->GetActorLocation().X + FMath::RandRange(-162, 162);
	float Y = GetPawn()->GetActorLocation().Y + FMath::RandRange(-162, 162);

	const FVector Location{ X, Y, 1.f };

	if (FVector::Dist(Location, Cast<ADefiler>(GetPawn())->StartLocation) > 162) return;

	FaceLocation(Location);
	MoveToLocation(Location, 25.f, false, true, false, false, nullptr, true);
}
