// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CartagraAIController.generated.h"

/**
 * 
 */
UCLASS()
class CARTAGRA_API ACartagraAIController : public AAIController
{
	GENERATED_BODY()

public:
	ACartagraAIController();

private:

	class UCharacterMovementComponent* MoveComp;

	//Just some simple Idle movement when the player is out of range of aggro
	void Move_Idle();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
	
	//Face the location towards moving
	//In case of aggroing a Player, it can also be used to face Player's location
	void FaceLocation(FVector Location);
};
