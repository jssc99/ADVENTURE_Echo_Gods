// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Adventure_ECHO_GODS_Actor.h"
#include "AEG_Carillon.generated.h"

/**
 *
 */
UCLASS(BlueprintType)
class ADVENTURE_ECHO_GODS_API AAEG_Carillon : public AAdventure_ECHO_GODS_Actor
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flower")
	TArray<class AAEG_CarillonFlower*> FlowerList;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Flower", meta = (GetOptions = "GetOptionsTest"))
	FString FlowerOrderDropdown = "-1";

	UPROPERTY(BlueprintReadOnly, Category = "Flower", meta = (ArrayClamp = "FlowerList"))
	int FlowerIndex = -1;

	UFUNCTION()
	TArray<FString> GetOptionsTest();

#if WITH_EDITOR
	void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;


	void PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent)  override;
#endif
};
