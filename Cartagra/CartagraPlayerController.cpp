#include "CartagraPlayerController.h"
#include "PlayerCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

ACartagraPlayerController::ACartagraPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void ACartagraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<APlayerCharacter>(GetPawn());
}

void ACartagraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (bMoveToMouseCursor)
	{
		MoveToMouseCursor(DeltaTime);
	}
}

void ACartagraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &ACartagraPlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &ACartagraPlayerController::OnSetDestinationReleased);

	InputComponent->BindAction("CameraZoomIn", IE_Pressed, this, &ACartagraPlayerController::OnCameraZoomIn);
	InputComponent->BindAction("CameraZoomOut", IE_Pressed, this, &ACartagraPlayerController::OnCameraZoomOut);

	InputComponent->BindAction("AlternateMode", IE_Pressed, this, &ACartagraPlayerController::OnBeginAlternateMode);
	InputComponent->BindAction("AlternateMode", IE_Released, this, &ACartagraPlayerController::OnEndAlternateMode);
}

void ACartagraPlayerController::MoveToMouseCursor(float DeltaTime)
{
	// Trace to see what is under the mouse cursor
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if (Hit.bBlockingHit)
	{
		// We hit something, move there
		SetNewMoveDestination(Hit.ImpactPoint);
	}
}

void ACartagraPlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	if (Player)
	{
		float const Distance = FVector::Dist(DestLocation, Player->GetActorLocation());

		// We need to issue move command only if far enough in order for walk animation to play correctly
		if ((Distance > 120.0f))
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
		}
	}
}

void ACartagraPlayerController::OnSetDestinationPressed()
{
	Player->OnBeginPrimaryAttack();
	bMoveToMouseCursor = Player->bMoving && !Player->bAttacking;
}

void ACartagraPlayerController::OnSetDestinationReleased()
{
	Player->OnEndPrimaryAttack();
	bMoveToMouseCursor = false;
}

void ACartagraPlayerController::OnCameraZoomIn()
{
	//TODO VInterp for smooth
	//Maybe rotation
	if (Player && Player->GetCameraBoom()->TargetArmLength > 350.f)
	{
		Player->GetCameraBoom()->TargetArmLength = Player->GetCameraBoom()->TargetArmLength - 50.f;
	}
}

void ACartagraPlayerController::OnCameraZoomOut()
{
	if (Player && Player->GetCameraBoom()->TargetArmLength < 1350.f)
	{
		Player->GetCameraBoom()->TargetArmLength = Player->GetCameraBoom()->TargetArmLength + 50.f;
	}
}

void ACartagraPlayerController::OnBeginPrimaryAttack()
{
	Player->OnBeginPrimaryAttack();
}

void ACartagraPlayerController::OnEndPrimaryAttack()
{
	Player->OnEndPrimaryAttack();
}

void ACartagraPlayerController::OnBeginAlternateMode()
{
	Player->bAlternateMode = true;
}

void ACartagraPlayerController::OnEndAlternateMode()
{
	Player->bAlternateMode = false;
}