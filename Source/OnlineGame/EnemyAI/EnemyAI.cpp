// Fill out your copyright notice in the Description page of Project Settings.

#include "OnlineGame.h"
#include "EnemyAI.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "OnlineGameCharacter.h"
#include "EnemyAI/EnemyAIController.h"
#include "Animation/AnimationComponent.h"
#include "WeaponComponents/RaycastComponent.h"

// Sets default values
AEnemyAI::AEnemyAI()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(GetCapsuleComponent());

	//HealthText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("HealthText"));
	//HealthText->SetIsReplicated(true);
	//HealthText->SetupAttachment(GetCapsuleComponent());

	GetCharacterMovement()->bUseRVOAvoidance = true;

	AnimationComponent = CreateDefaultSubobject<UAnimationComponent>(TEXT("AnimationComponent"));
	RaycastComponent = CreateDefaultSubobject<URaycastComponent>(TEXT("RaycastComponent"));

}

void AEnemyAI::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AEnemyAI, EnemyHealth);
	DOREPLIFETIME(AEnemyAI, MaxEnemyHealth);

	//DOREPLIFETIME(AEnemyAI, HealthText);
}

// Called when the game starts or when spawned
void AEnemyAI::BeginPlay()
{
	Super::BeginPlay();
	DissolveMat = GetMesh()->CreateDynamicMaterialInstance(0);
	GetMesh()->SetMaterial(0, DissolveMat);

	MaxEnemyHealth = EnemyHealth;
	//UE_LOG(LogTemp, Warning, TEXT("EnemyHealth: %d"), (int)EnemyHealth);

}

// Called every frame
void AEnemyAI::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	//if (HealthText != nullptr)
	//{
	//	//FString HealthString = FString::Printf(TEXT("Health: %d"), (int)EnemyHealth);
	//	//HealthText->Text = FText::FromString(HealthString);
	//	HealthText->SetText(FString::FromInt((int)EnemyHealth));
	//}
	if (bIsDead)
	{
		//UE_LOG(LogTemp, Warning, TEXT("ISDEAD"));
		if (DissolveMat != nullptr)
		{
			Alpha -= DeltaTime * DissolveRate;
			//UE_LOG(LogTemp, Warning, TEXT("Alpha Going Down: %f"), Alpha);

			
			DissolveMat->SetScalarParameterValue(TEXT("Dissolve"), Alpha);
			if (Alpha <= 0.0f)
			{
				Destroy();
			}
		}
	}
}

// Called to bind functionality to input
void AEnemyAI::SetupPlayerInputComponent(class UInputComponent* _InputComponent)
{
	Super::SetupPlayerInputComponent(_InputComponent);

}

void AEnemyAI::Attack()
{
	UWorld* const World = GetWorld();
	if (World == nullptr) return;
	AOnlineGameCharacter* PlayerCharacter = Cast<AOnlineGameCharacter>(World->GetFirstPlayerController()->GetCharacter());
	if (PlayerCharacter != nullptr)
	{
		if (AnimationComponent != nullptr)
		{
			int Rand = FMath::RandHelper(2);
			float AnimDuration = 0.0f;
			FTimerHandle TempHandle;
			if (Rand == 0)
			{
				if (AnimationComponent->GetAttackAnimMontage() != nullptr)
				{
					AnimDuration = PlayAnimMontage(AnimationComponent->GetAttackAnimMontage(), AnimSpeed);
					World->GetTimerManager().SetTimer(TempHandle, this, &AEnemyAI::DoDamage, 0.1f, false);
				}
			}
			else
			{
				if (AnimationComponent->GetAltAttackAnimMontage() != nullptr)
				{
					AnimDuration = PlayAnimMontage(AnimationComponent->GetAltAttackAnimMontage(), AnimSpeed);
					World->GetTimerManager().SetTimer(TempHandle, this, &AEnemyAI::DoDamage, 0.1f, false);
				}
			}
		}
	}
}

void AEnemyAI::DoDamage()
{
	if (RaycastComponent != nullptr)
	{
		// Fire a raycast through the component
		// And return what it hits
		FVector Direction = GetActorForwardVector();
		TArray<FHitResult> Hits =  RaycastComponent->RaycastTP(GetMesh(), Direction, 100.0f, 125.0f);	
		if (Hits.Num() == 0) return;
		for (FHitResult Hit : Hits)
		{
			AOnlineGameCharacter* Ally = Cast<AOnlineGameCharacter>(Hit.GetActor());
			if (Ally != nullptr)
			{
				Ally->DecreaseHealth(EnemyDamage);
			}
		}
	}
}

void AEnemyAI::TakeDamageOverTime(float DPS)
{
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		FTimerDelegate TimerDel;
		TimerDel.BindUFunction(this, FName("TakeDamagesNoRet"), DPS / 2);
		World->GetTimerManager().SetTimer(DOTHandle, TimerDel, 0.5f, true);
	}
}

void AEnemyAI::ClearDOT()
{
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		World->GetTimerManager().ClearTimer(DOTHandle);
	}
}

