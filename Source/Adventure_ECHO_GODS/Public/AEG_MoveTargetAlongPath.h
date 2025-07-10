#pragma once

#include "CoreMinimal.h"
#include "Adventure_ECHO_GODS_Actor.h"
#include "Components/SplineComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "AEG_Path.h"

#include "AEG_MoveTargetAlongPath.generated.h"

/**
 *
 */
USTRUCT(BlueprintType)
struct FNotesInfoGlider
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notes")
	float Position;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notes")
	int Line;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notes")
	bool bHold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notes")
	TMap<float, int> TimeAndLine;

	FNotesInfoGlider() : Position(0.f), Line(0), bHold(false), TimeAndLine({}) {}
};

/**
 *
 */
USTRUCT(BlueprintType)
struct FSplinePosPair
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FVector Position;

	UPROPERTY(BlueprintReadWrite)
	FVector Rotation;

	FSplinePosPair() : Position(FVector::ZeroVector), Rotation(FVector::ZeroVector) {}
};

/**
 *
 */
UCLASS()
class ADVENTURE_ECHO_GODS_API AAEG_MoveTargetAlongPath : public AAdventure_ECHO_GODS_Actor
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USplineComponent* SplineComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Music")
	float MusicTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Music")
	TArray<FNotesInfoGlider> NotesInfo;

	AAEG_MoveTargetAlongPath();

	~AAEG_MoveTargetAlongPath();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fix")
	USceneComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fix")
	TArray<AAEG_Path*> Splines;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fix")
	ACharacter* Character;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fix")
	APlayerController* PlayerController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Note")
	TSubclassOf<AActor> Note;

	UFUNCTION(BlueprintCallable)
	static FSplinePosPair GetSplinePos(FVector _splineWorldPos, USplineComponent* _inSpline, float _distance);

	UFUNCTION(BlueprintCallable)
	void SpawnNotes(float PlayerSpeed);

	UFUNCTION(BlueprintCallable)
	void SetSpline(int index);
};
