// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InterpolationComponent.generated.h"

DECLARE_DELEGATE(FOnHit);
//DECLARE_DELEGATE_OneParam(FOnTargetHit, class ACartagraCharacter*);
DECLARE_DELEGATE_TwoParams(FOnTargetHit, class ACartagraCharacter*, struct FCharacterStats&);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CARTAGRA_API UInterpolationComponent : public UActorComponent
{
	GENERATED_BODY()

private:

	//Interpolate the Projectile
	void Interpolate(FVector StartLocation, FVector TargetLocation, float DeltaTime);

	//Check for collision
	void CheckOverlap();

public:	
	// Sets default values for this component's properties
	UInterpolationComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//This will follow the target even though they have moved from the position they were shot at
	class ACartagraCharacter* HomingTarget;

	//Otherwise Use a simple FVector To interpolate the projectile
	//currenty this also uses a Z coordinate for realistic purposes, but can easly be adjusted 
	FVector StaticTarget;

	bool bHoming = false;
	
	UPROPERTY(EditAnywhere)
	float InterpSpeed;

	//Maybe it should persist for a while, so it shouldnt be destroyable immediately upon overlap / block
	UPROPERTY(EditAnywhere)
	bool bIsDestroyable = true;

	//Delegate for hitting nothing, can be used for area of effect abilities
	FOnHit OnHit;

	//Delegate for hitting an instanced Character
	FOnTargetHit OnTargetHit;

	//Have to store a Reference to the Spawner of the projectile for collision purposes
	//Can also be done with trace channels

	class ACartagraCharacter* Caller;
};
