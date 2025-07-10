// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CameraPreset.generated.h"


USTRUCT(BlueprintType)
struct FCameraPresetInfo {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly,Category = "Camera")
	FTransform CameraTransform;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = "5", ClampMax = "170"), Category = "Camera")
	float FOV = 90;


	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Spring Arm")
	FTransform ArmTransform;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Spring Arm")
	float TargetArmLength = 0;	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Spring Arm")
	FVector SocketOffset = FVector(0);

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Spring Arm")
	FVector TargetOffset = FVector(0);
};

/**
 * 
 */
UCLASS(BlueprintType)
class ADVENTURE_ECHO_GODS_API UCameraPresets : public UDataAsset
{
	GENERATED_BODY()

public : 

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FCameraPresetInfo> Presets;
	
};

