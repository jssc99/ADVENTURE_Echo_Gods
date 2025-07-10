#pragma once

#include "CoreMinimal.h"
#include "Adventure_ECHO_GODS_Actor.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "AEG_Path.generated.h"

/**
*
*/
USTRUCT(BlueprintType)
struct FOutDecalPoint
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Position;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator Rotation;

	FOutDecalPoint() : Position(FVector::ZeroVector), Rotation(FRotator::ZeroRotator) {}
};

/**
 *
 */
UCLASS()
class ADVENTURE_ECHO_GODS_API AAEG_Path : public AAdventure_ECHO_GODS_Actor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fix")
	USplineComponent* Path;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fix")
	USceneComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	AActor* Controller;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	bool bAllowScalling;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	bool bMustBeFullMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMesh* SplineMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	float SectionLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	float MaxPointsSpline = 10000.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	FVector3f OffsetPosition;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TArray<float> TangeanteStrength;

	AAEG_Path();

	UFUNCTION(BlueprintCallable)
	void ResetSpline();

	UFUNCTION(BlueprintCallable)
	FOutDecalPoint DecalSplinePoint(float Distance, const USplineComponent* Spline, float InOffset) const;

	UFUNCTION(BlueprintCallable)
	FVector3f GetPositionAlongSpline(int Index) const;

	UFUNCTION(BlueprintCallable)
	FVector3f GetTangentAlongSpline(int Index) const;

	UFUNCTION(BlueprintCallable)
	float GetRollAlongSpline(int Index) const;

	UFUNCTION(BlueprintCallable)
	void SetSplineLength(float NewLength) const;

	UFUNCTION(BlueprintCallable)
	void SpawnAlongSpline(ESplineMeshAxis::Type Axis = ESplineMeshAxis::X, ECollisionEnabled::Type ColType = ECollisionEnabled::NoCollision);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	float Offset;

	UFUNCTION(BlueprintCallable)
	int32 GetSplinePointIndexFromAlpha(int Alpha) const;

};
