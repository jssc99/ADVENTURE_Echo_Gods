// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Adventure_ECHO_GODS_Actor.h"
#include "AEG_WillOTheWisps.generated.h"

/**
 * 
 */
UCLASS()
class ADVENTURE_ECHO_GODS_API AAEG_WillOTheWisps : public AAdventure_ECHO_GODS_Actor
{
	GENERATED_BODY()

public : 

	AAEG_WillOTheWisps();

	UFUNCTION(BlueprintCallable)
	void UpdateIntensityFromDistance(AActor* Actor);
public : 

	class USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PlayerDetectionRange = 200;

	/**
	* Light Intensity in EV
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Light")
	float LightIntensity = 12.4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light")
	UCurveFloat* LightCurve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light")
	class UPointLightComponent* PointLight;

};
