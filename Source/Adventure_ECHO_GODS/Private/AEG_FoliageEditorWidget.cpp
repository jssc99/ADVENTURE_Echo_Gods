

// Fill out your copyright notice in the Description page of Project Settings.
#include "AEG_FoliageEditorWidget.h"


// let this
// we don't want this class include when building in any config
// don't use WITH_EDITOR because it is set to 1 when build in devellopement
// pass to 1 only on editor.exe
#if 1

#include "Runtime/Foliage/Public/InstancedFoliageActor.h"

//https://forums.unrealengine.com/t/cannot-resolve-dependency-on-foliageedit-in-editor-plugin/100142/3



TArray<FTransform> GetTransformsFromFoliage(const FFoliageInfo* f)
{
	
	auto FromComponent = f->GetComponent();
	if (!FromComponent)
		return {};
	
	TArray<FTransform> ts;
	ts.SetNum(FromComponent->GetInstanceCount());

	for (auto i = 0; i < ts.Num(); i++)
		FromComponent->GetInstanceTransform(i, ts[i], true);

	return ts;
}

static bool IsThereMeshInMap(AInstancedFoliageActor* InstancedFoliageActor, const TMap<UFoliageType*, TUniqueObj<FFoliageInfo>>& Map,
	UStaticMesh* FromMesh, UFoliageInstancedStaticMeshComponent** OutUFoliageInstancedStaticMeshComponent)
{
	for (const auto& FoliageTypePair : Map)
	{
		const UFoliageType* FoliageAsset = FoliageTypePair.Key;
		if (const UFoliageType_InstancedStaticMesh* ISMFoliage = Cast<UFoliageType_InstancedStaticMesh>(FoliageAsset))
		{
			if (ISMFoliage->GetStaticMesh() == FromMesh)
			{
				FFoliageInfo* Info = InstancedFoliageActor->FindInfo(FoliageAsset);
				if (Info && Info->GetComponent())
				{
					*OutUFoliageInstancedStaticMeshComponent = Cast<UFoliageInstancedStaticMeshComponent>(Info->GetComponent());
					return true;
				}
			}
		}
	}

	return false;
}

void AInstancedFoliageActor::AddInstances(UObject* WorldContextObject, UFoliageType* InFoliageType, const TArray<FTransform>& InTransforms)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (World)
	{
		TMap<AInstancedFoliageActor*, TArray<const FFoliageInstance*>> InstancesToAdd;
		TArray<FFoliageInstance> FoliageInstances;
		FoliageInstances.Reserve(InTransforms.Num()); // Reserve 

		for (const FTransform& InstanceTransfo : InTransforms)
		{
			AInstancedFoliageActor* IFA = AInstancedFoliageActor::Get(World, true, World->PersistentLevel, InstanceTransfo.GetLocation());
			FFoliageInstance FoliageInstance;
			FoliageInstance.Location = InstanceTransfo.GetLocation();
			FoliageInstance.Rotation = InstanceTransfo.GetRotation().Rotator();
			FoliageInstance.DrawScale3D = (FVector3f)InstanceTransfo.GetScale3D();

			FoliageInstances.Add(FoliageInstance);
			InstancesToAdd.FindOrAdd(IFA).Add(&FoliageInstances[FoliageInstances.Num() - 1]);
		}

		for (const auto& Pair : InstancesToAdd)
		{
			FFoliageInfo* TypeInfo = nullptr;
			if (UFoliageType* FoliageType = Pair.Key->AddFoliageType(InFoliageType, &TypeInfo))
			{
				TypeInfo->AddInstances(FoliageType, Pair.Value);
			}
		}
	}
}




UFoliageInstancedStaticMeshComponent* UFoliageEditorWidget::CopyFoliageTypeAndReplaceWith(
	AInstancedFoliageActor* InstancedFoliageActor,
	UStaticMesh* FromMesh,
	UStaticMesh* ToMesh)
{
	if (!InstancedFoliageActor || !FromMesh || !ToMesh)
	{
		return nullptr;
	}

	const auto& FoliageTypeMap = InstancedFoliageActor->GetFoliageInfos();

	// check if ToMesh already in map
	UFoliageInstancedStaticMeshComponent* FoliageInstancedStaticMeshComponent = nullptr; 
	if (IsThereMeshInMap(InstancedFoliageActor, FoliageTypeMap, ToMesh, &FoliageInstancedStaticMeshComponent))
	{
		return FoliageInstancedStaticMeshComponent;
	}
	
	for (const auto& FoliageTypePair : FoliageTypeMap)
	{
		const UFoliageType* FoliageAsset = FoliageTypePair.Key;
		if (const UFoliageType_InstancedStaticMesh* ISMFoliage = Cast<UFoliageType_InstancedStaticMesh>(FoliageAsset))
		{
			if (ISMFoliage->GetStaticMesh() == FromMesh)
			{
				UFoliageType_InstancedStaticMesh* NewFoliageType = DuplicateObject<UFoliageType_InstancedStaticMesh>(
					ISMFoliage, InstancedFoliageActor);

				NewFoliageType->SetStaticMesh(ToMesh);
				InstancedFoliageActor->AddMesh(NewFoliageType);

				FFoliageInfo* FoliageInfo = InstancedFoliageActor->FindInfo(NewFoliageType);
				auto component = FoliageInfo->GetComponent();

				
				auto ts = GetTransformsFromFoliage(InstancedFoliageActor->FindInfo(FoliageAsset));
				InstancedFoliageActor->AddInstances(InstancedFoliageActor->GetWorld(), NewFoliageType, ts);
				
				return Cast<UFoliageInstancedStaticMeshComponent>(component);
			}
		}
	}
	


	return nullptr;
}



#endif
