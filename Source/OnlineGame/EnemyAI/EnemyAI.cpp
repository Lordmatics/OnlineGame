// Fill out your copyright notice in the Description page of Project Settings.

#include "OnlineGame.h"
#include "EnemyAI.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "OnlineGameCharacter.h"
#include "EnemyAI/EnemyAIController.h"

// Sets default values
AEnemyAI::AEnemyAI()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(GetCapsuleComponent());

	GetCharacterMovement()->bUseRVOAvoidance = true;

}

// Called when the game starts or when spawned
void AEnemyAI::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyAI::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

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
		//UE_LOG(LogTemp, Warning, TEXT("EnemyAI: Attack: PlayerCharacterName: %s"), *PlayerCharacter->GetName());

		//PlayerCharacter->GetCharacterMovement()->AddImpulse(GetActorLocation() - PlayerCharacter->GetActorLocation());
		//PlayerCharacter->DecreaseHealth(Damage);
		GetMesh()->SetVisibility(false);
		//do attack stuff
		AEnemyAIController* TempController = Cast<AEnemyAIController>(GetController());
		if (TempController != nullptr)
		{
			//UE_LOG(LogTemp, Warning, TEXT("EnemyAI: Attack: TempController != nullptr"));
			//TempController->BlackboardComponent->SetValueAsBool("Dead", true);
		}
	}
}

//void AEnemyAI::MulticastTakeDamages_Implementation(float DamageIn)
//{
//	TakeDamages(DamageIn);
//}
//
//void AEnemyAI::ServerTakeDamages_Implementation(float DamageIn)
//{
//	MulticastTakeDamages(DamageIn);
//}
//
//bool AEnemyAI::ServerTakeDamages_Validate(float DamageIn)
//{
//	return true;
//}
//
//void AEnemyAI::CommitDamage(float DamageIn)
//{
//	EnemyHealth -= DamageIn;
//	UE_LOG(LogTemp, Warning, TEXT("EnemyHealth: %f"), EnemyHealth);
//	if (EnemyHealth <= 0.0f)
//	{
//		Die();
//	}
//}
bool AEnemyAI::TakeDamages(float DamageIn)
{
	EnemyHealth -= DamageIn;
	if (Role < ROLE_Authority)
	{
		ServerTakeDamages(DamageIn);
	}
	UE_LOG(LogTemp, Warning, TEXT("EnemyHealth: %f"), EnemyHealth);
	if (EnemyHealth <= 0.0f)
	{
		Die();
		return true;
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
	//GetMesh()->SetVisibility(false);
	AEnemyAIController* TempController = Cast<AEnemyAIController>(GetController());
	if (TempController != nullptr)
	{
		//TempController->GetBrainComponent()->StopLogic(TEXT("Dead"));
		TempController->BlackboardComponent->SetValueAsBool("Dead", true);
	}
	Destroy();
}