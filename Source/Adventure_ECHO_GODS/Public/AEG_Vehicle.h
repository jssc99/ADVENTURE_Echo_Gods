

#pragma once

#include "Adventure_ECHO_GODS_Character.h"
#include "AEG_Vehicle.generated.h"

UCLASS()
class ADVENTURE_ECHO_GODS_API AAEG_Vehicle : public AAdventure_ECHO_GODS_Character
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAEG_Vehicle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	
};
