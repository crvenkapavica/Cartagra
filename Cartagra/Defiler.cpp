#include "Defiler.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/InterpolationComponent.h"
#include "Components/ProgressBar.h"
#include "CartagraAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "CartagraPlayerController.h"
#include "CartagraItem.h"
#include "PlayerCharacter.h"

ADefiler::ADefiler()
{
	PrimaryActorTick.bCanEverTick = true;

	//add struct config here
}

// Called when the game starts or when spawned
void ADefiler::BeginPlay()
{
	Super::BeginPlay();

	SpawnDefaultController();

	Player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

	StartLocation = GetActorLocation();

	CharacterStats.MovementSpeed = 25.f;
	Stats.AggroRange = 1550.f;

	MoveComp = Cast<UCharacterMovementComponent>(GetComponentByClass(UCharacterMovementComponent::StaticClass()));
	MoveComp->MaxWalkSpeed = CharacterStats.MovementSpeed;
}

// Called every frame
void ADefiler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADefiler::OnInitiateAction(ACartagraCharacter* ActionInstigator, FVector HitLocation)
{
	InitiateAction(ActionInstigator, HitLocation);
}

void ADefiler::OnCompleteAction()
{
	CompleteAction();
}