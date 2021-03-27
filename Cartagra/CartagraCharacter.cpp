#include "CartagraCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/InterpolationComponent.h"
#include "Components/CapsuleComponent.h"
#include "CartagraPlayerController.h"
#include "Defiler.h"
#include "PlayerCharacter.h"


ACartagraCharacter::ACartagraCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(33.f, 88.f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->MaxWalkSpeed = 482.f;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 482.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
}

void ACartagraCharacter::BeginPlay()
{
	Super::BeginPlay();	

	PlayerController = Cast<ACartagraPlayerController>(GetController());


	//ActualHealth = /*Apply Modifiers*/ BaseHealth;  
	//ActualDamage = /*Apply Modifier*/ BaseDamage
}

void ACartagraCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



void ACartagraCharacter::InitiateAction(ACartagraCharacter* ActionInstigator, FVector HitLocation)
{
	FVector Destination = (HitLocation - ActionInstigator->GetActorLocation()) * CharacterStats.AttackRange;
	FRotator Rotation = (Destination - ActionInstigator->GetActorLocation()).Rotation();
	BeginPrimaryAttack(Destination, Rotation, ActionInstigator);
}

void ACartagraCharacter::InitiateAction(ACartagraCharacter* Target)
{
	BeginPrimaryAttack(Target);
}

void ACartagraCharacter::CompleteAction()
{
	bMoving = false;
}

void ACartagraCharacter::EndPrimaryAttack()
{

}

void ACartagraCharacter::BeginPrimaryAttack(ACartagraCharacter* Target)
{
	if (bAttackSpeedCheck == false) return;
	bAttackSpeedCheck = false;

	GetWorldTimerManager().SetTimer(AttackSpeedTimerHandle, this, &ACartagraCharacter::AttackSpeedTimer, CharacterStats.AttackSpeed, false, CharacterStats.AttackSpeed);
	bAttacking = true;

	PrimaryAttack(Target);

	bAttacking = false;
	bMoving = false;
}

void ACartagraCharacter::BeginPrimaryAttack(FVector Location, FRotator Rotation, ACartagraCharacter* ActionInstigator)
{
	if (bAttackSpeedCheck == false) return;
	bAttackSpeedCheck = false;

	GetWorldTimerManager().SetTimer(AttackSpeedTimerHandle, this, &ACartagraCharacter::AttackSpeedTimer, CharacterStats.AttackSpeed, false, CharacterStats.AttackSpeed);
	bAttacking = true;

	PrimaryAttack(Location, Rotation, ActionInstigator);
	

	bAttacking = false;
	bMoving = false;
}

//TODO all abilities should use the same sets of function Calls for initiating and ending them
void ACartagraCharacter::PrimaryAttack(ACartagraCharacter* Target)
{
	SpawnProjectile(Target);
}

void ACartagraCharacter::PrimaryAttack(FVector Location, FRotator Rotation, ACartagraCharacter* ActionInstigator)
{
	SpawnProjectile(Location, Rotation, ActionInstigator);
}

void ACartagraCharacter::SpawnProjectile(ACartagraCharacter* Target)
{
	AActor* Projectile = GetWorld()->SpawnActor<AActor>(PrimaryAttackProjectile_BP, GetActorLocation(), (Target->GetActorLocation() - GetActorLocation()).Rotation());

	UInterpolationComponent* InterpComp = Cast<UInterpolationComponent>(Projectile->GetComponentByClass(UInterpolationComponent::StaticClass()));
	InterpComp->InterpSpeed = 1500.f;
	InterpComp->bHoming = true;
	InterpComp->HomingTarget = Target;
	InterpComp->bIsDestroyable = true;
	InterpComp->Caller = this;
	InterpComp->OnHit.BindUFunction(this, FName("OnHit"));
	InterpComp->OnTargetHit.BindUObject(this, &ACartagraCharacter::OnTargetHit);
}

void ACartagraCharacter::SpawnProjectile(FVector Location, FRotator Rotation, ACartagraCharacter* ActionInstigator)
{
	Rotation.Pitch = 0.f;
	AActor* Projectile = GetWorld()->SpawnActor<AActor>(PrimaryAttackProjectile_BP, ActionInstigator->GetActorLocation(), Rotation);

	UInterpolationComponent* InterpComp = Cast<UInterpolationComponent>(Projectile->GetComponentByClass(UInterpolationComponent::StaticClass()));
	InterpComp->InterpSpeed = 1200.f;
	InterpComp->bHoming = false;
	InterpComp->StaticTarget = Location;
	InterpComp->bIsDestroyable = true;
	InterpComp->Caller = this;
	InterpComp->OnHit.BindUFunction(this, FName("OnHit"));
	InterpComp->OnTargetHit.BindUObject(this, &ACartagraCharacter::OnTargetHit);
}

void ACartagraCharacter::CalculateDamage(ACartagraCharacter* TargetHit, uint64 Damage)
{
	//After Calulating resistances, armor, procs...Apply the calculated Damage
	//for now this is not implemented
	//uint64 CalculatedDamage = Damage - Armor;

	//ApplyDamage(TargetHit, CalculatedDamage);
}

void ACartagraCharacter::ApplyDamage(ACartagraCharacter* DamagedTarget, uint64 DamageToApply)
{
	//DamagedTarget->ActualHealth -= DamageToApply;

	//if (DamagedTarget->ActualHealth <= 0)
	{
		OnBeginDeath(DamagedTarget);
	}
}

void ACartagraCharacter::OnBeginDeath(ACartagraCharacter* DyingTarget)
{
	//Check for possible resurrection / respawn thingies
	//Play Death animation / sound
	
	//if the TargetReallyDied Kill the Target
	OnEndDeath(DyingTarget);
}

void ACartagraCharacter::OnEndDeath(ACartagraCharacter* KilledTarget)
{
	//Check for loot drops, Dead Target types, for special functionality...
	//Perhaps call Delegates
	//Assign Kill Count and destroy;


	
	//RollLoot(KilledTarget->Stats);


	KilledTarget->Destroy();
}

void ACartagraCharacter::OnTargetHit(ACartagraCharacter* Target)
{
	//call CalculateDamage
	//perhaps add other Dynamic Delegates for procs..
	//CalculateDamage(Target, ActualDamage);
}


void ACartagraCharacter::OnHit()
{
	//We hit nothing, play animation
	//Can be used for ground effects and abilities
}

UCartagraItem* ACartagraCharacter::RollLoot(FDefilerStats& KilledDefilerStats)
{
	//take into account player magic find and others possible shiets
	float MagicFind = Cast<APlayerCharacter>(PlayerController->GetPawn())->CharacterStats.MagicFind;

	switch (KilledDefilerStats.DefilerType)
	{
	case EDefilerType::DT_Normal:
		break;
	case EDefilerType::DT_Elite:
		break;
	}

	return nullptr;
}

void ACartagraCharacter::AttackSpeedTimer()
{
	bAttackSpeedCheck = true;
}