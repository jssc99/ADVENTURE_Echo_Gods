


#include "AEG_Vehicle.h"


// Sets default values
AAEG_Vehicle::AAEG_Vehicle()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAEG_Vehicle::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAEG_Vehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAEG_Vehicle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


