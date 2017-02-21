// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "OnlineGame.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "OnlineGameCharacter.h"
#include "Classes/Animation/AnimMontage.h"
#include "WeaponComponents/OnlineGameWeapon.h"
#include "WeaponComponents/RaycastComponent.h"
#include "Animation/AnimationComponent.h"
#include "OnlineGamePlayerController.h"
#include "Lobby/OnlineGameLobbyPlayerController.h"
#include "Projectiles/WeaponProjectile.h"
#include "EnemyAI/EnemyAI.h"

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

	PlayerInputComponent->BindAxis("Turn", this, &AOnlineGameCharacter::MouseTurn);
	//PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AOnlineGameCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &AOnlineGameCharacter::MouseLook);
//	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
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
void AOnlineGameCharacter::MulticastStartAttacking_Implementation()
{
	StartAttacking();
}

void AOnlineGameCharacter::ServerStartAttacking_Implementation()
{
	MulticastStartAttacking();
}

bool AOnlineGameCharacter::ServerStartAttacking_Validate()
{
	return true;
}
void AOnlineGameCharacter::BeginContinuousAttack()
{
	if (HasAuthority())
	{
		MulticastStartAttacking();
	}
	else
	{
		ServerStartAttacking();
	}
}
void AOnlineGameCharacter::StartAttacking()
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
			float Duration = Anim->GetPlayLength() / AttackSpeedMultiplier;
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
			UWorld* const World = GetWorld();
			if (World == nullptr) return;
			// Shoot a raycast, slightly ahead of your position
			// To determine if you have hit anything - Cast to Enemy etc
			float Duration = PlayAnimMontage(Anim, 1.0f * AttackSpeedMultiplier, NAME_None);
			FHitResult Hit = ShootRay();
			// Test
			AEnemyAI* EnemyHit = Cast<AEnemyAI>(Hit.GetActor());
			if (EnemyHit != nullptr)
			{
				// Do normal Damage
				DealDamage(EnemyHit);
				//EnemyHit->TakeDamages(50.0f);
			}
			else
			{
				FTimerHandle ProjectileTimerHandle;
				World->GetTimerManager().SetTimer(ProjectileTimerHandle, this, &AOnlineGameCharacter::SpawnProjectile, Duration / (2 * AttackSpeedMultiplier));

			}
			// End Test


			// Toggle attack buffer on, and start timer to end it
			bAttackBufferActive = true;
			World->GetTimerManager().SetTimer(BufferAttackHandle, this, &AOnlineGameCharacter::EndAttackBuffer, Duration / AttackSpeedMultiplier);
		}
	}
}

void AOnlineGameCharacter::SpawnProjectile()
{
	UWorld* const World = GetWorld();
	if (World == nullptr) return;
	//UE_LOG(LogTemp, Warning, TEXT("Proj Attempted to Fire: OnlineGameCharacter"));
	if (WeaponProjectileClass == nullptr) return;
	AWeaponProjectile* WeaponProj = World->SpawnActor<AWeaponProjectile>(WeaponProjectileClass, GetActorLocation() + (GetActorForwardVector() * ForwardOffset), GetActorRotation());
	if (WeaponProj != nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Proj Fire Successful: OnlineGameCharacter"));
		// For time being should just see weapon firing visually
		WeaponProj->OnEnemyHit.AddDynamic(this, &AOnlineGameCharacter::DealDamage);
		//WeaponProj->OnEnemyHit.BindUF
		//WeaponProj.OnEnemyHit
		// Bind Delegate here to deal damage to Enemy
	}
}

