

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Adventure_ECHO_GODS_Character.generated.h"

UCLASS()
class ADVENTURE_ECHO_GODS_API AAdventure_ECHO_GODS_Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAdventure_ECHO_GODS_Character();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	
};

