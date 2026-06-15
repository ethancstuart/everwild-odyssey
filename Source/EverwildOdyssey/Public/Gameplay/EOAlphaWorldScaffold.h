#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "World/EOZoneProfile.h"
#include "EOAlphaWorldScaffold.generated.h"

class USceneComponent;
class UEOEncounterDirectorComponent;
class UExponentialHeightFogComponent;
class UPointLightComponent;
class UPostProcessComponent;
class USkyLightComponent;
class UStaticMeshComponent;

UCLASS()
class EVERWILDODYSSEY_API AEOAlphaWorldScaffold : public AActor
{
    GENERATED_BODY()

public:
    static int32 ExpectedLandmarkCount();
    static int32 ExpectedScenicPropCount();
    static int32 ExpectedShowcaseAssemblyPartCount();
    static int32 ExpectedAmbientLightCount();
    static int32 ExpectedMinimapMarkerCount();

    AEOAlphaWorldScaffold();

    static TArray<FEOZoneVisualSpec> BuildDefaultLandmarks();
    static TArray<FEOZoneVisualSpec> BuildDefaultScenicProps();
    static TArray<FEOZoneLightSpec> BuildDefaultAmbientLights();
    static TArray<FEOMinimapMarkerSpec> BuildDefaultMinimapMarkers();
    static FEOZoneProfile BuildDefaultZoneProfile();

    UEOEncounterDirectorComponent* GetEncounterDirectorComponent() const { return EncounterDirector; }

#if WITH_DEV_AUTOMATION_TESTS
    const TArray<TObjectPtr<UStaticMeshComponent>>& GetRuntimeWorldMeshesForTesting() const { return RuntimeWorldMeshes; }
    const TArray<TObjectPtr<UPointLightComponent>>& GetRuntimeWorldLightsForTesting() const { return RuntimeWorldLights; }
    const TArray<TObjectPtr<UStaticMeshComponent>>& GetRuntimeWorldMarkerMeshesForTesting() const { return RuntimeWorldMarkerMeshes; }
#endif

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(VisibleAnywhere, Category = "Alpha World")
    TObjectPtr<USceneComponent> SceneRoot;

    UPROPERTY(VisibleAnywhere, Category = "Alpha World")
    TObjectPtr<UEOEncounterDirectorComponent> EncounterDirector;

    UPROPERTY(VisibleAnywhere, Category = "Alpha World")
    TObjectPtr<USkyLightComponent> ValeSkyLight;

    UPROPERTY(VisibleAnywhere, Category = "Alpha World")
    TObjectPtr<UExponentialHeightFogComponent> ValeFog;

    UPROPERTY(VisibleAnywhere, Category = "Alpha World")
    TObjectPtr<UPostProcessComponent> ValeColorGrade;

    UPROPERTY(VisibleAnywhere, Category = "Alpha World")
    TArray<TObjectPtr<UStaticMeshComponent>> RuntimeWorldMeshes;

    UPROPERTY(VisibleAnywhere, Category = "Alpha World")
    TArray<TObjectPtr<UPointLightComponent>> RuntimeWorldLights;

    UPROPERTY(VisibleAnywhere, Category = "Alpha World")
    TArray<TObjectPtr<UStaticMeshComponent>> RuntimeWorldMarkerMeshes;

    void SpawnVerticalSliceWorld();
};
