#include "AEG_FlowerTrailComponent.h"

// ################################## CONSTRUCTORS / DESTRUCTORS ##################################

UAEG_FlowerTrailComponent::UAEG_FlowerTrailComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// ####################################### PUBLIC FUNCTIONS #######################################

void UAEG_FlowerTrailComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	// Call parent TickComponent
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Update Flower Trail Component
	UpdateFlowerTrail(DeltaTime);
	UpdateActiveFlowers(DeltaTime, ActiveMoveFlowers, MoveLifeTime, MoveScaleCurve);
	UpdateActiveFlowers(DeltaTime, ActiveIdleFlowers, IdleLifeTime, IdleScaleCurve);
}

// ###################################### PROTECTED FUNCTIONS #####################################

void UAEG_FlowerTrailComponent::BeginPlay()
{
	// Call parent BeginPlay
	Super::BeginPlay();

	// Init Variables
	MoveSpawnCooldown = MoveSpawnDelay;
	IdleSpawnCooldown = IdleSpawnDelay;
	PreviousOwnerLocation = GetOwner()->GetActorLocation();

	// Call Init functions
	InitPool();
}

// ####################################### PRIVATE FUNCTIONS ######################################

void UAEG_FlowerTrailComponent::InitPool()
{
	// Loop on each flower mesh (type of flower)
	const auto flowerTypeNbr = FlowerTypes.Num();
	for (int i = 0; i < flowerTypeNbr; i++) {

		// Loop on pool size and instantiate flowers
		const auto poolSize = FlowerTypes[i].InitialPoolSize;
		for (int j = 0; j < poolSize; j++) {
			InstantiateFlower(i, false);
		}
	}
}

void UAEG_FlowerTrailComponent::UpdateFlowerTrail(float DeltaTime)
{
	// Check if the owner is moving
	const FVector currentOwnerLocation = GetOwner()->GetActorLocation();
	const bool ownerIsMoving = PreviousOwnerLocation != currentOwnerLocation;

	// Update flower trailer
	if (ownerIsMoving) UpdateMoveFlowerTrail(DeltaTime, currentOwnerLocation);
	else UpdateIdleFlowerTrail(DeltaTime, currentOwnerLocation);

	// Update PreviousOwnerLocation
	PreviousOwnerLocation = currentOwnerLocation;
}

void UAEG_FlowerTrailComponent::UpdateMoveFlowerTrail(float DeltaTime, const FVector& OwnerLocation)
{
	// Check if the cooldown is ended
	if (MoveSpawnCooldown <= 0.f) {
		MoveSpawnCooldown += MoveSpawnDelay;
		SpawnFlowersWhileMoving(OwnerLocation);		
	}
	else MoveSpawnCooldown -= DeltaTime;
}

void UAEG_FlowerTrailComponent::UpdateIdleFlowerTrail(float DeltaTime, const FVector& OwnerLocation)
{
	// Check if the cooldown is ended
	if (IdleSpawnCooldown <= 0.f) {
		IdleSpawnCooldown += IdleSpawnDelay;
		SpawnFlowersWhileIdle(OwnerLocation);		
	}
	else IdleSpawnCooldown -= DeltaTime;
}

void UAEG_FlowerTrailComponent::UpdateActiveFlowers(float DeltaTime, TArray<FFlowerInstance>& ActiveFlowers, float LifeTime, UCurveFloat* ScaleCurve)
{
	// Loop on each active flower
	for (int i = ActiveFlowers.Num() - 1; i >= 0; i--) {

		// Update elapsed time
		ActiveFlowers[i].Elapsed += DeltaTime;
		const float t = ActiveFlowers[i].Elapsed / LifeTime;
		const float clampedT = FMath::Clamp(t, 0.0f, 1.0f);
		
		// Update scale
		const float scaleMultiplier = ScaleCurve->GetFloatValue(clampedT);
		ActiveFlowers[i].MeshActor->SetActorScale3D(ActiveFlowers[i].TargetScale * scaleMultiplier);

		// Check if flower reached is lifetime
		if (clampedT >= 1.f) DespawnFlower(ActiveFlowers[i], i, ActiveFlowers);
	}
}

void UAEG_FlowerTrailComponent::SpawnFlowersWhileMoving(const FVector& CurrentOwnerLocation)
{
	// Loop on each ray direction
	const auto rayDirectionNbr = RayDirections.Num();
	for (int i = 0; i < rayDirectionNbr; i++) {

		// Raycast
		FHitResult hitResult;
		FCollisionQueryParams params;
		params.AddIgnoredActor(GetOwner());
		const FVector start = CurrentOwnerLocation + RayOriginOffset;
		const FVector end = start + RayDirections[i] * RayLength;
		const bool bHit = GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECC_Visibility, params);

		// Check if raycast returned a hit
		if (bHit) {
			// Check detection tag 
			if (DetectionTag != "" && !hitResult.GetActor()->ActorHasTag(DetectionTag)) return;

			// Calculate world rotation
			FQuat baseQuat = FRotationMatrix::MakeFromZ(hitResult.ImpactNormal).ToQuat();
			float yawRandom = FMath::RandRange(0.f, 360.f);
			FQuat randomYawQuat = FQuat(hitResult.ImpactNormal, FMath::DegreesToRadians(yawRandom));
			FQuat finalQuat = randomYawQuat * baseQuat;

			// Calculate world scale
			const float scale = FMath::FRandRange(MoveSpawnMinScale, MoveSpawnMaxScale);
			const FVector scaleVector = FVector(scale, scale, scale);
			
			// Spawn flower
			SpawnFlower(GetRandomTypeIndex(), hitResult.Location, finalQuat.Rotator(), scaleVector, ActiveMoveFlowers);
		}
	}
}

