#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EOAlphaWorldScaffold.generated.h"

class USceneComponent;
class UEOEncounterDirectorComponent;
class UExponentialHeightFogComponent;
class UPointLightComponent;
class UPostProcessComponent;
class USkyLightComponent;
class UStaticMeshComponent;

USTRUCT(BlueprintType)
struct FEOAlphaLandmarkSpec
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Alpha World")
    FName LandmarkId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Alpha World")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Alpha World")
    FVector Location = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Alpha World")
    FVector Scale = FVector::OneVector;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Alpha World")
    FLinearColor TintColor = FLinearColor::White;

    bool IsValidForAlpha() const
    {
        return !LandmarkId.IsNone()
            && !DisplayName.IsEmpty()
            && Scale.X > 0.0f
            && Scale.Y > 0.0f
            && Scale.Z > 0.0f
            && TintColor.A > 0.0f;
    }
};

USTRUCT(BlueprintType)
struct FEOAlphaScenicPropSpec
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Alpha World")
    FName PropId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Alpha World")
    FName VisualStyle = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Alpha World")
    FVector Location = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Alpha World")
    FRotator Rotation = FRotator::ZeroRotator;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Alpha World")
    FVector Scale = FVector::OneVector;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Alpha World")
    FLinearColor TintColor = FLinearColor::White;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Alpha World")
    bool bBlocksMovement = true;

    bool IsValidForAlpha() const
    {
        return !PropId.IsNone()
            && !VisualStyle.IsNone()
            && Scale.X > 0.0f
            && Scale.Y > 0.0f
            && Scale.Z > 0.0f
            && TintColor.A > 0.0f;
    }
};

USTRUCT(BlueprintType)
struct FEOAlphaLightSpec
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Alpha World")
    FName LightId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Alpha World")
    FVector Location = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Alpha World")
    FLinearColor Color = FLinearColor::White;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Alpha World")
    float Intensity = 2500.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Alpha World")
    float Radius = 700.0f;

    bool IsValidForAlpha() const
    {
        return !LightId.IsNone()
            && Color.A > 0.0f
            && Intensity > 0.0f
            && Radius > 0.0f;
    }
};

UCLASS()
class EVERWILDODYSSEY_API AEOAlphaWorldScaffold : public AActor
{
    GENERATED_BODY()

public:
    static constexpr int32 ExpectedLandmarkCount = 14;
    static constexpr int32 ExpectedScenicPropCount = 160;
    static constexpr int32 ExpectedAmbientLightCount = 16;

    AEOAlphaWorldScaffold();

    static TArray<FEOAlphaLandmarkSpec> BuildDefaultLandmarks();
    static TArray<FEOAlphaScenicPropSpec> BuildDefaultScenicProps();
    static TArray<FEOAlphaLightSpec> BuildDefaultAmbientLights();

    UEOEncounterDirectorComponent* GetEncounterDirectorComponent() const { return EncounterDirector; }

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

    void SpawnVerticalSliceWorld();
};
