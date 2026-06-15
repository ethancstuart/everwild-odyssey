#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EOHeroCharacter.generated.h"

class UCameraComponent;
class UEOAbilityRuntimeComponent;
class UEOCompanionRosterComponent;
class UEOCombatStatsComponent;
class UEOHeroProgressionComponent;
class UEOInventoryComponent;
class UEOQuestLogComponent;
class USpringArmComponent;
class UStaticMeshComponent;
class AEOEnemyCharacter;

UCLASS()
class EVERWILDODYSSEY_API AEOHeroCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    static constexpr float DefaultCameraBoomLength = 680.0f;
    static constexpr float DefaultCameraPitch = -22.0f;
    static constexpr float DefaultCameraYaw = -35.0f;
    static constexpr float DefaultBasicAttackRange = 300.0f;
    static constexpr float DefaultAbilityAttackRange = 480.0f;
    static constexpr int32 RequiredRelicShardsForGate = 3;

    AEOHeroCharacter();

    UEOCombatStatsComponent* GetCombatStatsComponent() const { return CombatStats; }
    UEOAbilityRuntimeComponent* GetAbilityRuntimeComponent() const { return AbilityRuntime; }
    UEOCompanionRosterComponent* GetCompanionRosterComponent() const { return CompanionRoster; }
    UEOHeroProgressionComponent* GetProgressionComponent() const { return Progression; }
    UEOInventoryComponent* GetInventoryComponent() const { return Inventory; }
    UEOQuestLogComponent* GetQuestLogComponent() const { return QuestLog; }
    int32 GetRelicShardCount() const { return RelicShardCount; }
    int32 GetRequiredRelicShardCount() const { return RequiredRelicShardsForGate; }
    bool IsStarfallGateStabilized() const { return bStarfallGateStabilized; }

protected:
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
    UPROPERTY(VisibleAnywhere, Category = "Camera")
    TObjectPtr<USpringArmComponent> CameraBoom;

    UPROPERTY(VisibleAnywhere, Category = "Camera")
    TObjectPtr<UCameraComponent> FollowCamera;

    UPROPERTY(VisibleAnywhere, Category = "Presentation")
    TObjectPtr<UStaticMeshComponent> HeroSilhouette;

    UPROPERTY(VisibleAnywhere, Category = "Presentation")
    TObjectPtr<UStaticMeshComponent> HeroHead;

    UPROPERTY(VisibleAnywhere, Category = "Presentation")
    TObjectPtr<UStaticMeshComponent> HeroCape;

    UPROPERTY(VisibleAnywhere, Category = "Presentation")
    TObjectPtr<UStaticMeshComponent> HeroSword;

    UPROPERTY(VisibleAnywhere, Category = "Presentation")
    TObjectPtr<UStaticMeshComponent> HeroShield;

    UPROPERTY(VisibleAnywhere, Category = "Presentation")
    TObjectPtr<UStaticMeshComponent> MiraSilhouette;

    UPROPERTY(VisibleAnywhere, Category = "Presentation")
    TObjectPtr<UStaticMeshComponent> TorSilhouette;

    UPROPERTY(VisibleAnywhere, Category = "Combat")
    TObjectPtr<UEOCombatStatsComponent> CombatStats;

    UPROPERTY(VisibleAnywhere, Category = "Combat")
    TObjectPtr<UEOAbilityRuntimeComponent> AbilityRuntime;

    UPROPERTY(VisibleAnywhere, Category = "Progression")
    TObjectPtr<UEOQuestLogComponent> QuestLog;

    UPROPERTY(VisibleAnywhere, Category = "Progression")
    TObjectPtr<UEOHeroProgressionComponent> Progression;

    UPROPERTY(VisibleAnywhere, Category = "Progression")
    TObjectPtr<UEOCompanionRosterComponent> CompanionRoster;

    UPROPERTY(VisibleAnywhere, Category = "Progression")
    TObjectPtr<UEOInventoryComponent> Inventory;

    UPROPERTY(VisibleAnywhere, Category = "Progression")
    int32 RelicShardCount = 0;

    UPROPERTY(VisibleAnywhere, Category = "Progression")
    bool bStarfallGateStabilized = false;

    void MoveForward(float Value);
    void MoveRight(float Value);
    void CameraYaw(float Value);
    void CameraPitch(float Value);
    void BasicAttack();
    AEOEnemyCharacter* FindNearestEnemyInRange(float Range) const;
    void ActivateAbilityAndDamageNearestEnemy(int32 AbilityIndex, bool bUltimate);
    void AwardEnemyIfDefeated(AEOEnemyCharacter* Enemy);
    void TargetCycle();
    void Dodge();
    void BlockPressed();
    void BlockReleased();
    void Interact();
    void AbilityOne();
    void AbilityTwo();
    void RefreshStarfallQuestProgress();
};
