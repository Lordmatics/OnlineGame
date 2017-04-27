// Fill out your copyright notice in the Description page of Project Settings.

#include "OnlineGame.h"
#include "Gates/KeyGate.h"
#include "OnlineGameCharacter.h"
#include "Classes/Animation/AnimInstance.h"
#include "Gates/ButtonSwitch.h"

// Sets default values
AKeyGate::AKeyGate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("MyRootComponent"));
	RootComponent = MyRoot;

	MyGate = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MyGate"));
	MyGate->bGenerateOverlapEvents = true;
	MyGate->SetHiddenInGame(false);
	MyGate->SetupAttachment(MyRoot);

	//MyGateEnd = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MyGateEnd"));
	//MyGateEnd->bGenerateOverlapEvents = true;
	//MyGateEnd->SetHiddenInGame(true);
	//MyGateEnd->SetupAttachment(MyRoot);

	MyGateEndSM = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyGateEndSM"));
	MyGateEndSM->bGenerateOverlapEvents = true;
	MyGateEndSM->SetHiddenInGame(true);
	MyGateEndSM->SetupAttachment(MyRoot);

	MyTriggerZone = CreateDefaultSubobject<UBoxComponent>(TEXT("MyTriggerZone"));
	MyTriggerZone->bGenerateOverlapEvents = true;
	MyTriggerZone->OnComponentBeginOverlap.AddDynamic(this, &AKeyGate::OnTriggerEnter);
	MyTriggerZone->SetupAttachment(MyGate);

	bGateOpen = false;
}

void AKeyGate::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(AKeyGate, bGateOpen);

	//DOREPLIFETIME(AKeyGate, bButtonNotKey);

}

// Called when the game starts or when spawned
void AKeyGate::BeginPlay()
{
	Super::BeginPlay();

	if (MyGate != nullptr)
	{
		//MyGate->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		MyGate->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	}
	if (MyGateEndSM != nullptr)
	{
		//MyGateEnd->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		MyGateEndSM->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	}
	
}

// Called every frame
void AKeyGate::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );


		// If there is time, optimise this

	if (Button != nullptr)
	{
		if (Button->GetPressed() && bButtonNotKey && !bGateOpen)
		{
			InitiateOpen();
			//OpenGate();
		}
	}
}

void AKeyGate::InitiateOpen()
{
	if (HasAuthority())
	{
		MulticastOpenGate();
	}
	else
	{
		ServerOpenGate();
	}
}

void AKeyGate::MulticastOpenGate_Implementation()
{
	OpenGate();
}

void AKeyGate::ServerOpenGate_Implementation()
{
	MulticastOpenGate();
}

bool AKeyGate::ServerOpenGate_Validate()
{
	return true;
}

bool AKeyGate::OpenGate()
{
	UWorld* const World = GetWorld();
	if (World == nullptr) return false;

	if (MyGate != nullptr)
	{
		UAnimInstance* AnimInstance = MyGate->GetAnimInstance();
		if (AnimInstance != nullptr && OpenAnim != nullptr)
		{
			bGateOpen = true;
			float AnimDuration = AnimInstance->Montage_Play(OpenAnim, PlaySpeed);
			//OnGateOpen();
			if(MyGate != nullptr)
				MyGate->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			if(MyGateEndSM != nullptr)
				MyGateEndSM->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);


			FTimerHandle OpenGateHandle;
			World->GetTimerManager().SetTimer(OpenGateHandle, this, &AKeyGate::OnGateOpen, AnimDuration + AnimDurationOffset);
			//UE_LOG(LogTemp, Warning, TEXT("Duration: %f"), AnimDuration + AnimDurationOffset);
			return true;
		}
	}
	return false;
}

void AKeyGate::OnGateOpen()
{
	//UE_LOG(LogTemp, Warning, TEXT("Timer Ran"));

	if (MyGate != nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Timer Succeeded"));

		//MyGate->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
		//MyGate->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		//MyGate->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		MyGate->SetHiddenInGame(true);
	}
	if (MyGateEndSM != nullptr)
	{
		//MyGateEndSM->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
		//MyGateEnd->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		MyGateEndSM->SetHiddenInGame(false);
	}
}

void AKeyGate::OnTriggerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// If Gate is open , don't do anything
	if (bGateOpen) return;

	AOnlineGameCharacter* MyCharacter = Cast<AOnlineGameCharacter>(OtherActor);
	if (MyCharacter != nullptr)
	{
		if (MyCharacter->HasKeys() && !bButtonNotKey)
		{
			if (OpenGate())
			{
				MyCharacter->UseKey();
			}
		}
	}
}