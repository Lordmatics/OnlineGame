// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class ONLINEGAME_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
		AEnemyAIController();
public:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Enemy AI")
		UAIPerceptionComponent* PerceptionComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Enemy AI")
		class UAISenseConfig_Sight* SightConfig;
	
	UPROPERTY(VisibleDefaultsOnly, Category = "Enemy AI")
		class UAISenseConfig_Hearing* HearingConfig;
	
	UPROPERTY(VisibleDefaultsOnly, Category = "Enemy AI")
		UBehaviorTreeComponent* BehaviourTreeComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = "Enemy AI")
		UBlackboardComponent* BlackboardComponent;

	UFUNCTION()
		void UpdatePerception(TArray<AActor*>ActorsInSight);

	UFUNCTION()
		virtual void Possess(class APawn* InPawn) override;
	
	
};