void AEnemyAI::TakeDamagesNoRet(float DamageIn)
{
	if (Role < ROLE_Authority)
	{
		ServerTakeDamages(DamageIn);
	}
	else
	{
		EnemyHealth -= DamageIn;
		//HealthText->Text = FText::FromString(FString::Printf(TEXT("Health: %f"), EnemyHealth));
		//UE_LOG(LogTemp, Warning, TEXT("EnemyHealth: %f"), EnemyHealth);
		if (EnemyHealth <= 0.0f)
		{
			Die();
		}
	}
}

bool AEnemyAI::TakeDamages(float DamageIn)
{

	if (Role < ROLE_Authority)
	{
		ServerTakeDamages(DamageIn);
	}
	else
	{
		EnemyHealth -= DamageIn;
		//HealthText->Text = FText::FromString(FString::Printf(TEXT("Health: %f"), EnemyHealth));
		//UE_LOG(LogTemp, Warning, TEXT("EnemyHealth: %f"), EnemyHealth);
		if (EnemyHealth <= 0.0f && !bIgnoreRays)
		{
			Die();
			return true;
		}
	}

	return false;
	//if (HasAuthority())
	//{
	//	MulticastTakeDamages(DamageIn);
	//}
	//else
	//{
	//	ServerTakeDamages(DamageIn);
	//}

}

void AEnemyAI::ServerTakeDamages_Implementation(float DamageIn)
{
	TakeDamages(DamageIn);
}

bool AEnemyAI::ServerTakeDamages_Validate(float DamageIn)
{
	return true;
}
void AEnemyAI::Die()
{
	bIgnoreRays = true;
	if (OnEnemyDestroyed.IsBound())
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy Has Started Broadcasting"));
		OnEnemyDestroyed.Broadcast(this);
	}
	//GetMesh()->SetVisibility(false);
	AEnemyAIController* TempController = Cast<AEnemyAIController>(GetController());
	if (TempController != nullptr)
	{
		//TempController->GetBrainComponent()->StopLogic(TEXT("Dead"));
		TempController->BlackboardComponent->SetValueAsBool("Dead", true);
	}
	UWorld* const World = GetWorld();
	if (World == nullptr || AnimationComponent == nullptr)
	{
		//Destroy(); 
		GetMesh()->SetSimulatePhysics(true);
		return;
	}
	if (AnimationComponent->GetDeathAnimMontage() == nullptr)
	{
		//Destroy();
		GetMesh()->SetSimulatePhysics(true);
		return;
	}
//	if(BoxCollision != nullptr)
		//BoxCollision->SetCollisionResponseToChannels(ECR_Ignore);
	if (GetCapsuleComponent() != nullptr)
	{
		//GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		// Should be Projectile
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Ignore);
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_EngineTraceChannel1, ECR_Ignore);

	}
	if (GetMesh() != nullptr)
	{
		GetMesh()->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Ignore);
		GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
		GetMesh()->SetCollisionResponseToChannel(ECC_EngineTraceChannel1, ECR_Ignore);

	}
	//if(HealthText != nullptr)
		//HealthText->DestroyComponent();
	FTimerHandle TempHandle;
	// This doesnt replicate to other clients atm
	//if (HasAuthority())
	//{
		//float AnimDuration = PlayAnimMontage(AnimationComponent->GetDeathAnimMontage(), AnimSpeed);
		//World->GetTimerManager().SetTimer(TempHandle, this, &AEnemyAI::LatentDestroy, AnimDuration - 0.75f, false);
	//}
	//else
	//{
	//	ServerPlayDeathAnim();
	//}
	// It looks like this makes no sense
	// And  you could just use
	// MulticastPlayerDied
	// But i tested it, and
	// You do need the Server one apparently
		if (HasAuthority())
		{
			MulticastDeathAnim();
		}
		else
		{
			ServerDeathAnim();
		}
}

//void AEnemyAI::ServerPlayDeathAnim_Implementation()
//{
//	UWorld* const World = GetWorld();
//	if (World == nullptr) return;
//	FTimerHandle TempHandle;
//	float AnimDuration = PlayAnimMontage(AnimationComponent->GetDeathAnimMontage(), AnimSpeed);
//	World->GetTimerManager().SetTimer(TempHandle, this, &AEnemyAI::LatentDestroy, AnimDuration - 0.75f, false);
//}

//bool AEnemyAI::ServerPlayDeathAnim_Validate()
//{
//	return true;
//}

void AEnemyAI::LatentDestroy()
{
	//Destroy();
	GetMesh()->SetSimulatePhysics(true);
	bIsDead = true;
}

void AEnemyAI::MulticastDeathAnim_Implementation()
{
	DeathAnim();
}

void AEnemyAI::ServerDeathAnim_Implementation()
{
	MulticastDeathAnim();
}

bool AEnemyAI::ServerDeathAnim_Validate()
{
	return true;
}

void AEnemyAI::DeathAnim()
{
	UWorld* const World = GetWorld();
	if (World == nullptr) return;
	FTimerHandle TempHandle;
	float AnimDuration = PlayAnimMontage(AnimationComponent->GetDeathAnimMontage(), AnimSpeed);
	World->GetTimerManager().SetTimer(TempHandle, this, &AEnemyAI::LatentDestroy, AnimDuration - 0.75f, false);
}