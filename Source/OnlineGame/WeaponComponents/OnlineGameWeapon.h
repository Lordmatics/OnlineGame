// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "OnlineGameWeapon.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ONLINEGAME_API UOnlineGameWeapon : public UActorComponent
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category = "C++ Weapon")
		USkeletalMesh* SkeletalMeshRef;

	UPROPERTY(EditDefaultsOnly, Category = "C++ Weapon")
		FName WeaponSocketName = FName("hand_rSocket");
public:	
	// Sets default values for this component's properties
	UOnlineGameWeapon();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	// ForceInlines

	FORCEINLINE USkeletalMesh* GetWeaponMesh() const { return SkeletalMeshRef; }
	FORCEINLINE FName GetWeaponSocketName() const { return WeaponSocketName; }
		
	
};
