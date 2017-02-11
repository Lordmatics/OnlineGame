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
	APawn* TempCharacter = World->GetFirstPlayerController()->GetPawn();
	if (TempCharacter != nullptr)
	{
		for (int i = 0; i < ActorsInSight.Num(); i++)
		{
			if (ActorsInSight[i] == TempCharacter)
			{
				BlackboardComponent->SetValueAsObject("AttackTarget", TempCharacter);
			}
		}
	}
}

void AEnemyAIController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);
	//AQuackAIPawn* AICharacter = Cast<AQuackAIPawn>(InPawn);
	//if (AICharacter != nullptr)
	//{
	//	if (AICharacter->BehaviorTree != nullptr)
	//	{
	//		BlackboardComponent->InitializeBlackboard(*AICharacter->BehaviorTree->BlackboardAsset);
	//		BehaviourTreeComponent->StartTree(*AICharacter->BehaviorTree);
	//	}
	//}
}
