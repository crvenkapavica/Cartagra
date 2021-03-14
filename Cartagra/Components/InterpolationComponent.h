// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InterpolationComponent.generated.h"

DECLARE_DELEGATE(FOnTargetHit);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CARTAGRA_API UInterpolationComponent : public UActorComponent
{
	GENERATED_BODY()

private:

	void Interpolate(FVector StartLocation, FVector TargetLocation, float DeltaTime);


public:	
	// Sets default values for this component's properties
	UInterpolationComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;



	//TEMPLATED VERSION FOR CALLBACK FUNCTION 
	//DEPRECATED = USING DELEGATES
	/*
	template<class CustomClass>
	UFUNCTION()
	void OnTargetHit(CustomClass* Object, void (CustomClass::* Callback)())
	{
		//TargetHit = MakeShared<CustomClass>(Object, Callback);
		//TargetHit.BindUFunction(Object, FName("*Callback"));
		//TargetHit.BindRaw(Object, &CustomClass::CalculateDamage);
		//TargetHit.Execute();
		//(*TargetHit) = &(Object->*Callback)();
		//UE_LOG(LogTemp, Warning, TEXT("from Component"));
	}*/

	AActor* HomingTarget;

	FVector StaticTarget;

	bool bHoming = false;
	
	UPROPERTY(EditAnywhere)
	float InterpSpeed;

	UPROPERTY(EditAnywhere)
	bool bIsDestroyable = true;

	void CheckOverlap();

	FOnTargetHit TargetHit;
};
