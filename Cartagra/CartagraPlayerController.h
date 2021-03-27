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

	bool bAlternateMode = 0;

	//Move to DestLocation
	void MoveToMouseCursor(float DeltaTime);

	//While holding the mouse key keep updating the DestLocation
	void SetNewMoveDestination(const FVector Destination);

	void CameraZoomIn();
	void CameraZoomOut();

	///Initiate a specific action (attacking, selecting, interacting)
	void OnInitiateAction();
	///Depending on the action that was initated, complete the task
	void OnCompleteAction();

	//SHIFT key is pressed, so set bAlternateMode = true
	void AlternateModePressed();
	void AlternateModeReleased();

protected:

	virtual void PlayerTick(float DeltaTime) override;


public:
	ACartagraPlayerController();
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;
};
