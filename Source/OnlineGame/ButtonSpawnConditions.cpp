// Fill out your copyright notice in the Description page of Project Settings.

#include "OnlineGame.h"
#include "ButtonSpawnConditions.h"
#include "Gates/ButtonSwitch.h"
#include "Gates/KeyGate.h"

// Sets default values
AButtonSpawnConditions::AButtonSpawnConditions()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AButtonSpawnConditions::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AButtonSpawnConditions::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AButtonSpawnConditions::SpawnButton()
{
	UE_LOG(LogTemp, Warning, TEXT("SpawnedButton function"));

	UWorld* const World = GetWorld();
	if (World == nullptr) return;
	if (SpawnLocations.Num() > 0)
	{
		if (Counter < SpawnLocations.Num() && ButtonClass != nullptr && ButtonGates.Num() > 0)
		{
			// Play a cutscene to it maybe
			AButtonSwitch* Button = World->SpawnActor<AButtonSwitch>(ButtonClass, SpawnLocations[Counter]->GetActorLocation(), SpawnLocations[Counter]->GetActorRotation());
			if (Button != nullptr)
			{
				ButtonGates[Counter]->SetButtonRef(Button);
				Counter++;
			}
		}
	}


}
