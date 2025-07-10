// Fill out your copyright notice in the Description page of Project Settings.


#include "MathLibrary.h"

float UMathLibrary::InverseLerp(float Min, float Max, float Value)
{
	return (Value - Min) / (Max - Min);
}
