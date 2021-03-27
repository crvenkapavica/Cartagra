#include "../Components/InterpolationComponent.h"
#include "../CartagraCharacter.h"

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
	if (bHoming && HomingTarget)
	{
		if (FMath::IsNearlyZero(FVector::Dist(GetOwner()->GetActorLocation(), HomingTarget->GetActorLocation())))
		{
			OnTargetHit.ExecuteIfBound(HomingTarget, Caller->BaseStats);

			GetOwner()->Destroy();
		}
	}
	else if (!bHoming)
	{
		
		TArray<AActor*> ActorsOverlapped;
		GetOwner()->GetOverlappingActors(ActorsOverlapped, ACartagraCharacter::StaticClass());

		if (ActorsOverlapped.Num())
		{
			for (AActor* Actor : ActorsOverlapped)
			{
				if (Actor->GetClass() != Caller->GetClass())
				{
					OnTargetHit.ExecuteIfBound(Cast<ACartagraCharacter>(Actor), Caller->BaseStats);

					GetOwner()->Destroy();
				}
			}
		}
		else if (FMath::IsNearlyZero(FVector::Dist(GetOwner()->GetActorLocation(), StaticTarget)))
		{
			OnHit.ExecuteIfBound();

			GetOwner()->Destroy();
		}
	}
}
