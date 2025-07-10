

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Adventure_ECHO_GODS_Actor.generated.h"

UCLASS()
class ADVENTURE_ECHO_GODS_API AAdventure_ECHO_GODS_Actor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAdventure_ECHO_GODS_Actor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
