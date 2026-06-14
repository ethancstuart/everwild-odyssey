#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EOHeroCharacter.generated.h"

class UCameraComponent;
class UEOAbilityRuntimeComponent;
class UEOCombatStatsComponent;
class UEOQuestLogComponent;
class USpringArmComponent;
class UStaticMeshComponent;

UCLASS()
class EVERWILDODYSSEY_API AEOHeroCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    static constexpr float DefaultCameraBoomLength = 1200.0f;
    static constexpr float DefaultCameraPitch = -60.0f;
    static constexpr float DefaultCameraYaw = -45.0f;

    AEOHeroCharacter();

    UEOCombatStatsComponent* GetCombatStatsComponent() const { return CombatStats; }
    UEOAbilityRuntimeComponent* GetAbilityRuntimeComponent() const { return AbilityRuntime; }
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

    void MoveForward(float Value);
    void MoveRight(float Value);
    void BasicAttack();
    void Dodge();
    void BlockPressed();
    void BlockReleased();
    void Interact();
    void AbilityOne();
    void AbilityTwo();
};
