#include "../Components/InterpolationComponent.h"
#include "../CartagraCharacter.h"

// Sets default values for this component's properties
UInterpolationComponent::UInterpolationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	HomingTarget = nullptr;
	InterpSpeed = 200.f;
}


void UInterpolationComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInterpolationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckOverlap();
	//Homing
	if (bHoming)
	{
		Interpolate(GetOwner()->GetActorLocation(), HomingTarget->GetActorLocation(), DeltaTime);
	}
	//Point
	else
	{
		Interpolate(GetOwner()->GetActorLocation(), StaticTarget, DeltaTime);
	}
}

void UInterpolationComponent::Interpolate(FVector StartLocation, FVector TargetLocation, float DeltaTime)
{
	if (GetOwner())
	{
		FVector CurrentLocation = FMath::VInterpConstantTo(GetOwner()->GetActorLocation(), TargetLocation, DeltaTime, InterpSpeed);		
		GetOwner()->SetActorLocation(CurrentLocation);
	}
}

void UInterpolationComponent::CheckOverlap()
{
	if (GetOwner())
	{
		if (bHoming && HomingTarget)
		{
			if (FMath::IsNearlyZero(FVector::Dist(GetOwner()->GetActorLocation(), HomingTarget->GetActorLocation())))
			{
				//zovi delegate i destroy
				
				//OnTargetHit();

				TargetHit.Execute();

				GetOwner()->Destroy();
			}
		}
		else if (!bHoming)
		{
			if (FMath::IsNearlyZero(FVector::Dist(GetOwner()->GetActorLocation(), StaticTarget)))
			{
				//zovi delegate i destroy

				//OnTargetHit();	

				GetOwner()->Destroy();
			}
		}
	}
}
