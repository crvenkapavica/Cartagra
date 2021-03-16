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

	//Calculate Damage; For now the Damage system is not implemented it is just a per Character BaseDamage
	void CalculateDamage(ACartagraCharacter* TargetHit, uint64 Damage);

	//Apply Calculated Damage After accounting for procs and resistances etc.. to the Target we were Calculating for
	void ApplyDamage(ACartagraCharacter* DamagedTarget, uint64 DamageToApply);

	//Play animations, resurrections.....
	void OnBeginDeath(ACartagraCharacter* DyingTarget);

	//Start rolling rool, assing kill counts, and destroy
	void OnEndDeath(ACartagraCharacter* KilledTarget);

	UFUNCTION()
	void OnTargetHit(ACartagraCharacter* Target);

	UFUNCTION()
	void OnHit();

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

	//Keep track of base health and keep track of Actual Health
	//Values could be negative because of specific debuffs therefore shouldnn't be unsigned
	int64 BaseHealth;

	int64 ActualHealth;

	float AttackSpeed;

	int64 BaseDamage;

	int64 ActualDamage;

	int64 Armor;

	float Resistance;
	///////////////////////////////////////////////////////////////////////////////

	void OnBeginPrimaryAttack();
	void OnEndPrimaryAttack();

	void EndPrimaryAttack();
};
