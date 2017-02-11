// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "FindWaypointTask.generated.h"

/**
 * 
 */
UCLASS()
class ONLINEGAME_API UFindWaypointTask : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

private:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	
	
};
