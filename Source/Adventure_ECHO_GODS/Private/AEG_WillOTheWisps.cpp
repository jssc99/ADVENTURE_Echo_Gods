// Fill out your copyright notice in the Description page of Project Settings.


#include "AEG_WillOTheWisps.h"
#include "MathLibrary.h"
#include "Components/PointLightComponent.h"
#include "Components/SceneComponent.h"

AAEG_WillOTheWisps::AAEG_WillOTheWisps()
{
	SceneComponent = CreateDefaultSubobject <USceneComponent>("Scene");
	SetRootComponent(SceneComponent);
	PointLight = CreateDefaultSubobject<UPointLightComponent>("PointLight");
	PointLight->SetupAttachment(RootComponent);
	PointLight->IntensityUnits = ELightUnits::EV;
}

void AAEG_WillOTheWisps::UpdateIntensityFromDistance(AActor* Actor)
{
	float Distance = FVector2D::Distance((FVector2D)GetActorLocation(), (FVector2D) Actor->GetActorLocation());
	float t = UMathLibrary::InverseLerp(0, PlayerDetectionRange, Distance);
	float LerpIndex = (LightCurve != nullptr) ? LightCurve->GetFloatValue(t) : t;
	float Intensity = FMath::Lerp(0, LightIntensity, LerpIndex);
	PointLight->SetIntensity(Intensity);
}
