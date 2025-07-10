// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Adventure_ECHO_GODS_Actor.h"
#include "AEG_CarillonFlower.generated.h"

/**
 * 
 */
UCLASS()
class ADVENTURE_ECHO_GODS_API AAEG_CarillonFlower : public AAdventure_ECHO_GODS_Actor
{
	GENERATED_BODY()

public : 
	UFUNCTION(BlueprintCallable,BlueprintImplementableEvent)
	void ChangeStyleWhite();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ChangeStyleBlue();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void Dance();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void React();
	
};
