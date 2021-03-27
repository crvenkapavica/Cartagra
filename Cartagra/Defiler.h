// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CartagraCharacter.h"
#include "Defiler.generated.h"

UENUM()
enum class EDefilerType : uint16
{
	DT_Normal =		 1, 
	DT_Elite =		 2,
	DT_Champion =	 4,
	DT_Golden =		 8,
	DT_Cursed =		16,
};

USTRUCT()
struct FDefilerStats
{
	GENERATED_BODY()

	float AggroRange;

	EDefilerType DefilerType;
};

UCLASS()
class CARTAGRA_API ADefiler : public ACartagraCharacter
{
	GENERATED_BODY()

public:
	ADefiler();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	FDefilerStats Stats;

	FVector StartLocation;

	class UCharacterMovementComponent* MoveComp;


	void OnInitiateAction(ACartagraCharacter* ActionInstigator, FVector HitLocaiton);

	void OnInitiateAction(ACartagraCharacter* Target);

	//Questionable, perhaps the base should take care of it 
	void OnCompleteAction();

private:

	class APlayerCharacter* Player;

};
