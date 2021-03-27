// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CartagraCharacter.generated.h"


USTRUCT()
struct FDefensiveStats
{
	GENERATED_BODY()

	//Percent to avoid crushing blows
	float Resilience;

	//chance to block a fixed amount
	float BlockChance;

	float MeleeDodgeChance;

	float RangedDodgeChance;
};

USTRUCT()
struct FOffensiveStats
{
	GENERATED_BODY()

	float HitChance;

	float CritChance;

	float CritDamage;
};

USTRUCT()
struct FElementalResistance
{
	GENERATED_BODY()

	float FireResistance;
	float FrostResistance;
	float LightningResistance;

	FElementalResistance() : FireResistance(0.f), FrostResistance(0.f), LightningResistance(0.f) { }
};

USTRUCT()
struct FToxicResistance
{
	GENERATED_BODY()

	float AcidResistance;
	float PoisonResistance;
	float AlkalineResistance;

	FToxicResistance() : AcidResistance(0.f), PoisonResistance(0.f), AlkalineResistance(0.f) { }
};

USTRUCT()
struct FResistanceTypes
{
	GENERATED_BODY()

	FElementalResistance ElementalResistance;
	FToxicResistance ToxicResistance;

	FResistanceTypes() { }
};

USTRUCT()
struct FElementalDamage
{
	GENERATED_BODY()

	float FireDamage;
	float FrostDamage;
	float LightningDamage;

	FElementalDamage() : FireDamage(0.f), FrostDamage(0.f), LightningDamage(0.f) { }
};

USTRUCT()
struct FToxicDamage
{
	GENERATED_BODY()

	float AcidDamage;
	float PoisonDamage;
	float AlkalineDamage;

	FToxicDamage() : AcidDamage(0.f), PoisonDamage(0.f), AlkalineDamage(0.f) { }
};

USTRUCT()
struct FDamageTypes
{
	GENERATED_BODY()

	FElementalDamage ElementalDamage;
	FToxicDamage ToxicDamage;

	FDamageTypes() { }
};


USTRUCT()
struct FCharacterStats
{
	GENERATED_BODY()

	float BaseDamage;

	float AttackSpeed;

	float AttackRange;

	int64 BaseHealth;

	float BaseArmor;

	float MovementSpeed;

	//Percentage
	float MagicFind;

	FDefensiveStats Defense;

	FOffensiveStats Offense;

	FResistanceTypes Resistances;

	FDamageTypes DamageTypes;

	FCharacterStats() : BaseDamage(3.f), AttackSpeed(1.f), AttackRange(50.f), BaseHealth(100), BaseArmor(0.f), MovementSpeed(200.f), MagicFind(0.f) { }

	//also implement a Resource (mana, rage...etc) typedef, enum, struct???
};


UCLASS()
class CARTAGRA_API ACartagraCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACartagraCharacter();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	class ACartagraPlayerController* PlayerController;

	FCharacterStats CharacterStats;

	FVector AbilityLocation;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> PrimaryAttackProjectile_BP;

	FTimerHandle AttackSpeedTimerHandle;

protected:

	//Controller checks and decides whether to perform and attack, interaction etc..
	//If this is called we're already sure we want to perform some kind of attack / ability
	///Target is a location
	void InitiateAction(ACartagraCharacter* ActionInstigator, FVector HitLocation);

	///Target is a Character
	void InitiateAction(ACartagraCharacter* Target);

	void CompleteAction();

private:

	void EndPrimaryAttack();

	void BeginPrimaryAttack(class ACartagraCharacter* Target);
	void BeginPrimaryAttack(FVector Location, FRotator Rotation, ACartagraCharacter* ActionInstigator);

	/// This is a homing attack.
	void PrimaryAttack(class ACartagraCharacter* Target);

	/// This is a point attack.
	void PrimaryAttack(FVector Location, FRotator Rotation, ACartagraCharacter* ActionInstigator);

	///Spawn homing projectiles and start interpolation.
	void SpawnProjectile(class ACartagraCharacter* Target);

	//Spawn projectile at point and start interpolation.
	void SpawnProjectile(FVector Location, FRotator Rotation, ACartagraCharacter* ActionInstigator);

	//Apply Calculated Damage After accounting for procs and resistances etc.. to the Target we were Calculating for
	void ApplyDamage(ACartagraCharacter* DamagedTarget, uint64 DamageToApply);

	//Calculate Damage; For now the Damage system is not implemented it is just a per Character BaseDamage
	void CalculateDamage(class ACartagraCharacter* TargetHit, uint64 Damage);

	//Play animations, resurrections.....
	void OnBeginDeath(class ACartagraCharacter* DyingTarget);

	//Start rolling rool, assing kill counts, and destroy
	void OnEndDeath(class ACartagraCharacter* KilledTarget);

	UFUNCTION()
	void OnTargetHit(class ACartagraCharacter* Target);

	UFUNCTION()
	void OnHit();

	class UCartagraItem* RollLoot(struct FDefilerStats& KilledDefilerStats);

	void AttackSpeedTimer();

	bool bMoving = false;
	bool bAttacking = false;
	bool bAlternateMode = false;
	//this bool checks whether we can attack yet, relative to out attack speed
	bool bAttackSpeedCheck = true;
};

