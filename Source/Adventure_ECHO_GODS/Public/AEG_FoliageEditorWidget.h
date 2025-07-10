#pragma once

// let this
// we don't want this class include when building in any config
// don't use WITH_EDITOR because it is set to 1 when build in devellopement
// pass to 1 only on editor.exe
#if 1

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AEG_FoliageEditorWidget.generated.h"

UCLASS()
class UFoliageEditorWidget : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category="Utils")
	static UFoliageInstancedStaticMeshComponent* CopyFoliageTypeAndReplaceWith(
		AInstancedFoliageActor* InstancedFoliageActor,
		UStaticMesh* from,
		UStaticMesh* to);

	
};
#endif
