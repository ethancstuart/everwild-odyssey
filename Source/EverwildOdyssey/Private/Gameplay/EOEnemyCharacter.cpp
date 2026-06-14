#include "Gameplay/EOEnemyCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Gameplay/EOCombatStatsComponent.h"
#include "Gameplay/EOHeroCharacter.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

namespace
{
void ApplyTint(UStaticMeshComponent* MeshComponent, UMaterialInterface* BaseMaterial, const FLinearColor& TintColor)
{
    if (MeshComponent == nullptr || BaseMaterial == nullptr)
    {
        return;
    }

    UMaterialInstanceDynamic* EnemyMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, MeshComponent);
    EnemyMaterial->SetVectorParameterValue(TEXT("Color"), TintColor);
    MeshComponent->SetMaterial(0, EnemyMaterial);
}
}

AEOEnemyCharacter::AEOEnemyCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

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

    EnemyHalo = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EnemyHalo"));
    EnemyHalo->SetupAttachment(RootComponent);
    EnemyHalo->SetRelativeLocation(FVector(0.0f, 0.0f, -78.0f));
    EnemyHalo->SetRelativeScale3D(FVector(1.2f, 1.2f, 0.035f));
    EnemyHalo->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderMesh(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> BasicShapeMaterial(TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
    if (SphereMesh.Succeeded())
    {
        EnemySilhouette->SetStaticMesh(SphereMesh.Object);
        if (BasicShapeMaterial.Succeeded())
        {
            ApplyTint(EnemySilhouette, BasicShapeMaterial.Object, FLinearColor(1.0f, 0.22f, 0.34f, 1.0f));
        }
    }

    if (CylinderMesh.Succeeded())
    {
        EnemyHalo->SetStaticMesh(CylinderMesh.Object);
        if (BasicShapeMaterial.Succeeded())
        {
            ApplyTint(EnemyHalo, BasicShapeMaterial.Object, FLinearColor(1.0f, 0.12f, 0.50f, 1.0f));
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
    Archetype.TintColor = FLinearColor(1.0f, 0.22f, 0.34f, 1.0f);
    Archetype.SilhouetteScale = FVector(0.65f, 0.65f, 1.1f);
    Archetype.ContactDamage = 5.0f;
    Archetype.ContactRange = 135.0f;
    Archetype.AttackCooldownSeconds = 1.25f;
    return Archetype;
}

FEOEnemyArchetype AEOEnemyCharacter::BuildRelicSentinelArchetype()
{
    FEOEnemyArchetype Archetype;
    Archetype.EnemyId = TEXT("enemy.starfall.relic_sentinel");
    Archetype.DisplayName = FText::FromString(TEXT("Relic Sentinel"));
    Archetype.BaseStats.MaxHealth = 88.0f;
    Archetype.BaseStats.MaxResource = 50.0f;
    Archetype.BaseStats.AttackPower = 14.0f;
    Archetype.BaseStats.SpellPower = 18.0f;
    Archetype.BaseStats.Armor = 3.0f;
    Archetype.BaseStats.MoveSpeed = 270.0f;
    Archetype.ExperienceReward = 32;
    Archetype.ThreatRadius = 1050.0f;
    Archetype.bElite = true;
    Archetype.TintColor = FLinearColor(0.86f, 0.18f, 1.0f, 1.0f);
    Archetype.SilhouetteScale = FVector(1.05f, 1.05f, 1.65f);
    Archetype.ContactDamage = 9.0f;
    Archetype.ContactRange = 165.0f;
    Archetype.AttackCooldownSeconds = 1.45f;
    return Archetype;
}

void AEOEnemyCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    TryContactAttack(DeltaSeconds);
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
    ContactDamage = Archetype.ContactDamage;
    ContactRange = Archetype.ContactRange;
    AttackCooldownSeconds = Archetype.AttackCooldownSeconds;
    TimeUntilNextContactDamage = 0.0f;
    bExperienceRewardClaimed = false;
    CombatStats->InitializeFromStats(Archetype.BaseStats);
    GetCharacterMovement()->MaxWalkSpeed = Archetype.BaseStats.MoveSpeed;
    GetCharacterMovement()->SetMovementMode(MOVE_Walking);
    SetActorEnableCollision(true);
    ApplyEnemyVisuals(Archetype);
}

float AEOEnemyCharacter::ApplyIncomingHit(float RawDamage)
{
    const float AppliedDamage = CombatStats->ApplyDamage(RawDamage);
    if (!CombatStats->IsAlive())
    {
        EnemySilhouette->SetVisibility(false);
        EnemyHalo->SetVisibility(false);
        SetActorEnableCollision(false);
        GetCharacterMovement()->DisableMovement();
    }

    return AppliedDamage;
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

void AEOEnemyCharacter::ApplyEnemyVisuals(const FEOEnemyArchetype& Archetype)
{
    EnemySilhouette->SetVisibility(true);
    EnemyHalo->SetVisibility(true);
    EnemySilhouette->SetRelativeScale3D(Archetype.SilhouetteScale);
    EnemyHalo->SetRelativeScale3D(FVector(Archetype.SilhouetteScale.X * 1.65f, Archetype.SilhouetteScale.Y * 1.65f, 0.035f));

    UMaterialInterface* BasicShapeMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
    if (BasicShapeMaterial != nullptr)
    {
        ApplyTint(EnemySilhouette, BasicShapeMaterial, Archetype.TintColor);
        ApplyTint(EnemyHalo, BasicShapeMaterial, FLinearColor(Archetype.TintColor.R, Archetype.TintColor.G * 0.55f, Archetype.TintColor.B, 1.0f));
    }
}

void AEOEnemyCharacter::TryContactAttack(float DeltaSeconds)
{
    TimeUntilNextContactDamage = FMath::Max(0.0f, TimeUntilNextContactDamage - DeltaSeconds);
    if (CombatStats == nullptr || !CombatStats->IsAlive())
    {
        return;
    }

    AEOHeroCharacter* Hero = Cast<AEOHeroCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
    if (Hero == nullptr || Hero->GetCombatStatsComponent() == nullptr || !Hero->GetCombatStatsComponent()->IsAlive())
    {
        return;
    }

    const FVector ToHero = Hero->GetActorLocation() - GetActorLocation();
    const float DistanceSquared = ToHero.SizeSquared2D();
    if (DistanceSquared <= FMath::Square(ThreatRadius))
    {
        const FVector Direction = ToHero.GetSafeNormal2D();
        AddMovementInput(Direction, 0.72f);
    }

    if (DistanceSquared <= FMath::Square(ContactRange) && TimeUntilNextContactDamage <= 0.0f)
    {
        Hero->GetCombatStatsComponent()->ApplyDamage(ContactDamage);
        TimeUntilNextContactDamage = AttackCooldownSeconds;
        UE_LOG(LogTemp, Log, TEXT("%s lashes the hero for %.1f"), *EnemyId.ToString(), ContactDamage);
    }
}
