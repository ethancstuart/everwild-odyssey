#include "Gameplay/EOEnemyCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Gameplay/EOCombatStatsComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"

AEOEnemyCharacter::AEOEnemyCharacter()
{
    PrimaryActorTick.bCanEverTick = false;

    GetCapsuleComponent()->InitCapsuleSize(34.0f, 72.0f);

    UCharacterMovementComponent* Movement = GetCharacterMovement();
    Movement->MaxWalkSpeed = 360.0f;
    Movement->bOrientRotationToMovement = true;
    Movement->RotationRate = FRotator(0.0f, 540.0f, 0.0f);

    CombatStats = CreateDefaultSubobject<UEOCombatStatsComponent>(TEXT("CombatStats"));

    EnemySilhouette = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EnemySilhouette"));
    EnemySilhouette->SetupAttachment(RootComponent);
    EnemySilhouette->SetRelativeLocation(FVector(0.0f, 0.0f, -22.0f));
    EnemySilhouette->SetRelativeScale3D(FVector(0.65f, 0.65f, 1.1f));
    EnemySilhouette->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> BasicShapeMaterial(TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
    if (SphereMesh.Succeeded())
    {
        EnemySilhouette->SetStaticMesh(SphereMesh.Object);
        if (BasicShapeMaterial.Succeeded())
        {
            UMaterialInstanceDynamic* EnemyMaterial = UMaterialInstanceDynamic::Create(BasicShapeMaterial.Object, EnemySilhouette);
            EnemyMaterial->SetVectorParameterValue(TEXT("Color"), FLinearColor(1.0f, 0.22f, 0.34f, 1.0f));
            EnemySilhouette->SetMaterial(0, EnemyMaterial);
        }
    }

    InitializeFromArchetype(BuildRelicWispArchetype());
}

FEOEnemyArchetype AEOEnemyCharacter::BuildRelicWispArchetype()
{
    FEOEnemyArchetype Archetype;
    Archetype.EnemyId = TEXT("enemy.starfall.relic_wisp");
    Archetype.DisplayName = FText::FromString(TEXT("Relic Wisp"));
    Archetype.BaseStats.MaxHealth = 42.0f;
    Archetype.BaseStats.MaxResource = 35.0f;
    Archetype.BaseStats.AttackPower = 8.0f;
    Archetype.BaseStats.SpellPower = 12.0f;
    Archetype.BaseStats.Armor = 1.0f;
    Archetype.BaseStats.MoveSpeed = 360.0f;
    Archetype.ExperienceReward = 14;
    Archetype.ThreatRadius = 850.0f;
    Archetype.bElite = false;
    return Archetype;
}

void AEOEnemyCharacter::InitializeFromArchetype(const FEOEnemyArchetype& Archetype)
{
    if (!Archetype.IsValidForAlpha())
    {
        return;
    }

    EnemyId = Archetype.EnemyId;
    ExperienceReward = Archetype.ExperienceReward;
    ThreatRadius = Archetype.ThreatRadius;
    bExperienceRewardClaimed = false;
    CombatStats->InitializeFromStats(Archetype.BaseStats);
    GetCharacterMovement()->MaxWalkSpeed = Archetype.BaseStats.MoveSpeed;
}

float AEOEnemyCharacter::ApplyIncomingHit(float RawDamage)
{
    return CombatStats->ApplyDamage(RawDamage);
}

int32 AEOEnemyCharacter::ClaimExperienceReward()
{
    if (bExperienceRewardClaimed || CombatStats->IsAlive())
    {
        return 0;
    }

    bExperienceRewardClaimed = true;
    return ExperienceReward;
}
