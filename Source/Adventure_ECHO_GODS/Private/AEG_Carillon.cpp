// Fill out your copyright notice in the Description page of Project Settings.


#include "AEG_Carillon.h"
#include "AEG_CarillonFlower.h"

TArray<FString> AAEG_Carillon::GetOptionsTest() {
	TArray<FString> Array = TArray<FString>();
	for (int i = 0; i < FlowerList.Num(); i++) {
		Array.Add(FString::FromInt(i));
	}
	return Array;
}
#if WITH_EDITOR

void AAEG_Carillon::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	FName PropertyName = (PropertyChangedEvent.Property != NULL) ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	if (PropertyName == GET_MEMBER_NAME_CHECKED(AAEG_Carillon, FlowerOrderDropdown))
	{
		FlowerIndex = FCString::Atoi(*FlowerOrderDropdown);
	}
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

void AAEG_Carillon::PostEditChangeChainProperty(FPropertyChangedChainEvent& e)
{
	FName Name = e.GetPropertyName();
	FString PropertyName = Name.ToString();
	if (PropertyName == "FlowerList") {
		if (FlowerIndex < 0)
			FlowerOrderDropdown = "";
		else if (FlowerIndex >= FlowerList.Num()) {
			FlowerOrderDropdown = FString::FromInt(FlowerList.Num() - 1);
			FlowerIndex = FCString::Atoi(*FlowerOrderDropdown);
		}
	}
	Super::PostEditChangeChainProperty(e);
}

#endif