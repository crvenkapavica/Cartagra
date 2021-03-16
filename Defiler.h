// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CartagraCharacter.h"
#include "Defiler.generated.h"

UCLASS()
class CARTAGRA_API ADefiler : public ACartagraCharacter
{
	GENERATED_BODY()

private:


public:

	class UCharacterMovementComponent* MoveComp;


	ADefiler();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

};
