#include "AEG_MoveTargetAlongPath.h"

AAEG_MoveTargetAlongPath::AAEG_MoveTargetAlongPath() : Super()
{
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	Root->SetMobility(EComponentMobility::Type::Static);

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("MasterSpline"));
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	SplineComponent->SetupAttachment(Root);
	SpringArmComponent->SetupAttachment(SplineComponent);
	CameraComponent->SetupAttachment(SpringArmComponent);
}

AAEG_MoveTargetAlongPath::~AAEG_MoveTargetAlongPath() {}

FSplinePosPair AAEG_MoveTargetAlongPath::GetSplinePos(FVector SplineWorldPos, USplineComponent* InSpline, float Distance)
{
	FTransform transform = InSpline->GetTransformAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::Local, false);

	FVector localPos = transform.GetLocation();
	localPos.Z -= 90.0;

	auto eulerRot = transform.GetRotation().Rotator();

	FSplinePosPair fSplinePosPair;
	fSplinePosPair.Position = localPos;
	fSplinePosPair.Rotation = FVector(eulerRot.Roll, eulerRot.Pitch, eulerRot.Yaw);

	return fSplinePosPair;
}

void AAEG_MoveTargetAlongPath::SpawnNotes(float PlayerSpeed)
{
	for (auto& it : NotesInfo)
	{
		const AAEG_Path* sp = Splines[it.Line];

		FVector splineWorlP = sp->Root->K2_GetComponentLocation();
		auto splinePosRot = GetSplinePos(splineWorlP, sp->Path, PlayerSpeed * it.Position);

		FVector upvec = sp->Path->GetUpVectorAtDistanceAlongSpline(PlayerSpeed * it.Position, ESplineCoordinateSpace::Local);

		FVector noteSpawnLocation = splinePosRot.Position + upvec.Normalize() * (FVector::UpVector * 100);

		UE::Math::TRotator<double> rotator = UE::Math::TRotator<double>(splinePosRot.Rotation.X, splinePosRot.Rotation.Y, splinePosRot.Rotation.Z);

		GetWorld()->SpawnActor<AActor>(Note.Get(), noteSpawnLocation, rotator);
	}
}

void AAEG_MoveTargetAlongPath::SetSpline(int index)
{
}