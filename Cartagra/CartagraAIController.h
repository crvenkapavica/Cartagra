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

	void Move_Idle();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
	
	void FaceLocation(FVector Location);
};
