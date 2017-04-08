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
#include "Barrels/Barrels.h"
#include "Pickups/Powerups/Powerups.h"
#include "EnemyAI/Spawning/EnemyGate.h"
#include "DrawDebugHelpers.h"

//////////////////////////////////////////////////////////////////////////
// AOnlineGameCharacter

void AOnlineGameCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AOnlineGameCharacter, KeyCount);

	DOREPLIFETIME(AOnlineGameCharacter, GoldCount);

	DOREPLIFETIME(AOnlineGameCharacter, PotionCount);

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

	//GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AOnlineGameCharacter::OnTriggerEnter);
	PotionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PotionSphere"));
	PotionSphere->SetupAttachment(GetCapsuleComponent());
	PotionSphere->bGenerateOverlapEvents = true;
	PotionSphere->OnComponentBeginOverlap.AddDynamic(this, &AOnlineGameCharacter::OnTriggerEnter);

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
	
	PotionParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("PotionParticleComp"));
	PotionParticleComp->SetupAttachment(GetCapsuleComponent());

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

	PlayerInputComponent->BindAction("UsePower", IE_Pressed, this, &AOnlineGameCharacter::UseSelectedPower);
	PlayerInputComponent->BindAction("ConsumePotion", IE_Pressed, this, &AOnlineGameCharacter::ActivatePotion);

	PlayerInputComponent->BindAction("UseTurbo", IE_Pressed, this, &AOnlineGameCharacter::UseTurboAttack);


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

void AOnlineGameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ChargePowerUpBar(DeltaTime);
}

