#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EOAlphaWorldScaffold.generated.h"

class USceneComponent;
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

    bool IsValidForAlpha() const
    {
        return !LandmarkId.IsNone()
            && !DisplayName.IsEmpty()
            && Scale.X > 0.0f
            && Scale.Y > 0.0f
            && Scale.Z > 0.0f;
    }
};

UCLASS()
class EVERWILDODYSSEY_API AEOAlphaWorldScaffold : public AActor
{
    GENERATED_BODY()

public:
    static constexpr int32 ExpectedLandmarkCount = 6;

    AEOAlphaWorldScaffold();

    static TArray<FEOAlphaLandmarkSpec> BuildDefaultLandmarks();

private:
    UPROPERTY(VisibleAnywhere, Category = "Alpha World")
    TObjectPtr<USceneComponent> SceneRoot;

    UPROPERTY(VisibleAnywhere, Category = "Alpha World")
    TObjectPtr<UStaticMeshComponent> DawnwatchKeep;

    UPROPERTY(VisibleAnywhere, Category = "Alpha World")
    TObjectPtr<UStaticMeshComponent> StarfallGate;

    UPROPERTY(VisibleAnywhere, Category = "Alpha World")
    TObjectPtr<UStaticMeshComponent> TrainingRing;

    UPROPERTY(VisibleAnywhere, Category = "Alpha World")
    TObjectPtr<UStaticMeshComponent> RelicSurge;

    UPROPERTY(VisibleAnywhere, Category = "Alpha World")
    TObjectPtr<UStaticMeshComponent> ValeRoad;

    UPROPERTY(VisibleAnywhere, Category = "Alpha World")
    TObjectPtr<UStaticMeshComponent> SkybridgeHub;
};
