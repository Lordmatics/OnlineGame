// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "OnlineGame.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "OnlineGameCharacter.h"
#include "Classes/Animation/AnimMontage.h"
#include "WeaponComponents/OnlineGameWeapon.h"
#include "WeaponComponents/RaycastComponent.h"
#include "Animation/AnimationComponent.h"
#include "OnlineGamePlayerController.h"

//////////////////////////////////////////////////////////////////////////
// AOnlineGameCharacter

void AOnlineGameCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(AOnlineGameCharacter, bIsDead);
}

AOnlineGameCharacter::AOnlineGameCharacter()
{
	bReplicates = true;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh());
	
	WeaponComponent = CreateDefaultSubobject<UOnlineGameWeapon>(TEXT("WeaponComponent"));

	RaycastComponent = CreateDefaultSubobject<URaycastComponent>(TEXT("RaycastComponent"));

	AnimationStorage = CreateDefaultSubobject<UAnimationComponent>(TEXT("AnimationComponent"));

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

void AOnlineGameCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (WeaponComponent != nullptr && Weapon != nullptr)
	{
		Weapon->SetSkeletalMesh(WeaponComponent->GetWeaponMesh());
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponComponent->GetWeaponSocketName());
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AOnlineGameCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AOnlineGameCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AOnlineGameCharacter::MoveRight);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AOnlineGameCharacter::BeginContinuousAttack);
	PlayerInputComponent->BindAction("Attack", IE_Released, this, &AOnlineGameCharacter::ResetAttack);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AOnlineGameCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AOnlineGameCharacter::LookUpAtRate);

	PlayerInputComponent->BindAction("Die", IE_Pressed, this, &AOnlineGameCharacter::Die);

}

void AOnlineGameCharacter::MulticastPlayerDied_Implementation()
{
	PlayerDied();
}

void AOnlineGameCharacter::ServerPlayerDied_Implementation()
{
	MulticastPlayerDied();
}

bool AOnlineGameCharacter::ServerPlayerDied_Validate()
{
	return true;
}


void AOnlineGameCharacter::Die()
{
	// It looks like this makes no sense
	// And  you could just use
	// MulticastPlayerDied
	// But i tested it, and
	// You do need the Server one apparently
	if (HasAuthority())
	{
		MulticastPlayerDied();
	}
	else
	{
		ServerPlayerDied();
	}
}

void AOnlineGameCharacter::PlayerDied()
{
	if (AnimationStorage != nullptr)
	{
		UAnimMontage* Anim = AnimationStorage->GetDeathAnimMontage();
		if (Anim != nullptr)
		{
			float Duration = PlayAnimMontage(Anim, 1.0f, NAME_None);
			UWorld* const World = GetWorld();
			if (World == nullptr) return;
			FTimerHandle DeathHandle;
			World->GetTimerManager().SetTimer(DeathHandle, this, &AOnlineGameCharacter::OnDied, Duration);
		}

	}
}

void AOnlineGameCharacter::OnDied()
{
	bIsDead = true;
	UE_LOG(LogTemp, Warning, TEXT("Dead !!! : %s"), bIsDead ? TEXT("True") : TEXT("False"));
}




























/// OPEN CLOSED PRINCIPLE
void AOnlineGameCharacter::BeginContinuousAttack()
{
	// Cache button pressed, so it auto resumes if 
	// It is still held down after the buffer reset
	UWorld* const World = GetWorld();
	bAttackPressedCached = true;
	if (World == nullptr) return;
	if (AnimationStorage != nullptr)
	{
		UAnimMontage* Anim = AnimationStorage->GetAttackAnimMontage();
		if (Anim != nullptr && !bAttackBufferActive)
		{
			// Get animation length, so timer is accurate
			// Call attack immediately to prevent
			// waiting the Duration before first attack
			// HENCE the need for the buffer, to prevent 
			// Attacking at infinite attack rate
			float Duration = Anim->GetPlayLength();
			Attack();
			World->GetTimerManager().SetTimer(BeginAttackHandle, this, &AOnlineGameCharacter::Attack, Duration, true);
		}
	}
}
void AOnlineGameCharacter::Attack()
{
	if (AnimationStorage != nullptr)
	{
		UAnimMontage* Anim = AnimationStorage->GetAttackAnimMontage();
		if (Anim != nullptr)
		{
			// Shoot a raycast, slightly ahead of your position
			// To determine if you have hit anything - Cast to Enemy etc
			float Duration = PlayAnimMontage(Anim, 1.0f, NAME_None);
			FHitResult Hit = ShootRay();
			UWorld* const World = GetWorld();
			if (World == nullptr) return;
			// Toggle attack buffer on, and start timer to end it
			bAttackBufferActive = true;
			World->GetTimerManager().SetTimer(BufferAttackHandle, this, &AOnlineGameCharacter::EndAttackBuffer, Duration);
		}
	}
}
void AOnlineGameCharacter::EndAttackBuffer()
{
	UWorld* const World = GetWorld();
	if (World == nullptr) return;
	// Disable Attack Buffer
	World->GetTimerManager().ClearTimer(BufferAttackHandle);
	bAttackBufferActive = false;
	// IF the Attack Button is still being held down by the player
	// Resume attacking - Quality of Life Sort of optimise
	if (bAttackPressedCached)
	{
		BeginContinuousAttack();
	}
}
void AOnlineGameCharacter::ResetAttack()
{
	UWorld* const World = GetWorld();
	if (World == nullptr) return;
	// Clear Attack Anim, and confirm the 
	// Attack button has been let go
	World->GetTimerManager().ClearTimer(BeginAttackHandle);
	bAttackPressedCached = false;
}
FHitResult AOnlineGameCharacter::ShootRay()
{
	if (RaycastComponent != nullptr)
	{
		// Fire a raycast through the component
		// And return what it hits
		FVector Direction = GetActorForwardVector();
		return RaycastComponent->RaycastTP(GetMesh(), Direction, 150.0f);
	}
	return FHitResult();
}
// Looking Around
void AOnlineGameCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}
void AOnlineGameCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}
// Moving Around
void AOnlineGameCharacter::MoveForward(float Value)
{
	// Cant move while attacking
	if (bAttackBufferActive) return;

	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}
void AOnlineGameCharacter::MoveRight(float Value)
{
	// Can't move while attacking
	if (bAttackBufferActive) return;

	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
