// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CartagraCharacter.h"
#include "PlayerCharacter.generated.h"

/**
 * This class is Derived from ACartagraCharacter which is a base class for all characters
 * Inventory should be implemented here, as well as abilities, resistances, procs etc.. specific to players
 */
UCLASS()
class CARTAGRA_API APlayerCharacter : public ACartagraCharacter
{

private:

	class USpringArmComponent* CameraBoom;

	class UCameraComponent* CameraComponent;

public:
	APlayerCharacter();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	USpringArmComponent* GetCameraBoom() { return CameraBoom; }

	GENERATED_BODY()
	
};
