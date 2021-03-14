// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CartagraCharacter.generated.h"

UCLASS()
class CARTAGRA_API ACartagraCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	void OnSetDestinationPressed();

	//ACharacter* Target;

	class ACartagraPlayerController* Controller;

	FVector AbilityLocation;

	void BeginPrimaryAttack(class ADefiler* Target);
	void BeginPrimaryAttack(FVector Location, FRotator Rotation);

	/// This is a homing attack.
	void PrimaryAttack(class ADefiler* Target);

	/// This is a point attack.
	void PrimaryAttack(FVector Location, FRotator Rotation);

	///Spawn homing projectiles and start interpolation.
	void SpawnProjectile(class ADefiler* Target);

	//Spawn projectile at point and start interpolation.
	void SpawnProjectile(FVector Location, FRotator Rotation);


	//Callback for calculating and applying damage after projectile hits a target
	UFUNCTION()
	void CalculateDamage();

	void OnTargetHit();


public:

	bool bMoving = false;
	bool bAttacking = false;
	bool bAlternateMode = false;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> PrimaryAttackProjectile_BP;

	float AttackRange = 20.f;

	ACartagraCharacter();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:

	//CHARACTER STATS
	//*************************************************
	float MovementSpeed;

	FVector StartLocation;

	float AttackSpeed;

	uint64 BaseDamage;
	///////////////////////////////////////////////////////////////////////////////

	void OnBeginPrimaryAttack();
	void OnEndPrimaryAttack();

	void EndPrimaryAttack();
};