void UAEG_FlowerTrailComponent::SpawnFlowersWhileIdle(const FVector& CurrentOwnerLocation)
{
	// Loop on idle spawn count
	for (int i = 0; i < IdleSpawnCount; i++) {

		// Variables
		const float angle = FMath::FRandRange(0.f, 2 * PI);
		const float radius = FMath::Sqrt(FMath::FRand());
		const float scaledRadius = radius * IdleSpawnRadius;
		const FVector offset = FVector(FMath::Cos(angle), FMath::Sin(angle), 0.f) * scaledRadius;

		// Raycast
		FHitResult hitResult;
		FCollisionQueryParams params;
		params.AddIgnoredActor(GetOwner());
		const FVector start = CurrentOwnerLocation + offset + RayOriginOffset;
		const FVector end = start - FVector(0.f, 0.f, RayLength);
		const bool bHit = GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECC_Visibility, params);

		// Check if raycast returned a hit
		if (bHit) {
			// Check detection tag 
			if (DetectionTag != "" && !hitResult.GetActor()->ActorHasTag(DetectionTag)) return;

			// Calculate world rotation
			FQuat baseQuat = FRotationMatrix::MakeFromZ(hitResult.ImpactNormal).ToQuat();
			float yawRandom = FMath::RandRange(0.f, 360.f);
			FQuat randomYawQuat = FQuat(hitResult.ImpactNormal, FMath::DegreesToRadians(yawRandom));
			FQuat finalQuat = randomYawQuat * baseQuat;

			// Calculate world scale
			const float scale = FMath::Lerp(IdleSpawnMaxScale, IdleSpawnMinScale, radius);
			const FVector scaleVector = FVector(scale, scale, scale);
			
			// Spawn flower
			SpawnFlower(GetRandomTypeIndex(), hitResult.Location, finalQuat.Rotator(), scaleVector, ActiveIdleFlowers);
		}
	}
}

AStaticMeshActor* UAEG_FlowerTrailComponent::InstantiateFlower(int32 TypeIndex, bool InitAsActive)
{
	// Variables
	const auto& flowerType = FlowerTypes[TypeIndex];

	// Instantiate a new static mesh actor
	AStaticMeshActor* flowerActor = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass());
	if (!flowerActor) return nullptr;

	// Setup static mesh actor
	flowerActor->SetActorTickEnabled(false);
	
	// Setup static mesh component
	if (auto meshComponent = flowerActor->GetStaticMeshComponent()) {
		meshComponent->SetMobility(EComponentMobility::Movable);
		meshComponent->SetStaticMesh(flowerType.Mesh);
		meshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		meshComponent->SetGenerateOverlapEvents(false);
		meshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

		// Check if init as active
		if (!InitAsActive) {
			flowerActor->SetActorHiddenInGame(true);
			FlowerTypes[TypeIndex].AvailableFlowers.Add(flowerActor);
		}
		else flowerActor->SetActorHiddenInGame(false);
	}
	
	return flowerActor;
}

FFlowerInstance* UAEG_FlowerTrailComponent::SpawnFlower(int32 TypeIndex, const FVector& WorldLocation, const FRotator& WorldRotation, const FVector& WorldScale, TArray<FFlowerInstance>& ActiveFlowers)
{
	// Variables
	AStaticMeshActor* meshActor;

	// Get Mesh for available meshes
	if (!FlowerTypes[TypeIndex].AvailableFlowers.IsEmpty()) {
		meshActor = FlowerTypes[TypeIndex].AvailableFlowers.Pop();
		meshActor->SetActorHiddenInGame(false);
	}
	// Instantiate a new mesh
	else meshActor = InstantiateFlower(TypeIndex, true);

	// Update mesh transform
	meshActor->SetActorLocation(WorldLocation);
	meshActor->SetActorRotation(WorldRotation);
	meshActor->SetActorScale3D(FVector(0, 0, 0));

	// Init FlowerInstance
	const FFlowerInstance instance {
		.Elapsed = 0.f,
		.TypeIndex = TypeIndex,
		.MeshActor = meshActor,
		.TargetScale = WorldScale
	};
	const auto instanceIndex = ActiveFlowers.Add(instance);
	
	return &ActiveFlowers[instanceIndex];
}

void UAEG_FlowerTrailComponent::DespawnFlower(const FFlowerInstance& FlowerInstance, int32 InstanceIndex, TArray<FFlowerInstance>& ActiveFlowers)
{
	FlowerInstance.MeshActor->SetActorHiddenInGame(true);
	FlowerTypes[FlowerInstance.TypeIndex].AvailableFlowers.Add(FlowerInstance.MeshActor);
	ActiveFlowers.RemoveAt(InstanceIndex);
}

int32 UAEG_FlowerTrailComponent::GetRandomTypeIndex() const
{
	// Create available index array
	TArray<int32> availableTypes;
	const auto flowerTypeNbr = FlowerTypes.Num();
	for (int j = 0; j < flowerTypeNbr; j++) {
		if (!FlowerTypes[j].AvailableFlowers.IsEmpty()) availableTypes.Add(j);
	}

	// Return random type index
	if (availableTypes.IsEmpty()) return FMath::RandRange(0, FlowerTypes.Num() - 1);
	return availableTypes[FMath::RandRange(0, availableTypes.Num() - 1)];
}

