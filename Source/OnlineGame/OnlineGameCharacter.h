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

	UPROPERTY(VisibleDefaultsOnly, Category = "C++ Weapon")
		class UOnlineGameWeapon* WeaponComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = "C++ Animations")
		class UAnimationComponent* AnimationStorage;

	UPROPERTY(VisibleDefaultsOnly, Category = "C++ Weapon")
		class URaycastComponent* RaycastComponent;

	// LOGIC FOR DOING ATTACKS - FINISHED AND SMOOTH
	UPROPERTY(VisibleAnywhere, Category = "C++ Variables")
		uint32 bAttackBufferActive : 1;

	UPROPERTY(VisibleAnywhere, Category = "C++ Variables")
		uint32 bAttackPressedCached : 1;

	FTimerHandle BeginAttackHandle;
	FTimerHandle AttackHandle;
	FTimerHandle BufferAttackHandle;
	// END LOGIC FOR DOING ATTACKS - FINISHED AND SMOOTH
private:
	// My Functions
	// SHOOTING FUNCTIONS
	UFUNCTION()
		void BeginContinuousAttack();
	UFUNCTION()
		void Attack();
	UFUNCTION()
		void EndAttackBuffer();
	UFUNCTION()
		void ResetAttack();
	FHitResult ShootRay();
	// END SHOOTING FUNCTIONS
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

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	virtual void PostInitializeComponents() override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

