// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MathLibrary.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class ADVENTURE_ECHO_GODS_API UMathLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	
public : 
	UFUNCTION(BlueprintCallable,BlueprintPure)
	static float InverseLerp(float Min, float Max, float Value);
	
};
