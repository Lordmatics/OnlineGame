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
void AEnemyAI::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

void AEnemyAI::Attack()
{
	UE_LOG(LogTemp, Warning, TEXT("Get Attacked"));
	UWorld* const World = GetWorld();
	if (World == nullptr) return;
	AOnlineGameCharacter* PlayerCharacter = Cast<AOnlineGameCharacter>(World->GetFirstPlayerController()->GetCharacter());
	if (PlayerCharacter != nullptr)
	{
		//PlayerCharacter->GetCharacterMovement()->AddImpulse(GetActorLocation() - PlayerCharacter->GetActorLocation());
		//PlayerCharacter->DecreaseHealth(Damage);
		GetMesh()->SetVisibility(false);
		//do attack stuff
		AEnemyAIController* TempController = Cast<AEnemyAIController>(GetController());
		if (TempController != nullptr)
		{
			TempController->BlackboardComponent->SetValueAsBool("Dead", true);
			//TempController->GetBrainComponent()->StopLogic(TEXT("Dead"));
		}
		//Destroy();	
	}
}

void AEnemyAI::TakeDamages(float DamageIn)
{
	//CurrentHealth -= DamageIn;
	//if (CurrentHealth <= 0)
	//{
	//	Die();
	//}
}

void AEnemyAI::Die()
{
	GetMesh()->SetVisibility(false);
	AEnemyAIController* TempController = Cast<AEnemyAIController>(GetController());
	if (TempController != nullptr)
	{
		//TempController->GetBrainComponent()->StopLogic(TEXT("Dead"));
		TempController->BlackboardComponent->SetValueAsBool("Dead", true);
	}
}