void AOnlineGameCharacter::UseTurboAttack()
{
	if (TurboBoost <= MaxTurboBoost * 0.33f) return;

	FVector NewScale;
	float Radius = 100.0f;
	float Damage = 25.0f;

	// Full
	if (TurboBoost >= MaxTurboBoost)
	{
		// In Red
		TurboBoost -= MaxTurboBoost;
		UE_LOG(LogTemp, Warning, TEXT("Turboed - Red"));
		NewScale = FVector(3.0f);
		Radius = 500.0f;
		Damage = 100.0f;
	}
	// Else > 2/3
	else if (TurboBoost > MaxTurboBoost * 0.66f)
	{
		TurboBoost -= MaxTurboBoost * 0.66f;
		UE_LOG(LogTemp, Warning, TEXT("Turboed - Yellow"));
		NewScale = FVector(1.3f);
		Radius = 300.0f;
		Damage = 50.0f;
	}
	// > 1/3
	else if (TurboBoost > MaxTurboBoost* 0.33f)
	{
		TurboBoost -= MaxTurboBoost * 0.33f;
		UE_LOG(LogTemp, Warning, TEXT("Turboed - Green"));
		NewScale = FVector(0.6f);
		Radius = 200.0f;
		Damage = 25.0f;
	}
	if (TurboPS != nullptr)
	{
		UParticleSystemComponent* PSC = UGameplayStatics::SpawnEmitterAtLocation(this, TurboPS, GetActorLocation());
		if (PSC != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("ParticleSystem Made"));
			UWorld* const World = GetWorld();
			if (World == nullptr) return;
			PSC->SetWorldScale3D(NewScale);
			FVector NewLocation = URaycastComponent::RayDown(World, PSC->GetComponentLocation()).Location;
			PSC->SetWorldLocation(NewLocation, true);
			UE_LOG(LogTemp, Warning, TEXT("snapped to ground"));

			AActor* ActorToIgnore = this;
			FVector Start = GetActorLocation();
			FVector End = GetActorLocation() + GetActorForwardVector() * 1;
			TArray<FHitResult> HitOut;			
			//ECollisionChannel EnemyChannel = ECC_GameTraceChannel4;

			//ECollisionChannel BarrelChannel = ECC_GameTraceChannel2;

			FCollisionQueryParams CQP;
			CQP.bTraceComplex = true;
			//TraceParams.bTraceAsyncScene = true;
			CQP.bReturnPhysicalMaterial = false;
			CQP.bTraceAsyncScene = true;
			//Ignore Actors
			//CQP.AddIgnoredActor(ActorToIgnore);
			//Re-initialize hit info
			//HitOut = FHitResult(ForceInit);
			//UKismetSystemLibrary::SphereTraceMulti_NEW(this, Start, End, Radius, ETraceTypeQuery::TraceTypeQuery1, false, IgnoredActors, HitOut, true);
			
		    //World->SweepMultiByChannel(HitOut, Start, End, FQuat(), ECC_Visibility, FCollisionShape::MakeSphere(Radius), CQP);
			if (World->SweepMultiByChannel(HitOut, Start, End, FQuat(), ECC_Visibility, FCollisionShape::MakeSphere(Radius), CQP))
			{
				UE_LOG(LogTemp, Warning, TEXT("%d"), HitOut.Num());

				//UE_LOG(LogTemp, Warning, TEXT("%d"), HitOut.Num());
				//UE_LOG(LogTemp, Warning, TEXT("Sweep Multi By Channel Made"));
				//if (DrawDebugType != EDrawDebugTrace::None)
				//{
				//bool bPersistent = true;//DrawDebugType == EDrawDebugTrace::Persistent;
				//	//float LifeTime = (DrawDebugType == EDrawDebugTrace::ForDuration) ? UKismetSystemLibrary::KISMET_TRACE_DEBUG_DRAW_DURATION : 0.f;

				//	if (HitOut.Last().bBlockingHit)
				//	{
				//		UE_LOG(LogTemp, Warning, TEXT("Draw Sphere Pls"));
				//		// Red up to the blocking hit, green thereafter
				//		FVector const BlockingHitPoint = HitOut.Last().Location;
				//		DrawDebugSphere(World, Start, Radius, 15.0f, FColor::Green, bPersistent);
				//		//::DrawDebugSweptSphere(World, Start, BlockingHitPoint, Radius, FColor::Red, bPersistent);
				//		
				//		//UKismetSystemLibrary::DrawDebugSweptSphere(World, BlockingHitPoint, End, Radius, FColor::Green, bPersistent);
				//	}
				//	else
				//	{
				//		// no hit means all red
				//		//UKismetSystemLibrary::DrawDebugSweptSphere(World, Start, End, Radius, FColor::Red, bPersistent);
				//	}

				//	// draw hits
				//	for (int32 HitIdx = 0; HitIdx<HitOut.Num(); ++HitIdx)
				//	{
				//		FHitResult const& Hit = HitOut[HitIdx];
				//		::DrawDebugPoint(World, Hit.ImpactPoint, 50.0f, (Hit.bBlockingHit ? FColor::Red : FColor::Green), bPersistent);
				//	}
				
				//World->SweepMultiByChannel(OutHits, Start, End, FQuat::Identity, TraceChannel, FCollisionShape::MakeSphere(Radius), Params);
				for (size_t i = 0; i < HitOut.Num(); i++)
				{
					AActor* HitActor = HitOut[i].GetActor();
					if (HitActor == nullptr) continue;
					AEnemyAI* Enemy = Cast<AEnemyAI>(HitActor);
					if (Enemy == nullptr) continue;
					Enemy->TakeDamages(Damage);
					ABarrels* Barrel = Cast<ABarrels>(HitActor);
					if (Barrel == nullptr) continue;
					Barrel->Fracture();
					//UE_LOG(LogTemp, Warning, TEXT("Something AOE Hit: %s"), *HitActor->GetName());
				}
			}
			//if(UKismetSystemLibrary::SphereTraceMulti_NEW(this, GetActorLocation(), GetActorLocation(), 5000.0f, ECC_Visibility, true, IgnoredActors, EDrawDebugTrace::Persistent, ThingsHit))
			//{

			//}
			
		}
	}
}

