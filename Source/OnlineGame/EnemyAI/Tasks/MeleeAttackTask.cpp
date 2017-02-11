// Fill out your copyright notice in the Description page of Project Settings.

#include "OnlineGame.h"
#include "MeleeAttackTask.h"
#include "EnemyAI/EnemyAI.h"
#include "EnemyAI/EnemyAIController.h"

EBTNodeResult::Type UMeleeAttackTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyAIController* MyController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	if (MyController == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	AEnemyAI* TempPawn = Cast<AEnemyAI>(MyController->GetPawn());
	if (TempPawn != nullptr)
	{
		TempPawn->Attack();
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
