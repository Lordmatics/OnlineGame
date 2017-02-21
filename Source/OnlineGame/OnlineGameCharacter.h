// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "OnlineGameCharacter.generated.h"


UCLASS(config=Game)
class AOnlineGameCharacter : public ACharacter
{
	GENERATED_BODY()
private:

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleDefaultsOnly, Category = "C++ Weapon")
		USkeletalMeshComponent* Weapon;

	UPROPERTY(EditDefaultsOnly, Category = "C++ Weapon")
		class UOnlineGameWeapon* WeaponComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = "C++ Animations")
		class UAnimationComponent* AnimationStorage;

	UPROPERTY(VisibleDefaultsOnly, Category = "C++ Weapon")
		class URaycastComponent* RaycastComponent;

	// LOGIC FOR DOING ATTACKS - FINISHED AND SMOOTH
	UPROPERTY(EditDefaultsOnly, Category = "C++ Weapon")
		TSubclassOf<class AWeaponProjectile> WeaponProjectileClass;

	UPROPERTY(VisibleAnywhere, Category = "C++ Variables")
		uint32 bAttackBufferActive : 1;

	UPROPERTY(VisibleAnywhere, Category = "C++ Variables")
		uint32 bAttackPressedCached : 1;

	UPROPERTY(EditAnywhere, Category = "C++ Variables")
		float ForwardOffset = 100.0f;
	
	UPROPERTY(EditAnywhere, Category = "C++ Variables")
		float AttackSpeedMultiplier = 2.0f;

	FTimerHandle BeginAttackHandle;
	FTimerHandle AttackHandle;
	FTimerHandle BufferAttackHandle;
	// END LOGIC FOR DOING ATTACKS - FINISHED AND SMOOTH

	// DEATH VARIABLES
	///** Replicated version of relative movement. Read-only on simulated proxies! */
	///UPROPERTY(ReplicatedUsing = OnRep_ReplicatedBasedMovement)
	UPROPERTY(VisibleAnywhere, Category = "C++ Variables")
		uint32 bIsDead : 1;
	/** Rep notify for ReplicatedBasedMovement */
	///UFUNCTION()
		///virtual void OnRep_ReplicatedBasedMovement();
	// END OF DEATH VARIABLES

private:
	// My Functions
	// DAMAGE ORIENTED FUNCTIONS
	UFUNCTION()
		void DealDamage(AActor* _Enemy);
	// END OF DAMAGE FUNCTIONS
	// SHOOTING FUNCTIONS
	UFUNCTION()
		void BeginContinuousAttack();
	UFUNCTION()
		void StartAttacking();
	// Function that runs PlayerDied on all clients / server
	UFUNCTION(NetMulticast, Reliable)
		void MulticastStartAttacking();
	virtual void MulticastStartAttacking_Implementation();
	// Same as above ... idk
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerStartAttacking();
	virtual void ServerStartAttacking_Implementation();
	virtual bool ServerStartAttacking_Validate();
	UFUNCTION()
		void Attack();
	UFUNCTION()
		void SpawnProjectile();
	UFUNCTION()
		void EndAttackBuffer();
	UFUNCTION()
		void ResetAttack();
	UFUNCTION()
		void AttackReset();
	// Function that runs PlayerDied on all clients / server
	UFUNCTION(NetMulticast, Reliable)
		void MulticastAttackReset();
	virtual void MulticastAttackReset_Implementation();
	// Same as above ... idk
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerAttackReset();
	virtual void ServerAttackReset_Implementation();
	virtual bool ServerAttackReset_Validate();
	FHitResult ShootRay();
	// END SHOOTING FUNCTIONS

	// DEATH FUNCTIONS
	// Function that starts the death
	UFUNCTION()
		void Die();
	// Function that begins Death animation - Logic
	UFUNCTION()
		void PlayerDied();
	// Function that runs PlayerDied on all clients / server
	UFUNCTION(NetMulticast, Reliable)
		void MulticastPlayerDied();
	virtual void MulticastPlayerDied_Implementation();
	// Same as above ... idk
	UFUNCTION(Server,Reliable, WithValidation)
		void ServerPlayerDied();
	virtual void ServerPlayerDied_Implementation();
	virtual bool ServerPlayerDied_Validate();
	// Resultant function of death animation
	UFUNCTION()
		void OnDied();
	// END OF DEATH FUNCTIONS
public:
	AOnlineGameCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:
	
	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	// MOUSE TURNS
	UFUNCTION()
		void MouseTurn(float Rate);

	UFUNCTION()
		void MouseLook(float Rate);
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	virtual void PostInitializeComponents() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

