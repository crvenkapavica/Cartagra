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
#include "Blueprint/AIBlueprintHelperLibrary.h"

ADefiler::ADefiler()
{
	PrimaryActorTick.bCanEverTick = true;

	//add struct config here
	BaseStats.MovementSpeed = 225.f;
	BaseStats.AttackRange = 12.f;
	BaseStats.BaseDamage = 4.f;
	Stats.AggroRange = 550.f;
	Health = BaseStats.BaseHealth;
}

// Called when the game starts or when spawned
void ADefiler::BeginPlay()
{
	Super::BeginPlay();

	SpawnDefaultController();

	Player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

	StartLocation = GetActorLocation();

	MoveComp = Cast<UCharacterMovementComponent>(GetComponentByClass(UCharacterMovementComponent::StaticClass()));
	MoveComp->MaxWalkSpeed = BaseStats.MovementSpeed;

	GetWorldTimerManager().SetTimer(MoveIdleTimerHandle, this, &ADefiler::Move_Idle, 2.2f, true, 0.3f);
}

// Called every frame
void ADefiler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bPlayerHasAggro)
	{
		CheckIsPlayerInAggroRange();
	}
	else if (CheckIsPlayerInChaseRange())
	{
		ChasePlayer();
	}
}

void ADefiler::OnCompleteAction()
{
	CompleteAction();
}

void ADefiler::CheckIsPlayerInAggroRange()
{
	if (FVector::Dist(Player->GetActorLocation(), GetActorLocation()) <= Stats.AggroRange)
	{
		bPlayerHasAggro = true;
		GetWorldTimerManager().ClearTimer(MoveIdleTimerHandle);
	}
}

bool ADefiler::CheckIsPlayerInAttackRange()
{
	if (FVector::Dist(Player->GetActorLocation(), GetActorLocation()) <= BaseStats.AttackRange)
	{
		return true;
	}
	return false;
}

bool ADefiler::CheckIsPlayerInChaseRange()
{
	if (FVector::Dist(GetActorLocation(), StartLocation) > AGGRO_FOLLOW_RANGE)
	{
		bPlayerHasAggro = false;
		MoveToLocation(StartLocation);
		GetWorldTimerManager().SetTimer(MoveIdleTimerHandle, this, &ADefiler::Move_Idle, 2.2f, true, 0.3f);
		return false;
	}
	return true;
}

void ADefiler::ChasePlayer()
{
	//face player
	FaceLocation(Player->GetActorLocation());
	
	//if out of ranged attack move closer
	if (!CheckIsPlayerInAttackRange())
	{
		MoveToLocation(Player->GetActorLocation());
	}

	OnInitiateAction(this, Player->GetActorLocation());
}

void ADefiler::FaceLocation(FVector Location)
{
	FRotator LookAtRotation = ((Location - GetActorLocation()).Rotation());
	LookAtRotation.Pitch = 0.f;

	SetActorRotation(LookAtRotation);
}

void ADefiler::Move_Idle()
{
	float X = GetActorLocation().X + FMath::RandRange(-162, 162);
	float Y = GetActorLocation().Y + FMath::RandRange(-162, 162);

	const FVector Location{ X, Y, 1.f };

	if (FVector::Dist(Location, StartLocation) > 162) return;

	FaceLocation(Location);
	MoveToLocation(Location);
}

void ADefiler::MoveToLocation(FVector DestLocation)
{
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), DestLocation);
}


void ADefiler::OnInitiateAction(ACartagraCharacter* ActionInstigator, FVector HitLocation)
{
	InitiateAction(ActionInstigator, HitLocation);
}

void ADefiler::OnInitiateAction(ACartagraCharacter* Target)
{
	InitiateAction(Target);
}