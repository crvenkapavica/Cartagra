// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CartagraPlayerController.generated.h"

/**
 *
 */
UCLASS()
class CARTAGRA_API ACartagraPlayerController : public APlayerController
{
	GENERATED_BODY()

private:

	class APlayerCharacter* Player;

	bool bMoveToMouseCursor = 0;

	//Move to DestLocation
	void MoveToMouseCursor(float DeltaTime);

	//While holding the mouse key keep updating the DestLocation
	void SetNewMoveDestination(const FVector DestLocation);

	void OnCameraZoomIn();
	void OnCameraZoomOut();

	void OnSetDestinationPressed();
	void OnSetDestinationReleased();

	//this is used for Primary attack (LM), but could be implemented as a template that every ability uses the same set of functions
	void OnBeginPrimaryAttack();
	void OnEndPrimaryAttack();

	//Alternate mode is while holding down "SHIFT" so that the Player can attack without moving, as is basically standard on ARPG games today
	void OnBeginAlternateMode();
	void OnEndAlternateMode();

protected:

	virtual void PlayerTick(float DeltaTime) override;


public:
	ACartagraPlayerController();
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;
};
