// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "OnlineGameCharacter.generated.h"


UCLASS(config=Game)
class AOnlineGameCharacter : public ACharacter
{
	GENERATED_BODY()
private:

	UPROPERTY(EditAnywhere, Category = "Ignored Actors")
		TArray<AActor*> IgnoredActors;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleDefaultsOnly, Category = "C++ Weapon")
		USkeletalMeshComponent* Weapon;

	UPROPERTY(VisibleDefaultsOnly, Category = "C++ Weapon")
		USphereComponent* PotionSphere;

	UPROPERTY(EditDefaultsOnly, Category = "C++ Weapon")
		class UOnlineGameWeapon* WeaponComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = "C++ Animations")
		class UAnimationComponent* AnimationStorage;

	UPROPERTY(VisibleDefaultsOnly, Category = "C++ Weapon")
		class URaycastComponent* RaycastComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = "C++ Potions")
		UParticleSystemComponent* PotionParticleComp;

	UPROPERTY(EditAnywhere, Category = "C++ Potions")
		UParticleSystem* PotionShield;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", meta = (AllowPrivateAccess = "true"), Replicated)
		int KeyCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", meta = (AllowPrivateAccess = "true"), Replicated)
		float GoldCount = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", meta = (AllowPrivateAccess = "true"), Replicated)
		int PotionCount = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", meta = (AllowPrivateAccess = "true"), Replicated)
		float Health = 500.0f;

	UPROPERTY(EditAnywhere, Category = "Shake")
		TSubclassOf<UCameraShake> HitShake;
private:
	// My Functions

	void UpdateStats();

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
	TArray<FHitResult> ShootRay();
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

	// POWER UPS
	UFUNCTION()
		void UseSelectedPower();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "C++ Power Ups")
		class APowerups* CurrentPowerUp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "C++ Power Ups")
		float ChargeRate = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "C++ Power Ups")
		float TurboBoost = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "C++ Power Ups")
		float MaxTurboBoost = 15.0f;

	UFUNCTION(NetMulticast, Reliable)
		void MulticastUseTurboAttack();
	virtual void MulticastUseTurboAttack_Implementation();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerUseTurboAttack();
	virtual void ServerUseTurboAttack_Implementation();
	virtual bool ServerUseTurboAttack_Validate();

	UFUNCTION()
		void InitiateTurbo();

	UPROPERTY(EditAnywhere, Category = "C++ Power Ups")
		UParticleSystem* TurboPS;
	UFUNCTION()
		void UseTurboAttack();
	void ChargePowerUpBar(float DeltaTime);
	// END of POWER UPS

	// POTIONS

	UFUNCTION(NetMulticast, Reliable)
		void MulticastActivatePotion();
	virtual void MulticastActivatePotion_Implementation();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerActivatePotion();
	virtual void ServerActivatePotion_Implementation();
	virtual bool ServerActivatePotion_Validate();

	UFUNCTION()
		void InitiatePotion();

	UFUNCTION()
		void ActivatePotion();

	UFUNCTION()
		void DeactivatePotion();

	UPROPERTY(EditAnywhere, Category = "C++ Potions")
		float PotionDuration = 5.0f;

	UPROPERTY(EditAnywhere, Category = "C++ Potions")
		uint32 bPotionActive : 1;
	// END OF POTINOS

	// OVERLAP
	UFUNCTION()
		void OnTriggerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	// END OVERLAP
public:
	AOnlineGameCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	// DAMAGE ORIENTED FUNCTIONS
	UFUNCTION()
		void DealDamage(AActor* _Enemy);

	UFUNCTION()
		void DecreaseHealth(float Amount);
	// END OF DAMAGE FUNCTIONS



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

	virtual void Tick(float DeltaTime) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FORCEINLINE int GetKeyCount() const { return KeyCount; }
	FORCEINLINE void SetKeyCount(int Keys) { KeyCount = Keys; }
	FORCEINLINE void UseKey() { KeyCount -= 1; KeyCount = FMath::Clamp(KeyCount, 0, 99); }
	FORCEINLINE void GainKey() { KeyCount += 1; KeyCount = FMath::Clamp(KeyCount, 0, 99); }
	FORCEINLINE bool HasKeys() { return KeyCount > 0 ? true : false; }

	FORCEINLINE float GetGoldCount() const { return GoldCount; }
	FORCEINLINE void SetKeyCount(float NewGold) { GoldCount = NewGold; }
	FORCEINLINE void SpendGold(float Amount) { Amount > GoldCount ? GoldCount += 0.0f : GoldCount -= Amount; GoldCount = FMath::Clamp(GoldCount, 0.0f, 99999.0f); }
	FORCEINLINE void ObtainGold(float Amount) { GoldCount += Amount; GoldCount = FMath::Clamp(GoldCount, 0.0f, 99999.0f); }
	FORCEINLINE bool HasGold() { return GoldCount > 0.0f ? true : false; }

	FORCEINLINE int GetPotionCount() const { return PotionCount; }
	FORCEINLINE void SetPotionCount(int NewPotionCount) { PotionCount = NewPotionCount; }
	FORCEINLINE void UsePotion() { PotionCount -= 1; PotionCount = FMath::Clamp(PotionCount, 0, 99); }
	FORCEINLINE void GainPotion() { PotionCount += 1; PotionCount = FMath::Clamp(PotionCount, 0, 99); }
	FORCEINLINE bool HasPotions() { return PotionCount > 0 ? true : false; }

	void ObtainPower(class APowerups* OverlappedPower);

};

