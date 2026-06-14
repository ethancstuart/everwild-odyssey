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
    static constexpr float DefaultCameraBoomLength = 1200.0f;
    static constexpr float DefaultCameraPitch = -60.0f;
    static constexpr float DefaultCameraYaw = -45.0f;
    static constexpr float DefaultBasicAttackRange = 260.0f;
    static constexpr float DefaultAbilityAttackRange = 420.0f;

    AEOHeroCharacter();

    UEOCombatStatsComponent* GetCombatStatsComponent() const { return CombatStats; }
    UEOAbilityRuntimeComponent* GetAbilityRuntimeComponent() const { return AbilityRuntime; }
    UEOCompanionRosterComponent* GetCompanionRosterComponent() const { return CompanionRoster; }
    UEOHeroProgressionComponent* GetProgressionComponent() const { return Progression; }
    UEOInventoryComponent* GetInventoryComponent() const { return Inventory; }
    UEOQuestLogComponent* GetQuestLogComponent() const { return QuestLog; }

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

    void MoveForward(float Value);
    void MoveRight(float Value);
    void BasicAttack();
    AEOEnemyCharacter* FindNearestEnemyInRange(float Range) const;
    void ActivateAbilityAndDamageNearestEnemy(int32 AbilityIndex, bool bUltimate);
    void AwardEnemyIfDefeated(AEOEnemyCharacter* Enemy);
    void Dodge();
    void BlockPressed();
    void BlockReleased();
    void Interact();
    void AbilityOne();
    void AbilityTwo();
};
