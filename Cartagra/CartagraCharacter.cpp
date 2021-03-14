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
}

void ACartagraCharacter::BeginPlay()
{
	Super::BeginPlay();	

	//Controller = Cast<ACartagraPlayerController>(GetWorld()->GetFirstPlayerController());
	Controller = Cast<ACartagraPlayerController>(GetController());
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
	Controller->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, Hit);

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

//TODO maybe all use the same function?
//change assets depending on ability;
//Start 
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
	InterpComp->TargetHit.BindUFunction(this, FName("CalculateDamage"));
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
	//InterpComp->OnTargetHit(this, &ACartagraCharacter::CalculateDamage);
	InterpComp->TargetHit.BindUFunction(this, FName("CalculateDamage"));
}

void ACartagraCharacter::CalculateDamage()
{
	UE_LOG(LogTemp, Warning, TEXT("Calc dmg deleGATED"));
}

void ACartagraCharacter::OnTargetHit()
{

}