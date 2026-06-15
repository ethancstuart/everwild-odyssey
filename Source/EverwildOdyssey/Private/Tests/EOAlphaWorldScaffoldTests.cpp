#if WITH_DEV_AUTOMATION_TESTS

#include "Components/PointLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Gameplay/EOAlphaWorldScaffold.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Misc/AutomationTest.h"
#include "Presentation/EOAssetRoleTypes.h"
#include "World/EOZoneProfile.h"

namespace
{
bool AreColorsNearlyEqual(const FLinearColor& A, const FLinearColor& B)
{
    constexpr float Tolerance = 0.001f;
    return FMath::IsNearlyEqual(A.R, B.R, Tolerance)
        && FMath::IsNearlyEqual(A.G, B.G, Tolerance)
        && FMath::IsNearlyEqual(A.B, B.B, Tolerance)
        && FMath::IsNearlyEqual(A.A, B.A, Tolerance);
}

UWorld* CreateRuntimeScaffoldTestWorld()
{
    if (GEngine == nullptr)
    {
        return nullptr;
    }

    const FName WorldName = MakeUniqueObjectName(nullptr, UWorld::StaticClass(), TEXT("EOAlphaWorldScaffoldTestWorld"), EUniqueObjectNameOptions::GloballyUnique);
    UWorld* TestWorld = UWorld::CreateWorld(EWorldType::Game, false, WorldName, GetTransientPackage());
    if (TestWorld == nullptr)
    {
        return nullptr;
    }

    FWorldContext& WorldContext = GEngine->CreateNewWorldContext(EWorldType::Game);
    TestWorld->AddToRoot();
    WorldContext.SetCurrentWorld(TestWorld);
    TestWorld->InitializeActorsForPlay(FURL());
    return TestWorld;
}

void DestroyRuntimeScaffoldTestWorld(UWorld* TestWorld)
{
    if (TestWorld == nullptr)
    {
        return;
    }

    TestWorld->RemoveFromRoot();
    if (GEngine != nullptr)
    {
        GEngine->DestroyWorldContext(TestWorld);
    }
    TestWorld->DestroyWorld(false);
}

const UStaticMeshComponent* FindRuntimeMeshWithTags(const TArray<TObjectPtr<UStaticMeshComponent>>& RuntimeMeshes, FName SpecId, FName AssetRoleId)
{
    for (const TObjectPtr<UStaticMeshComponent>& RuntimeMesh : RuntimeMeshes)
    {
        const UStaticMeshComponent* MeshComponent = RuntimeMesh.Get();
        if (MeshComponent != nullptr
            && MeshComponent->ComponentTags.Contains(SpecId)
            && MeshComponent->ComponentTags.Contains(AssetRoleId))
        {
            return MeshComponent;
        }
    }

    return nullptr;
}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEOAlphaWorldScaffoldTest, "EverwildOdyssey.Gameplay.AlphaWorldScaffold", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FEOAlphaWorldScaffoldTest::RunTest(const FString& Parameters)
{
    const FEOZoneProfile Profile = FEOZoneProfileCatalog::BuildStarfallValeProfile();
    TestTrue(TEXT("Alpha world consumes a valid Starfall Vale profile."), Profile.IsValidForAlpha());
    TestEqual(TEXT("Scaffold expected landmark count matches profile."), AEOAlphaWorldScaffold::ExpectedLandmarkCount(), Profile.Landmarks.Num());
    TestEqual(TEXT("Scaffold expected scenic prop count matches profile."), AEOAlphaWorldScaffold::ExpectedScenicPropCount(), Profile.ScenicProps.Num());
    TestEqual(TEXT("Scaffold expected ambient light count matches profile."), AEOAlphaWorldScaffold::ExpectedAmbientLightCount(), Profile.Lights.Num());
    TestEqual(TEXT("Scaffold expected minimap marker count matches profile."), AEOAlphaWorldScaffold::ExpectedMinimapMarkerCount(), Profile.MinimapMarkers.Num());

    const TArray<FEOZoneVisualSpec> Landmarks = AEOAlphaWorldScaffold::BuildDefaultLandmarks();

    TestEqual(TEXT("Alpha world exposes expected landmark count."), Landmarks.Num(), AEOAlphaWorldScaffold::ExpectedLandmarkCount());

    TSet<FName> LandmarkIds;
    for (const FEOZoneVisualSpec& Landmark : Landmarks)
    {
        TestTrue(TEXT("Each alpha landmark is valid."), Landmark.IsValidForAlpha());
        TestTrue(TEXT("Each alpha landmark has a readable tint."), Landmark.Tint.A > 0.5f && Landmark.Tint != FLinearColor::White);
        LandmarkIds.Add(Landmark.Id);
    }

    TestTrue(TEXT("Dawnwatch Gate landmark exists."), LandmarkIds.Contains(TEXT("landmark.dawnwatch.gate")));
    TestTrue(TEXT("Starfall Road landmark exists."), LandmarkIds.Contains(TEXT("landmark.starfall_road")));
    TestTrue(TEXT("Relic Surge landmark exists."), LandmarkIds.Contains(TEXT("landmark.relic_core")));

    const TArray<FEOZoneVisualSpec> ScenicProps = AEOAlphaWorldScaffold::BuildDefaultScenicProps();
    TestEqual(TEXT("Alpha world exposes expected scenic prop count."), ScenicProps.Num(), AEOAlphaWorldScaffold::ExpectedScenicPropCount());

    TSet<FName> AssetRoles;
    for (const FEOZoneVisualSpec& ScenicProp : ScenicProps)
    {
        TestTrue(TEXT("Each scenic prop is valid."), ScenicProp.IsValidForAlpha());
        AssetRoles.Add(ScenicProp.AssetRoleId);
    }

    TestTrue(TEXT("Alpha world has road dressing."), AssetRoles.Contains(TEXT("zone.road.lamp")));
    TestTrue(TEXT("Alpha world has crystalline surge dressing."), AssetRoles.Contains(TEXT("zone.relic_surge.crystal")));
    TestTrue(TEXT("Alpha world has hub dressing."), AssetRoles.Contains(TEXT("zone.vendor.crates")));
    TestTrue(TEXT("Alpha world has foliage dressing."), AssetRoles.Contains(TEXT("zone.foliage.tree")));

    const TArray<FEOZoneLightSpec> AmbientLights = AEOAlphaWorldScaffold::BuildDefaultAmbientLights();
    TestEqual(TEXT("Alpha world exposes expected ambient light count."), AmbientLights.Num(), AEOAlphaWorldScaffold::ExpectedAmbientLightCount());

    for (const FEOZoneLightSpec& AmbientLight : AmbientLights)
    {
        TestTrue(TEXT("Each ambient light is valid."), AmbientLight.IsValidForAlpha());
    }

    const TArray<FEOMinimapMarkerSpec> MinimapMarkers = AEOAlphaWorldScaffold::BuildDefaultMinimapMarkers();
    TestEqual(TEXT("Alpha world exposes expected minimap marker count."), MinimapMarkers.Num(), AEOAlphaWorldScaffold::ExpectedMinimapMarkerCount());

    for (const FEOMinimapMarkerSpec& MinimapMarker : MinimapMarkers)
    {
        TestTrue(TEXT("Each minimap marker is valid."), MinimapMarker.IsValidForAlpha());
    }

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEOAlphaWorldScaffoldRuntimeGenerationTest, "EverwildOdyssey.Gameplay.AlphaWorldScaffold.RuntimeGeneration", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FEOAlphaWorldScaffoldRuntimeGenerationTest::RunTest(const FString& Parameters)
{
    const FEOZoneProfile Profile = FEOZoneProfileCatalog::BuildStarfallValeProfile();
    UWorld* TestWorld = CreateRuntimeScaffoldTestWorld();
    TestNotNull(TEXT("Runtime scaffold test world can be created."), TestWorld);

    AEOAlphaWorldScaffold* Scaffold = nullptr;
    if (TestWorld != nullptr)
    {
        Scaffold = TestWorld->SpawnActor<AEOAlphaWorldScaffold>();
    }

    TestNotNull(TEXT("Runtime scaffold actor can be spawned."), Scaffold);

    if (Scaffold == nullptr)
    {
        DestroyRuntimeScaffoldTestWorld(TestWorld);
        return false;
    }

    Scaffold->DispatchBeginPlay();
    TestTrue(TEXT("Runtime scaffold actor follows BeginPlay lifecycle."), Scaffold->HasActorBegunPlay());

    const TArray<TObjectPtr<UStaticMeshComponent>>& RuntimeMeshes = Scaffold->GetRuntimeWorldMeshesForTesting();
    const TArray<TObjectPtr<UPointLightComponent>>& RuntimeLights = Scaffold->GetRuntimeWorldLightsForTesting();
    const TArray<TObjectPtr<UStaticMeshComponent>>& RuntimeMarkers = Scaffold->GetRuntimeWorldMarkerMeshesForTesting();

    TestEqual(TEXT("Runtime mesh count matches profile visuals."), RuntimeMeshes.Num(), Profile.Landmarks.Num() + Profile.ScenicProps.Num());
    TestEqual(TEXT("Runtime point light count matches profile lights."), RuntimeLights.Num(), Profile.Lights.Num());
    TestEqual(TEXT("Runtime marker count matches profile minimap markers."), RuntimeMarkers.Num(), Profile.MinimapMarkers.Num());

    auto RuntimeMeshHasTags = [&RuntimeMeshes](FName SpecId, FName AssetRoleId)
    {
        return FindRuntimeMeshWithTags(RuntimeMeshes, SpecId, AssetRoleId) != nullptr;
    };

    bool bFoundBlockingLandmark = false;
    bool bFoundBlockingScenic = false;
    bool bFoundNonBlockingScenic = false;
    bool bFoundDawnwatchGateComponent = false;
    bool bFoundRoadLampRole = false;
    bool bFoundTreeRole = false;

    for (const TObjectPtr<UStaticMeshComponent>& RuntimeMesh : RuntimeMeshes)
    {
        const UStaticMeshComponent* MeshComponent = RuntimeMesh.Get();
        if (MeshComponent == nullptr)
        {
            continue;
        }

        const FString ComponentName = MeshComponent->GetName();
        const bool bIsLandmark = ComponentName.StartsWith(TEXT("Landmark_"));
        const bool bIsScenic = ComponentName.StartsWith(TEXT("Scenic_"));
        const ECollisionEnabled::Type CollisionEnabled = MeshComponent->GetCollisionEnabled();

        TestTrue(TEXT("Runtime mesh component is registered."), MeshComponent->IsRegistered());
        TestTrue(TEXT("Runtime mesh component is owned by scaffold."), MeshComponent->GetOwner() == Scaffold);
        TestTrue(TEXT("Runtime mesh component is attached to scaffold root."), MeshComponent->GetAttachParent() == Scaffold->GetRootComponent());

        bFoundBlockingLandmark |= bIsLandmark && CollisionEnabled == ECollisionEnabled::QueryAndPhysics;
        bFoundBlockingScenic |= bIsScenic && CollisionEnabled == ECollisionEnabled::QueryAndPhysics;
        bFoundNonBlockingScenic |= bIsScenic && CollisionEnabled == ECollisionEnabled::NoCollision;
        bFoundDawnwatchGateComponent |= ComponentName.Contains(TEXT("Landmark_landmark.dawnwatch.gate"));
        bFoundRoadLampRole |= MeshComponent->ComponentTags.Contains(FName(TEXT("zone.road.lamp")));
        bFoundTreeRole |= MeshComponent->ComponentTags.Contains(FName(TEXT("zone.foliage.tree")));
    }

    for (const FEOZoneVisualSpec& Landmark : Profile.Landmarks)
    {
        FEOAssetRoleDefinition AssetRole;
        TestTrue(TEXT("Each runtime landmark references a valid asset role."), FEOAssetRoleCatalog::TryGetRoleDefinition(Landmark.AssetRoleId, AssetRole));
        TestTrue(TEXT("Each runtime landmark mesh is tagged with spec and role ids."), RuntimeMeshHasTags(Landmark.Id, Landmark.AssetRoleId));
    }

    for (const FEOZoneVisualSpec& ScenicProp : Profile.ScenicProps)
    {
        FEOAssetRoleDefinition AssetRole;
        TestTrue(TEXT("Each runtime scenic prop references a valid asset role."), FEOAssetRoleCatalog::TryGetRoleDefinition(ScenicProp.AssetRoleId, AssetRole));
        TestTrue(TEXT("Each runtime scenic mesh is tagged with spec and role ids."), RuntimeMeshHasTags(ScenicProp.Id, ScenicProp.AssetRoleId));
    }

    const FEOZoneVisualSpec* ProfileTintVisual = Profile.Landmarks.FindByPredicate([](const FEOZoneVisualSpec& VisualSpec)
    {
        return VisualSpec.Id == TEXT("landmark.dawnwatch.gate");
    });
    TestNotNull(TEXT("Profile tint assertion target exists."), ProfileTintVisual);

    if (ProfileTintVisual != nullptr)
    {
        FEOAssetRoleDefinition AssetRole;
        const bool bRoleFound = FEOAssetRoleCatalog::TryGetRoleDefinition(ProfileTintVisual->AssetRoleId, AssetRole);
        TestTrue(TEXT("Profile tint assertion target role resolves."), bRoleFound);

        const UStaticMeshComponent* ProfileTintMesh = FindRuntimeMeshWithTags(RuntimeMeshes, ProfileTintVisual->Id, ProfileTintVisual->AssetRoleId);
        TestNotNull(TEXT("Profile tint assertion target mesh exists."), ProfileTintMesh);

        if (ProfileTintMesh != nullptr && bRoleFound)
        {
            UMaterialInstanceDynamic* DynamicMaterial = Cast<UMaterialInstanceDynamic>(ProfileTintMesh->GetMaterial(0));
            TestNotNull(TEXT("Profile-tinted mesh uses dynamic material."), DynamicMaterial);
            if (DynamicMaterial != nullptr)
            {
                const FLinearColor RuntimeTint = DynamicMaterial->K2_GetVectorParameterValue(TEXT("Color"));
                TestTrue(TEXT("Runtime mesh color uses profile tint."), AreColorsNearlyEqual(RuntimeTint, ProfileTintVisual->Tint));
                TestFalse(TEXT("Runtime mesh color is not overridden by role debug tint."), AreColorsNearlyEqual(RuntimeTint, AssetRole.DebugTint));
            }
        }
    }

    for (const FEOMinimapMarkerSpec& Marker : Profile.MinimapMarkers)
    {
        bool bFoundMarker = false;
        for (const TObjectPtr<UStaticMeshComponent>& RuntimeMarker : RuntimeMarkers)
        {
            const UStaticMeshComponent* MarkerComponent = RuntimeMarker.Get();
            if (MarkerComponent != nullptr && MarkerComponent->ComponentTags.Contains(Marker.MarkerId))
            {
                bFoundMarker = true;
                TestEqual(TEXT("Runtime marker collision is disabled."), MarkerComponent->GetCollisionEnabled(), ECollisionEnabled::NoCollision);
                break;
            }
        }

        TestTrue(TEXT("Each minimap marker is generated and tagged."), bFoundMarker);
    }

    TestTrue(TEXT("Runtime world has blocking landmark collision."), bFoundBlockingLandmark);
    TestTrue(TEXT("Runtime world has blocking scenic collision."), bFoundBlockingScenic);
    TestTrue(TEXT("Runtime world has non-blocking scenic collision."), bFoundNonBlockingScenic);
    TestTrue(TEXT("Runtime world names generated landmark components."), bFoundDawnwatchGateComponent);
    TestTrue(TEXT("Runtime world tags generated road lamp role."), bFoundRoadLampRole);
    TestTrue(TEXT("Runtime world tags generated tree role."), bFoundTreeRole);

    DestroyRuntimeScaffoldTestWorld(TestWorld);
    return true;
}

#endif
