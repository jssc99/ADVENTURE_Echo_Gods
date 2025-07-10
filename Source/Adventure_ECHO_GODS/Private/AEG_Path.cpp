#include "AEG_Path.h"

#include "AEG_MoveTargetAlongPath.h"

AAEG_Path::AAEG_Path() : Super()
{
	Root = CreateDefaultSubobject<std::remove_pointer_t<decltype(Root)>>(TEXT("Root"));
	Root->SetMobility(EComponentMobility::Type::Static);

	Path = CreateDefaultSubobject<std::remove_pointer_t<decltype(Path)>>(TEXT("Spline"));
	Path->SetupAttachment(Root);

	TangeanteStrength.Add(0.5f);
}

void AAEG_Path::ResetSpline()
{
		
	if (SectionLength == 0.f || MaxPointsSpline == 0)
		return;

	if (Controller == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("The controller of %s is nullptr"), *GetName());
		return;
	}

	AAEG_MoveTargetAlongPath* masterSpline = Cast<AAEG_MoveTargetAlongPath>(Controller);
	if (masterSpline == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("The controller of %s is not a MoveTargetAlongPath"), *GetName());
		return;
	}

	if (masterSpline->SplineComponent == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("The masterSpline->SplineComponent of %s is nullptr"), *GetName());
		return;
	}

	const int32 sectionLengthInt = FMath::Max(1, FMath::TruncToInt(SectionLength)); // avoid div by zero
	const int32 iterationCount = static_cast<int32>(MaxPointsSpline);
	const int32 nbrOfPoint = FMath::DivideAndRoundUp(iterationCount, sectionLengthInt);

	// Resize containers
	TArray<FVector> points;
	points.SetNumUninitialized(nbrOfPoint);
	TArray<FRotator> rotations;
	rotations.SetNumUninitialized(nbrOfPoint);
	TArray<FVector> tangents;
	tangents.SetNumUninitialized(nbrOfPoint);

	int32 splinePointCount = 0;
	const float splineLength = masterSpline->SplineComponent->GetSplineLength();

	for (int32 i = 0; i < iterationCount; ++i)
	{
		if (i % sectionLengthInt == 0)
		{
			const float alpha = static_cast<float>(i) / FMath::Max(1, iterationCount - 1); 
			const float distance = alpha * splineLength;

			const FOutDecalPoint decalPoint = DecalSplinePoint(distance, masterSpline->SplineComponent, Offset);

			if (splinePointCount < nbrOfPoint - 1)
			{
				points[splinePointCount] = decalPoint.Position;
				rotations[splinePointCount] = decalPoint.Rotation;
				tangents[splinePointCount] = masterSpline->SplineComponent->GetTangentAtDistanceAlongSpline(distance, ESplineCoordinateSpace::Local);
				++splinePointCount;
			}
		}
	}

	
	// Add final point exactly at end of spline
	const FOutDecalPoint outDecalPoint = DecalSplinePoint(splineLength, masterSpline->SplineComponent, Offset);
	points[nbrOfPoint - 1] = outDecalPoint.Position;
	rotations[nbrOfPoint - 1] = outDecalPoint.Rotation;
	tangents[nbrOfPoint - 1] = masterSpline->SplineComponent->GetTangentAtDistanceAlongSpline(splineLength, ESplineCoordinateSpace::Local);

	// Apply to spline
	Path->SetSplinePoints(points, ESplineCoordinateSpace::Local, false);

	for (int32 i = 0; i < nbrOfPoint; ++i)
	{
		Path->SetRotationAtSplinePoint(i, rotations[i], ESplineCoordinateSpace::Local, false);

		const int32 TangentStrengthIndex = GetSplinePointIndexFromAlpha(i);
		checkf(TangentStrengthIndex >= 0 && TangentStrengthIndex < TangeanteStrength.Num(), TEXT("TangentStrengthIndex out of bounds: %d"), TangentStrengthIndex);

		Path->SetTangentAtSplinePoint(i, tangents[i] * TangeanteStrength[TangentStrengthIndex], ESplineCoordinateSpace::Local, false);
	}

	Path->UpdateSpline();
}

