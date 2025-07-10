#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/StaticMeshActor.h"
#include "AEG_FlowerTrailComponent.generated.h"

// ############################################ STRUCTS ###########################################

// FLOWER TYPE DATA
USTRUCT()
struct FFlowerTypeData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UStaticMesh* Mesh = nullptr;
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	int InitialPoolSize = 0;
	TArray<AStaticMeshActor*> AvailableFlowers;
};

// FLOWER INSTANCE
USTRUCT()
struct FFlowerInstance
{
	GENERATED_BODY()

	float Elapsed = 0.f;
	int32 TypeIndex = -1;
	AStaticMeshActor* MeshActor = nullptr;
	FVector TargetScale = FVector::ZeroVector;
};

// ############################################# CLASS ############################################

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ADVENTURE_ECHO_GODS_API UAEG_FlowerTrailComponent : public UActorComponent
{
	GENERATED_BODY()
	
// ---------------------------------- CONSTRUCTORS / DESTRUCTORS ----------------------------------
public:
	UAEG_FlowerTrailComponent();

// ------------------------------------------- VARIABLES ------------------------------------------
private:
	// Internal Variables
	TArray<FFlowerInstance> ActiveMoveFlowers;
	TArray<FFlowerInstance> ActiveIdleFlowers;
	FVector PreviousOwnerLocation;
	float MoveSpawnCooldown = 0.f;
	float IdleSpawnCooldown = 0.f;

	// Flower Trail Settings
	// ---------------------
	
	// Core Settings
	UPROPERTY(EditAnywhere, Category = "Flower Trail Settings|Core Settings", meta = (AllowPrivateAccess = "true"))
	TArray<FFlowerTypeData> FlowerTypes;
	UPROPERTY(EditAnywhere, Category = "Flower Trail Settings|Core Settings", meta = (AllowPrivateAccess = "true"))
	FName DetectionTag;
	UPROPERTY(EditAnywhere, Category = "Flower Trail Settings|Core Settings", meta = (AllowPrivateAccess = "true"))
	FVector RayOriginOffset;
	UPROPERTY(EditAnywhere, Category = "Flower Trail Settings|Core Settings", meta = (AllowPrivateAccess = "true"))
	float RayLength = 100.f;

	// Move Spawn Settings
	UPROPERTY(EditAnywhere, Category = "Flower Trail Settings|Move Spawn Settings", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* MoveScaleCurve = nullptr;
	UPROPERTY(EditAnywhere, Category = "Flower Trail Settings|Move Spawn Settings", meta = (AllowPrivateAccess = "true"))
	TArray<FVector> RayDirections;
	UPROPERTY(EditAnywhere, Category = "Flower Trail Settings|Move Spawn Settings", meta = (AllowPrivateAccess = "true"))
	float MoveSpawnDelay = 0.2f;
	UPROPERTY(EditAnywhere, Category = "Flower Trail Settings|Move Spawn Settings", meta = (AllowPrivateAccess = "true"))
	float MoveLifeTime = 1.f;
	UPROPERTY(EditAnywhere, Category = "Flower Trail Settings|Move Spawn Settings", meta = (AllowPrivateAccess = "true"))
	float MoveSpawnMinScale = 0.5f;
	UPROPERTY(EditAnywhere, Category = "Flower Trail Settings|Move Spawn Settings", meta = (AllowPrivateAccess = "true"))
	float MoveSpawnMaxScale = 2.f;

	// Idle Spawn Settings
	UPROPERTY(EditAnywhere, Category = "Flower Trail Settings|Idle Spawn Settings", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* IdleScaleCurve = nullptr;
	UPROPERTY(EditAnywhere, Category = "Flower Trail Settings|Idle Spawn Settings", meta = (AllowPrivateAccess = "true", ClampMin = "0"))
	int32 IdleSpawnCount = 10;
	UPROPERTY(EditAnywhere, Category = "Flower Trail Settings|Idle Spawn Settings", meta = (AllowPrivateAccess = "true"))
	float IdleSpawnRadius = 100.f;
	UPROPERTY(EditAnywhere, Category = "Flower Trail Settings|Idle Spawn Settings", meta = (AllowPrivateAccess = "true"))
	float IdleSpawnDelay = 1.f;
	UPROPERTY(EditAnywhere, Category = "Flower Trail Settings|Idle Spawn Settings", meta = (AllowPrivateAccess = "true"))
	float IdleLifeTime = 10.f;
	UPROPERTY(EditAnywhere, Category = "Flower Trail Settings|Idle Spawn Settings", meta = (AllowPrivateAccess = "true"))
	float IdleSpawnMinScale = 0.5f;
	UPROPERTY(EditAnywhere, Category = "Flower Trail Settings|Idle Spawn Settings", meta = (AllowPrivateAccess = "true"))
	float IdleSpawnMaxScale = 2.f;
	
// ------------------------------------------- FUNCTIONS ------------------------------------------
private:
	void InitPool();
	void UpdateFlowerTrail(float DeltaTime);
	void UpdateMoveFlowerTrail(float DeltaTime, const FVector& OwnerLocation);
	void UpdateIdleFlowerTrail(float DeltaTime, const FVector& OwnerLocation);
	void UpdateActiveFlowers(float DeltaTime, TArray<FFlowerInstance>& ActiveFlowers, float LifeTime, UCurveFloat* ScaleCurve);
	void SpawnFlowersWhileMoving(const FVector& CurrentOwnerLocation);
	void SpawnFlowersWhileIdle(const FVector& CurrentOwnerLocation);
	AStaticMeshActor* InstantiateFlower(int32 TypeIndex, bool InitAsActive);
	FFlowerInstance* SpawnFlower(int32 TypeIndex, const FVector& WorldLocation, const FRotator& WorldRotation, const FVector& WorldScale, TArray<FFlowerInstance>& ActiveFlowers);
	void DespawnFlower(const FFlowerInstance& FlowerInstance, int32 InstanceIndex, TArray<FFlowerInstance>& ActiveFlowers);
	int32 GetRandomTypeIndex() const;
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
