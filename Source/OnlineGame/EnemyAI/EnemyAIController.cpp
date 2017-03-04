// Fill out your copyright notice in the Description page of Project Settings.

#include "OnlineGame.h"
#include "EnemyAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyAI/EnemyAI.h"
#include "OnlineGameGameMode.h"

AEnemyAIController::AEnemyAIController()
{
	PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SenseConfig"));
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	BehaviourTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviourTree"));
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
	PerceptionComp->ConfigureSense(*SightConfig);
	PerceptionComp->ConfigureSense(*HearingConfig);
	PerceptionComp->SetDominantSense(SightConfig->GetSenseImplementation());
	PerceptionComp->OnPerceptionUpdated.AddDynamic(this, &AEnemyAIController::UpdatePerception);
	
}

void AEnemyAIController::UpdatePerception(TArray<AActor*>ActorsInSight)
{
	//UE_LOG(LogTemp, Warning, TEXT("I see you"));
	UWorld* const World = GetWorld();
	if (World == nullptr) return;
	AOnlineGameGameMode* GM = Cast<AOnlineGameGameMode>(World->GetAuthGameMode());
	if (GM != nullptr)
	{
		for (size_t i = 0; i < GM->PlayerControllers.Num(); i++)
		{
			APawn* TempCharacter = GM->PlayerControllers[i]->GetPawn();//World->GetFirstPlayerController()->GetPawn();
			if (TempCharacter != nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("EnemyAIController: UpdatePerception: TempCharacterName: %s"), *TempCharacter->GetName());
				for (int j = 0; j < ActorsInSight.Num(); j++)
				{
					if (ActorsInSight[i] == TempCharacter)
					{
						// Maybe some check in here to see if value already set, then compare chase target
						// Since multiplayer
						BlackboardComponent->SetValueAsObject("AttackTarget", TempCharacter);
					}
				}
			}
		}
	}
}

void AEnemyAIController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);
	AEnemyAI* AICharacter = Cast<AEnemyAI>(InPawn);
	if (AICharacter != nullptr)
	{
		if (AICharacter->BehaviorTree != nullptr)
		{
			BlackboardComponent->InitializeBlackboard(*AICharacter->BehaviorTree->BlackboardAsset);
			BehaviourTreeComponent->StartTree(*AICharacter->BehaviorTree);
		}
	}
}
