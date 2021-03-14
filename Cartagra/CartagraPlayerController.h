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

	void MoveToMouseCursor(float DeltaTime);

	void SetNewMoveDestination(const FVector DestLocation);

	void OnCameraZoomIn();
	void OnCameraZoomOut();

	void OnSetDestinationPressed();
	void OnSetDestinationReleased();

	void OnBeginPrimaryAttack();
	void OnEndPrimaryAttack();

	void OnBeginAlternateMode();
	void OnEndAlternateMode();

protected:

	virtual void PlayerTick(float DeltaTime) override;


public:
	ACartagraPlayerController();
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;
};
