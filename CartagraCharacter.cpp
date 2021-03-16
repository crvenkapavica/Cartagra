#include "CartagraCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "CartagraPlayerController.h"
#include "Defiler.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/InterpolationComponent.h"

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

	//Starting Stats, TODO; use presets here
	MovementSpeed = 200.f;
	AttackSpeed = 5000.f;
	BaseDamage = 10;
	Armor = 2;
	//should be percenteage, not implemented
	Resistance = 0;
	BaseHealth = 100;
}

void ACartagraCharacter::BeginPlay()
{
	Super::BeginPlay();	

	Controller = Cast<ACartagraPlayerController>(GetController());

	ActualHealth = /*Apply Modifiers*/ BaseHealth;
	ActualDamage = /*Apply Modifier*/ BaseDamage;
}

void ACartagraCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ACartagraCharacter::OnBeginPrimaryAttack()
{
	if (bAttacking) return;

	bMoving = true;

	FHitResult Hit;
	Controller->GetHitResultUnderCursor(ECollisionChannel::ECC_Pawn, false, Hit);

	//if we clicked on a NPC, begin homing attack
	if (ADefiler* Target = Cast<ADefiler>(Hit.GetActor()))
	{
		BeginPrimaryAttack(Target);
	}
	//if not, begin point attack
	else if (bAlternateMode)
	{
		FVector Destination = (Hit.Location - GetActorLocation()) * AttackRange;
		FRotator Rotation = (Destination - GetActorLocation()).Rotation();
		BeginPrimaryAttack(Destination, Rotation);
	}
}

void ACartagraCharacter::OnEndPrimaryAttack()
{
	bMoving = false;
}

void ACartagraCharacter::BeginPrimaryAttack(ADefiler* Target)
{
	bAttacking = true;
	PrimaryAttack(Target);

	bAttacking = false;
	bMoving = false;
}

void ACartagraCharacter::BeginPrimaryAttack(FVector Location, FRotator Rotation)
{
	bAttacking = true;
	PrimaryAttack(Location, Rotation);

	bAttacking = false;
	bMoving = false;
}

//TODO all abilities should use the same sets of function Calls for initiating and ending them
void ACartagraCharacter::PrimaryAttack(ADefiler* Target)
{
	SpawnProjectile(Target);
}

void ACartagraCharacter::PrimaryAttack(FVector Location, FRotator Rotation)
{
	SpawnProjectile(Location, Rotation);
}

void ACartagraCharacter::SpawnProjectile(ADefiler* Target)
{
	AActor* Projectile = GetWorld()->SpawnActor<AActor>(PrimaryAttackProjectile_BP, GetActorLocation(), (Target->GetActorLocation() - GetActorLocation()).Rotation());

	UInterpolationComponent* InterpComp = Cast<UInterpolationComponent>(Projectile->GetComponentByClass(UInterpolationComponent::StaticClass()));
	InterpComp->InterpSpeed = 3500.f;
	InterpComp->bHoming = true;
	InterpComp->HomingTarget = Target;
	InterpComp->bIsDestroyable = true;
	InterpComp->Caller = this;
	InterpComp->OnHit.BindUFunction(this, FName("OnHit"));
	InterpComp->OnTargetHit.BindUObject(this, &ACartagraCharacter::OnTargetHit);
}

void ACartagraCharacter::SpawnProjectile(FVector Location, FRotator Rotation)
{
	Rotation.Pitch = 0.f;
	AActor* Projectile = GetWorld()->SpawnActor<AActor>(PrimaryAttackProjectile_BP, GetActorLocation(), Rotation);

	UInterpolationComponent* InterpComp = Cast<UInterpolationComponent>(Projectile->GetComponentByClass(UInterpolationComponent::StaticClass()));
	InterpComp->InterpSpeed = 1800.f;
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
	uint64 CalculatedDamage = Damage - Armor;

	ApplyDamage(TargetHit, CalculatedDamage);
}

void ACartagraCharacter::ApplyDamage(ACartagraCharacter* DamagedTarget, uint64 DamageToApply)
{
	DamagedTarget->ActualHealth -= DamageToApply;

	if (DamagedTarget->ActualHealth <= 0)
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
	KilledTarget->Destroy();
}

void ACartagraCharacter::OnTargetHit(ACartagraCharacter* Target)
{
	//call CalculateDamage
	//perhaps add other Dynamic Delegates for procs..
	CalculateDamage(Target, ActualDamage);
}


void ACartagraCharacter::OnHit()
{
	//We hit nothing, play animation
	//Can be used for ground effects and abilities
}