FOutDecalPoint AAEG_Path::DecalSplinePoint(float Distance, const USplineComponent* Spline, float InOffset) const
{
	FVector locationAlongSpline = Spline->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::Local);
	locationAlongSpline += Spline->GetRightVectorAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::Local) * InOffset;

	const FRotator rotationAlongSpline = Spline->GetRotationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
	FOutDecalPoint fOutDecalPoint;
	fOutDecalPoint.Position = locationAlongSpline;
	fOutDecalPoint.Rotation = rotationAlongSpline;

	return fOutDecalPoint;
}

FVector3f AAEG_Path::GetPositionAlongSpline(int Index) const
{
	return FVector3f(Path->GetLocationAtDistanceAlongSpline(SectionLength * (Index), ESplineCoordinateSpace::Local));
}

FVector3f AAEG_Path::GetTangentAlongSpline(int Index) const
{
	auto result = FVector3f(Path->GetTangentAtDistanceAlongSpline(SectionLength * (Index), ESplineCoordinateSpace::Local));
	return result.GetClampedToSize(0.f, SectionLength);
}

float AAEG_Path::GetRollAlongSpline(int Index) const
{
	return Path->GetRollAtDistanceAlongSpline(SectionLength * (Index), ESplineCoordinateSpace::Local);
}

void AAEG_Path::SetSplineLength(float NewLength) const
{
	float currentLength = Path->GetSplineLength(), toAdd;
	if (currentLength >= NewLength)
		return;
	else
		toAdd = NewLength - currentLength;

	int32 lengthSpline = Path->GetNumberOfSplinePoints();

	FVector location, tangent;
	Path->GetLocalLocationAndTangentAtSplinePoint(lengthSpline - 1, location, tangent);

	tangent.Normalize(); tangent *= toAdd;
	location += tangent;

	Path->AddSplinePointAtIndex(
		location,
		lengthSpline,
		ESplineCoordinateSpace::Local
	);
}

void AAEG_Path::SpawnAlongSpline(ESplineMeshAxis::Type Axis, ECollisionEnabled::Type ColType)
{
	if (bAllowScalling)
	{
		float scale = GetActorScale3D().X;
		SectionLength *= scale;
	}

	if (!Path || SectionLength <= 0.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid Path or SectionLength"));
		return;
	}

	int32 NumSections = FMath::TruncToInt(Path->GetSplineLength() / SectionLength);
	
	
	if (bMustBeFullMesh)
	{
		NumSections--;
	}

	for (int32 i = 0; i < NumSections; ++i)
	{
		USplineMeshComponent* newSplineMesh = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
		if (!newSplineMesh)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to create spline mesh"));
			break;
		}

		newSplineMesh->SetMobility(EComponentMobility::Static);
		newSplineMesh->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		
		if (SplineMesh)
		{
			newSplineMesh->SetStaticMesh(SplineMesh);
			newSplineMesh->SetCollisionEnabled(ColType);
		}

		newSplineMesh->SetForwardAxis(Axis, false);

		newSplineMesh->SetStartRollDegrees(GetRollAlongSpline(i), false);
		newSplineMesh->SetEndRollDegrees(GetRollAlongSpline(i + 1), false);

		const FVector StartPos = FVector(GetPositionAlongSpline(i) + OffsetPosition);
		const FVector StartTan = FVector(GetTangentAlongSpline(i));
		const FVector EndPos = FVector(GetPositionAlongSpline(i + 1) + OffsetPosition);
		const FVector EndTan = FVector(GetTangentAlongSpline(i + 1));

		newSplineMesh->SetStartAndEnd(StartPos, StartTan, EndPos, EndTan, false);

		newSplineMesh->RegisterComponent();
	}

	RegisterAllComponents(); 
}

int32 AAEG_Path::GetSplinePointIndexFromAlpha(int Alpha) const
{
	if (!Path) return -1;
	
	int32 NumPoints = Path->GetNumberOfSplinePoints();
	if (NumPoints <= 1) return 0;

	const float v = static_cast<float>(TangeanteStrength.Num() - 1) *  Alpha / static_cast<float>(NumPoints - 1);
	return FMath::RoundToInt(v);
}
