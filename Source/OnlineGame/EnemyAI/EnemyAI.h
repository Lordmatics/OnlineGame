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

	UPROPERTY(Category = Character, VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Replicated)
		UTextRenderComponent* HealthText;

public:
	// Sets default values for this character's properties
	AEnemyAI();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* _InputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Attack();

	//UFUNCTION(NetMulticast, Reliable)
	//	void MulticastTakeDamages(float DamageIn);
	//virtual void MulticastTakeDamages_Implementation(float DamageIn);
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerTakeDamages(float DamageIn);
	virtual void ServerTakeDamages_Implementation(float DamageIn);
	virtual bool ServerTakeDamages_Validate(float DamageIn);
	//UFUNCTION()
	//	void CommitDamage(float DamageIn);
	UFUNCTION()
		bool TakeDamages(float DamageIn);

	void Die();

	UPROPERTY(Category = Character, EditAnywhere, BlueprintReadWrite)
		class UBehaviorTree* BehaviorTree;
	
	UPROPERTY(EditAnywhere, Category = "Delegate")
		FOnEnemyDestroyed OnEnemyDestroyed;

	UPROPERTY(EditAnywhere, Category = "Health", Replicated)
		float EnemyHealth = 100.0f;
};
