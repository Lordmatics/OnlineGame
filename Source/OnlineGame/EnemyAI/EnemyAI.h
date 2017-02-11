// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "EnemyAI.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDestroyed, AEnemyAI*, _Minion);

UCLASS()
class ONLINEGAME_API AEnemyAI : public ACharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(Category = Character, VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* BoxCollision;
public:
	// Sets default values for this character's properties
	AEnemyAI();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	virtual void Attack();

	void TakeDamages(float DamageIn);

	void Die();

	UPROPERTY(Category = Character, EditAnywhere, BlueprintReadWrite)
		class UBehaviorTree* BehaviorTree;
	
	UPROPERTY(EditAnywhere, Category = "Delegate")
		FOnEnemyDestroyed OnEnemyDestroyed;
};