void AOnlineGameCharacter::DealDamage(AActor* _Enemy)
{
	AEnemyAI* Enemy = Cast<AEnemyAI>(_Enemy);
	if (Enemy != nullptr)
	{
		float Damage = 0.0f;
		//UE_LOG(LogTemp, Warning, TEXT("Damage Before Load: %f"), Damage);
		APlayerController* PC = Cast<APlayerController>(Controller);
		AOnlineGamePlayerController* MyPC = Cast<AOnlineGamePlayerController>(PC);
		AOnlineGameLobbyPlayerController* LobbyPC = Cast<AOnlineGameLobbyPlayerController>(PC);
		if (PC != nullptr)
		{
			MyPC = Cast<AOnlineGamePlayerController>(PC);
			if (MyPC != nullptr)
			{
				FMyPlayerInfo MyData = MyPC->GetPlayerData();
				Damage = MyData.PlayerDamage;
				UE_LOG(LogTemp, Warning, TEXT("Damage After Load, in Game, : %f"), Damage);
			}
			else
			{
				LobbyPC = Cast <AOnlineGameLobbyPlayerController>(PC);
				if (LobbyPC != nullptr)
				{
					FMyPlayerInfo MyLobbyData = LobbyPC->GetPlayerSettings();
					Damage = MyLobbyData.PlayerDamage;
					UE_LOG(LogTemp, Warning, TEXT("Damage After Load , in Lobby, : %f"), Damage);
				}
			}
			if (Enemy->TakeDamages(Damage))
			{
				// Enemy was Slain
				// Gain Exp
				// For Test Just gonna add Damage
				if (LobbyPC != nullptr)
				{
					FMyPlayerInfo CurrentStats = LobbyPC->GetPlayerSettings();
					CurrentStats.PlayerDamage += 15.0f;
					LobbyPC->SetPlayerSettings(CurrentStats);
					UE_LOG(LogTemp, Warning, TEXT("Damage After Killing Enemy: %f"), CurrentStats.PlayerDamage);
				}
				if (MyPC != nullptr)
				{
					FMyPlayerInfo CurrentStats = MyPC->GetPlayerData();
					CurrentStats.PlayerDamage += 15.0f;
					MyPC->SetPlayerData(CurrentStats);
					UE_LOG(LogTemp, Warning, TEXT("Damage After Killing Enemy: %f"), CurrentStats.PlayerDamage);
				}
			}
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

void AOnlineGameCharacter::MulticastAttackReset_Implementation()
{
	AttackReset();
}

void AOnlineGameCharacter::ServerAttackReset_Implementation()
{
	MulticastAttackReset();
}

bool AOnlineGameCharacter::ServerAttackReset_Validate()
{
	return true;
}
void AOnlineGameCharacter::ResetAttack()
{
	if (HasAuthority())
	{
		MulticastAttackReset();
	}
	else
	{
		ServerAttackReset();
	}
}

void AOnlineGameCharacter::AttackReset()
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
	//UE_LOG(LogTemp, Warning, TEXT("TurnAtRate: %f"), Rate);
	// calculate delta for this frame from the rate information
	if (bAttackBufferActive && FMath::Abs(Rate) > 0.0f)
	{
		FRotator NewRot = FollowCamera->GetComponentRotation();
		GetCapsuleComponent()->SetRelativeRotation(FRotator(0.0f, NewRot.Yaw, 0.0f));
	}
	// In Theory this would be nice, but it actually makes it worse
	//if (bAttackBufferActive && FMath::Abs(Rate) > 0.0f)
	//{
	//	float MyRotation = GetCapsuleComponent()->GetComponentRotation().Yaw;
	//	float NewRot = FollowCamera->GetComponentRotation().Yaw;
	//	float RotateRate = 500.0f;
	//	MyRotation = FMath::FInterpConstantTo(MyRotation, NewRot, GetWorld()->GetDeltaSeconds(), RotateRate);
	//	GetCapsuleComponent()->SetRelativeRotation(FRotator(0.0f, MyRotation, 0.0f));
	//}

	UWorld* const World = GetWorld();
	if (World == nullptr) return;
	AddControllerYawInput(Rate * BaseTurnRate * World->GetDeltaSeconds());
}
void AOnlineGameCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AOnlineGameCharacter::MouseTurn(float Rate)
{
	if (bAttackBufferActive && FMath::Abs(Rate) > 0.0f)
	{
		FRotator NewRot = FollowCamera->GetComponentRotation();
		GetCapsuleComponent()->SetRelativeRotation(FRotator(0.0f, NewRot.Yaw, 0.0f));
	}
	if (Rate != 0.f && Controller && Controller->IsLocalPlayerController())
	{
		APlayerController* const PC = CastChecked<APlayerController>(Controller);
		PC->AddYawInput(Rate);
	}
}

void AOnlineGameCharacter::MouseLook(float Rate)
{
	if (Rate != 0.f && Controller && Controller->IsLocalPlayerController())
	{
		APlayerController* const PC = CastChecked<APlayerController>(Controller);
		PC->AddPitchInput(Rate);
	}
}

// Moving Around
void AOnlineGameCharacter::MoveForward(float Value)
{
	// Cant move while attacking.
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
