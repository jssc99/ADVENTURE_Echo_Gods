

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Adventure_ECHO_GODS_Pawn.generated.h"

UCLASS()
class ADVENTURE_ECHO_GODS_API AAdventure_ECHO_GODS_Pawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AAdventure_ECHO_GODS_Pawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	
};
