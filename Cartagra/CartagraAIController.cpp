#include "CartagraAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../PlayerCharacter.h"
#include "Defiler.h"

ACartagraAIController::ACartagraAIController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACartagraAIController::BeginPlay()
{
	Super::BeginPlay();

	PossessedPawn = Cast<ADefiler>(GetPawn());

	//FTimerHandle TimerHandle;
	//GetWorldTimerManager().SetTimer(TimerHandle, this, &ACartagraAIController::Move_Idle, 2.2f, true, 0.3f);
}

void ACartagraAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsPlayerInRange && PossessedPawn)
	{
		CheckIsPlayerInRange();
	}
}

void ACartagraAIController::FaceLocation(FVector Location)
{
	FRotator LookAtRotation = ((Location - PossessedPawn->GetActorLocation()).Rotation());
	LookAtRotation.Pitch = 0.f;

	PossessedPawn->SetActorRotation(LookAtRotation);
}

void ACartagraAIController::Move_Idle()
{
	if (!PossessedPawn) return;

	float X = GetPawn()->GetActorLocation().X + FMath::RandRange(-162, 162);
	float Y = GetPawn()->GetActorLocation().Y + FMath::RandRange(-162, 162);

	const FVector Location{ X, Y, 1.f };

	if (FVector::Dist(Location, PossessedPawn->StartLocation) > 162) return;

	FaceLocation(Location);
	MoveToLocation(Location, 25.f, false, true, false, false, nullptr, true);
}

void ACartagraAIController::CheckIsPlayerInRange()
{
	if (ACartagraCharacter* Player = Cast<ACartagraCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn()))
	{
		if (FVector::Dist(PossessedPawn->GetActorLocation(), Player->GetActorLocation()) <= PossessedPawn->Stats.AggroRange)
		{
			bIsPlayerInRange = true;
			//begin attack 
			FaceLocation(Player->GetActorLocation());

			//Begin AI Point attack, AI never does a  homing attack unless specifically intented
			//Differ melee, ranged, spell depending on shiets;

			PossessedPawn->OnInitiateAction(PossessedPawn, Player->GetActorLocation());
		}
	}
}