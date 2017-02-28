// Fill out your copyright notice in the Description page of Project Settings.

#include "OnlineGame.h"
#include "WeaponProjectile.h"

// Sets default values
AWeaponProjectile::AWeaponProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("MyProjRoot"));
	RootComponent = MyRoot;
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	//CollisionComp->OnComponentHit.AddDynamic(this, &AQuackProjectile::OnHit);		// set up a notification for when this component hits something blocking
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AWeaponProjectile::OnTriggerEnter);

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;
	CollisionComp->SetupAttachment(MyRoot);
	// Set as root component
	//RootComponent = CollisionComp;

	ProjectileMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(CollisionComp);


	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->bRotationFollowsVelocity = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

// Called when the game starts or when spawned
void AWeaponProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeaponProjectile::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	FRotator CurrentRotation = ProjectileMesh->GetComponentRotation();
	CurrentRotation.Add(DeltaTime*RotationY, DeltaTime*RotationZ, DeltaTime*RotationX);
	ProjectileMesh->SetRelativeRotation(CurrentRotation);

}

void AWeaponProjectile::OnTriggerEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Projectile Destroyed"));
		if (OnEnemyHit.IsBound())
		{
			OnEnemyHit.Broadcast(OtherActor);
			//OnEnemyHit.Broadcast();
		}
		Destroy();
	}
	// Maybe Broadcast something hit, pass in actor, delegate converts to correct type
	// and does logic elsewhere, for decoupling
}

void AWeaponProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

}