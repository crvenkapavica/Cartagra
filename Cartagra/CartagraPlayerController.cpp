#include "CartagraPlayerController.h"
#include "PlayerCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Defiler.h"

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

	InputComponent->BindAction("InitiateOrCompleteTask", IE_Pressed, this, &ACartagraPlayerController::OnInitiateAction);
	InputComponent->BindAction("InitiateOrCompleteTask", IE_Released, this, &ACartagraPlayerController::OnCompleteAction);

	InputComponent->BindAction("CameraZoomIn", IE_Pressed, this, &ACartagraPlayerController::CameraZoomIn);
	InputComponent->BindAction("CameraZoomOut", IE_Pressed, this, &ACartagraPlayerController::CameraZoomOut);

	InputComponent->BindAction("AlternateMode", IE_Pressed, this, &ACartagraPlayerController::AlternateModePressed);
	InputComponent->BindAction("AlternateMode", IE_Released, this, &ACartagraPlayerController::AlternateModeReleased);
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

void ACartagraPlayerController::SetNewMoveDestination(const FVector Destination)
{
	if (Player)
	{
		float const Distance = FVector::Dist(Destination, Player->GetActorLocation());

		// Dont move if the location is too close
		if ((Distance > 120.0f))
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, Destination);
		}
	}
}

//LM clicked, check if we are attacking, or interacting, etc...and call appropriate PlayerCharacter Functions
void ACartagraPlayerController::OnInitiateAction()
{
	//TODO Check what is under the mouse cursor and initiate the propriate task
	/*
	* Check under cursor; if its a walkable path; move the character there;
	* if its an enemy check for selected abilities and use the ability or the default attack;
	* if its an interactable object - initiate interaction;
	* ofcourse if its an inventory object do inventory stuff
	*/

	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Pawn, false, Hit);
	if (Hit.bBlockingHit)
	{
		if (bAlternateMode)
		{
			//Player->InitiateAction(Hit.ImpactPoint);
			Player->OnInitiateAction(Cast<ACartagraCharacter>(GetPawn()), Hit.ImpactPoint);
		}
		else if (ADefiler* TargetCharacter = Cast<ADefiler>(Hit.GetActor()))
		{
			Player->OnInitiateAction(TargetCharacter);
		}
		/*
		* else 
		* {
		*	we're walking towards a friendly NPC or interactable;	
		*	also maybe item;
		* }
		*/
		else
		{
			bMoveToMouseCursor = true;
		}
	}
}

void ACartagraPlayerController::OnCompleteAction()
{
	Player->OnCompleteAction();
	bMoveToMouseCursor = false;
}

void ACartagraPlayerController::CameraZoomIn()
{
	//TODO VInterp for smooth
	//so that the camera moves like on a swing
	if (Player && Player->GetCameraBoom()->TargetArmLength > 350.f)
	{
		Player->GetCameraBoom()->TargetArmLength = Player->GetCameraBoom()->TargetArmLength - 50.f;
	}
}

void ACartagraPlayerController::CameraZoomOut()
{
	if (Player && Player->GetCameraBoom()->TargetArmLength < 1350.f)
	{
		Player->GetCameraBoom()->TargetArmLength = Player->GetCameraBoom()->TargetArmLength + 50.f;
	}
}

void ACartagraPlayerController::AlternateModePressed()
{
	bAlternateMode = true;
}

void ACartagraPlayerController::AlternateModeReleased()
{
	bAlternateMode = false;
}