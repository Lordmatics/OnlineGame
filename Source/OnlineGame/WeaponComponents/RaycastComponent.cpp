// Fill out your copyright notice in the Description page of Project Settings.

#include "OnlineGame.h"
#include "RaycastComponent.h"


// Sets default values for this component's properties
URaycastComponent::URaycastComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URaycastComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void URaycastComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// Third Person View
TArray<FHitResult> URaycastComponent::RaycastTP(USkeletalMeshComponent* Mesh, FVector ForwardVector, float CustomLength, float Radius)
{
	UWorld* const World = GetWorld();
	if (World == nullptr) return TArray<FHitResult>(); 
	FVector Start = Mesh->GetComponentLocation() + FVector(0.0f, 0.0f, 100.0f);
	// Forward Vector is different for TP since u can turn camera but not turn facing
	FVector End = Start + (ForwardVector * CustomLength);

	//FVector Start = GetActorLocation();
	//FVector End = GetActorLocation() + GetActorForwardVector() * 1;
	TArray<FHitResult> HitOut;
	FCollisionQueryParams CQP;
	CQP.bTraceComplex = true;
	CQP.bReturnPhysicalMaterial = false;
	CQP.bTraceAsyncScene = true;
	//
	// MeleeTrace - Block
	if (World->SweepMultiByChannel(HitOut, Start, End, FQuat(), ECC_WorldStatic, FCollisionShape::MakeSphere(Radius), CQP))
	{
		//UE_LOG(LogTemp, Warning, TEXT("%d"), HitOut.Num());
		//UE_LOG(LogTemp, Warning, TEXT("Sweep Multi By Channel Made"));
		//if (HitOut.Last().bBlockingHit)
		//{
		//	UE_LOG(LogTemp, Warning, TEXT("Draw Sphere Pls"));
		//	// Red up to the blocking hit, green thereafter
		//	FVector const BlockingHitPoint = HitOut.Last().Location;
		//	DrawDebugSphere(World, Start, Radius, 15.0f, FColor::Green, true);
		//}
		// //draw hits
		//for (int32 HitIdx = 0; HitIdx<HitOut.Num(); ++HitIdx)
		//{
		//	FHitResult const& Hit = HitOut[HitIdx];
		//	::DrawDebugPoint(World, Hit.ImpactPoint, 50.0f, (Hit.bBlockingHit ? FColor::Red : FColor::Green), true);
		//}
		return HitOut;
	}
	else return TArray<FHitResult>();
	//if (World->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, CQP))
	//{
	//	return Hit;
	//}
	//else return FHitResult();
}

// First Person View
FHitResult URaycastComponent::RaycastFP(UCameraComponent* FirstPersonCamera, float CustomLength)
{
	UWorld* const World = GetWorld();
	if (World == nullptr) return FHitResult();
	FHitResult Hit;
	FVector Start = FirstPersonCamera->GetComponentLocation();
	FVector Forward = FirstPersonCamera->GetForwardVector();
	FVector End = Start + (Forward * CustomLength);
	FCollisionQueryParams CQP;
	if (World->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, CQP))
	{
		CreateBulletHole(Hit);
		return Hit;
	}
	else return FHitResult();
}

// ForLaser
FHitResult URaycastComponent::RaycastLaser(UParticleSystemComponent* ParticleSystem, float CustomLength)
{
	UWorld* const World = GetWorld();
	if (World == nullptr) return FHitResult();
	FHitResult Hit;
	FVector Start = ParticleSystem->GetComponentLocation();
	FVector Forward = ParticleSystem->GetForwardVector();
	FVector End = Start + (Forward * CustomLength);
	FCollisionQueryParams CQP;
	if (World->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, CQP))
	{
		return Hit;
	}
	else return FHitResult();
}

void URaycastComponent::CreateBulletHole(FHitResult HitResult)
{
	UWorld* const World = GetWorld();
	if (World == nullptr) return;
	if (BulletHoleDecal != nullptr)
	{
		FVector BulletHoleSizxe = DecalSize;
		UGameplayStatics::SpawnDecalAtLocation(World, BulletHoleDecal, BulletHoleSizxe, HitResult.ImpactPoint, HitResult.ImpactNormal.Rotation(), BulletDecalLifetime);
	}
}

FHitResult URaycastComponent::RayDown(const UWorld* WorldObject, const FVector& Start, float Length)
{
	if (WorldObject != nullptr)
	{
		FHitResult Hit;
		FVector Down = -FVector::UpVector;
		FVector End = Start + (Down * Length);
		FCollisionQueryParams CQP;
		if(WorldObject->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, CQP))
		{
			return Hit;
		}
	}
	return FHitResult();
}