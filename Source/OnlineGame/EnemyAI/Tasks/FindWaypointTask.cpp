// Fill out your copyright notice in the Description page of Project Settings.

#include "OnlineGame.h"
#include "FindWaypointTask.h"

#include "EnemyAI/EnemyAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UFindWaypointTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyAIController* MyController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	if (MyController == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	const float SearchRadius = 2000.0f;
	const FVector SearchOrigin = MyController->GetPawn()->GetActorLocation();
	const FVector Loc = UNavigationSystem::GetRandomPointInNavigableRadius(MyController, SearchOrigin, SearchRadius);
	if (Loc != FVector::ZeroVector)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector("MoveLocation", Loc);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}




