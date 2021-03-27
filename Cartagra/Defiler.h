// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CartagraCharacter.h"
#include "Defiler.generated.h"

#define AGGRO_FOLLOW_RANGE 350.f

UENUM()
enum class EDefilerTag : uint16
{
	DT_Normal =		 1, 
	DT_Elite =		 2,
	DT_Champion =	 4,
	DT_Golden =		 8,
	DT_Cursed =		16,
};

USTRUCT()
struct FDefilerType
{
	GENERATED_BODY()

	EDefilerTag DefilerTag;

	UPROPERTY(EditAnywhere)
	TSubclassOf<USkeletalMesh> BP_SkeletalMeshClass;
};

USTRUCT(BlueprintType)
struct FDefilerStats
{
	GENERATED_BODY()

	float AggroRange;

	FDefilerType DefilerType;
};

UCLASS()
class CARTAGRA_API ADefiler : public ACartagraCharacter
{
	GENERATED_BODY()

public:
	ADefiler();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<USkeletalMeshComponent> BP_SkeletalMeshClass;

	FDefilerStats Stats;

	FVector StartLocation;

	class APlayerCharacter* Player;

	void OnInitiateAction(ACartagraCharacter* ActionInstigator, FVector HitLocaiton);

	void OnInitiateAction(ACartagraCharacter* Target);

	//Questionable, perhaps the base should take care of it 
	void OnCompleteAction();

private:

	class UCharacterMovementComponent* MoveComp;

	void CheckIsPlayerInAggroRange();

	bool CheckIsPlayerInAttackRange();

	bool CheckIsPlayerInChaseRange();

	void ChasePlayer();
	
	//Just idle movement when Player is out of aggro range
	void Move_Idle();

	FTimerHandle MoveIdleTimerHandle;

	void MoveToLocation(FVector DestLocation);

	//Face the location towards moving
	//In case of aggroing a Player, it can also be used to face Player's location
	void FaceLocation(FVector Location);

	bool bPlayerHasAggro = false;
};