void AOnlineGameCharacter::ChargePowerUpBar(float DeltaTime)
{
	TurboBoost += DeltaTime * ChargeRate;
	TurboBoost = FMath::Clamp(TurboBoost, 0.0f, MaxTurboBoost);
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

void AOnlineGameCharacter::ObtainPower(APowerups* OverlappedPower)
{
	if (OverlappedPower != nullptr)
	{
		CurrentPowerUp = OverlappedPower;
	}
}

void AOnlineGameCharacter::UseSelectedPower()
{
	if (CurrentPowerUp != nullptr)
	{
		if (CurrentPowerUp->GetPowerCharges() > 0)
		{
			CurrentPowerUp->UsePower();
			// If you use last charge, set it to null immediately
			// as opposed to wait for you to try and use it 
			// after it has expired
			if (CurrentPowerUp->GetPowerCharges() == 1)
			{
				CurrentPowerUp = nullptr;
			}
		}
		// Backup, in case i implement something that consumes multiple
		// charges
		else
		{
			CurrentPowerUp = nullptr;
		}
	}
}

void AOnlineGameCharacter::ActivatePotion()
{
	UWorld* const World = GetWorld();
	if (World == nullptr) return;
	if (HasPotions())
	{
		if (PotionShield != nullptr)
		{
			if (/*PotionParticleComp == nullptr &&*/ !bPotionActive)
			{
				UsePotion();
				UE_LOG(LogTemp, Warning, TEXT("UsedPotion"));
				bPotionActive = true;
				PotionParticleComp = UGameplayStatics::SpawnEmitterAttached(PotionShield, GetMesh(), FName("AttachPoint"));
				FTimerHandle TempHandle;
				World->GetTimerManager().SetTimer(TempHandle, this, &AOnlineGameCharacter::DeactivatePotion, PotionDuration, false);
			}
		}
		
	}
}

void AOnlineGameCharacter::DeactivatePotion()
{
	if (PotionParticleComp != nullptr)
	{
		PotionParticleComp->DeactivateSystem();
		PotionParticleComp = nullptr;
		bPotionActive = false;
	}
}

void AOnlineGameCharacter::OnTriggerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEnemyAI* Enemy = Cast<AEnemyAI>(OtherActor);
	if (Enemy != nullptr)
	{
		if (bPotionActive)
		{
			if (Enemy->TakeDamages(Enemy->EnemyHealth))
			{
				UE_LOG(LogTemp, Warning, TEXT("Enemy Vaporised"));
				// Play Sound? 
				// Play Vaproise Particle if I can find one
			}
		}
	}
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
			ABarrels* Barrel = Cast<ABarrels>(Hit.GetActor());
			AEnemyGate* Gate = Cast<AEnemyGate>(Hit.GetActor());
			if (EnemyHit != nullptr)
			{
				// Do normal Damage
				DealDamage(EnemyHit);
				//EnemyHit->TakeDamages(50.0f);
			}
			else if (Barrel != nullptr)
			{
				// Deal Damage, will know what logic to run
				// Based on type
				DealDamage(Barrel);
				//Barrel->Fracture();
			}
			else if (Gate != nullptr)
			{
				DealDamage(Gate);
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
		if (Enemy->IsDead()) return;
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
	ABarrels* Barrel = Cast<ABarrels>(_Enemy);
	if (Barrel != nullptr)
	{
		Barrel->Fracture();
	}

	AEnemyGate* Gate = Cast<AEnemyGate>(_Enemy);
	if (Gate != nullptr)
	{
		Gate->TakeDamages();
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
	//if (bAttackBufferActive) return;
	// TEST - If attacking ,can move at half speed
	if (bAttackBufferActive)
	{
		if ((Controller != NULL) && (Value != 0.0f))
		{
			// find out which way is forward
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get forward vector
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			AddMovementInput(Direction, Value / 2);
		}
	}
	else
	{
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
}
void AOnlineGameCharacter::MoveRight(float Value)
{
	// Can't move while attacking
	//if (bAttackBufferActive) return;
	if (bAttackBufferActive)
	{
		if ((Controller != NULL) && (Value != 0.0f))
		{
			// find out which way is right
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get right vector 
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			// add movement in that direction
			AddMovementInput(Direction, Value / 2);
		}
	}
	else
	{
		if ((Controller != NULL) && (Value != 0.0f))
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
}
