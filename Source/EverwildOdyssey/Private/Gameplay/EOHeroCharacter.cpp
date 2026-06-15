#include "Gameplay/EOHeroCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Core/EOHeroClassDefinition.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Gameplay/EOAbilityRuntimeComponent.h"
#include "Gameplay/EOCompanionRosterComponent.h"
#include "Gameplay/EOCombatResolution.h"
#include "Gameplay/EOCombatStatsComponent.h"
#include "Gameplay/EOEnemyCharacter.h"
#include "Gameplay/EOHeroProgressionComponent.h"
#include "Gameplay/EOInventoryComponent.h"
#include "Gameplay/EOQuestLogComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"

namespace
{
void ApplyHeroTint(UStaticMeshComponent* MeshComponent, UMaterialInterface* BaseMaterial, const FLinearColor& TintColor)
{
    if (MeshComponent == nullptr || BaseMaterial == nullptr)
    {
        return;
    }

    UMaterialInstanceDynamic* Material = UMaterialInstanceDynamic::Create(BaseMaterial, MeshComponent);
    Material->SetVectorParameterValue(TEXT("Color"), TintColor);
    MeshComponent->SetMaterial(0, Material);
}
}

AEOHeroCharacter::AEOHeroCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    UCharacterMovementComponent* Movement = GetCharacterMovement();
    Movement->bOrientRotationToMovement = true;
    Movement->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
    Movement->MaxWalkSpeed = 620.0f;
    Movement->BrakingDecelerationWalking = 1800.0f;
    Movement->JumpZVelocity = 520.0f;
    Movement->AirControl = 0.35f;

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("HeroicThirdPersonCameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = DefaultCameraBoomLength;
    CameraBoom->SetRelativeRotation(FRotator(DefaultCameraPitch, DefaultCameraYaw, 0.0f));
    CameraBoom->bDoCollisionTest = false;
    CameraBoom->bUsePawnControlRotation = false;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("HeroicThirdPersonCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    HeroSilhouette = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeroSilhouette"));
    HeroSilhouette->SetupAttachment(RootComponent);
    HeroSilhouette->SetRelativeLocation(FVector(0.0f, 0.0f, -32.0f));
    HeroSilhouette->SetRelativeScale3D(FVector(1.05f, 0.82f, 1.72f));
    HeroSilhouette->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    HeroHead = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeroHead"));
    HeroHead->SetupAttachment(RootComponent);
    HeroHead->SetRelativeLocation(FVector(0.0f, 0.0f, 112.0f));
    HeroHead->SetRelativeScale3D(FVector(0.42f, 0.42f, 0.42f));
    HeroHead->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    HeroCape = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeroCape"));
    HeroCape->SetupAttachment(RootComponent);
    HeroCape->SetRelativeLocation(FVector(-48.0f, -38.0f, 18.0f));
    HeroCape->SetRelativeRotation(FRotator(-4.0f, -18.0f, -8.0f));
    HeroCape->SetRelativeScale3D(FVector(0.12f, 0.70f, 1.46f));
    HeroCape->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    HeroSword = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeroSword"));
    HeroSword->SetupAttachment(RootComponent);
    HeroSword->SetRelativeLocation(FVector(68.0f, 28.0f, 20.0f));
    HeroSword->SetRelativeRotation(FRotator(0.0f, 28.0f, -32.0f));
    HeroSword->SetRelativeScale3D(FVector(0.10f, 0.12f, 1.72f));
    HeroSword->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    HeroShield = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeroShield"));
    HeroShield->SetupAttachment(RootComponent);
    HeroShield->SetRelativeLocation(FVector(-70.0f, 20.0f, 28.0f));
    HeroShield->SetRelativeRotation(FRotator(0.0f, -18.0f, 0.0f));
    HeroShield->SetRelativeScale3D(FVector(0.46f, 0.14f, 0.78f));
    HeroShield->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    MiraSilhouette = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MiraSilhouette"));
    MiraSilhouette->SetupAttachment(RootComponent);
    MiraSilhouette->SetRelativeLocation(FVector(-82.0f, -78.0f, -54.0f));
    MiraSilhouette->SetRelativeScale3D(FVector(0.42f, 0.42f, 0.92f));
    MiraSilhouette->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    TorSilhouette = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TorSilhouette"));
    TorSilhouette->SetupAttachment(RootComponent);
    TorSilhouette->SetRelativeLocation(FVector(88.0f, -88.0f, -50.0f));
    TorSilhouette->SetRelativeScale3D(FVector(0.58f, 0.58f, 1.05f));
    TorSilhouette->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderMesh(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> BasicShapeMaterial(TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));

    // These meshes are explicit hero proxies. The asset resolver reports whether final role assets are present.
    if (CylinderMesh.Succeeded())
    {
        HeroSilhouette->SetStaticMesh(CylinderMesh.Object);
        if (BasicShapeMaterial.Succeeded())
        {
            ApplyHeroTint(HeroSilhouette, BasicShapeMaterial.Object, FLinearColor(0.12f, 0.54f, 1.0f, 1.0f));
        }
    }

    if (SphereMesh.Succeeded())
    {
        HeroHead->SetStaticMesh(SphereMesh.Object);
        MiraSilhouette->SetStaticMesh(SphereMesh.Object);
        TorSilhouette->SetStaticMesh(SphereMesh.Object);
        if (BasicShapeMaterial.Succeeded())
        {
            ApplyHeroTint(HeroHead, BasicShapeMaterial.Object, FLinearColor(0.96f, 0.78f, 0.58f, 1.0f));
            ApplyHeroTint(MiraSilhouette, BasicShapeMaterial.Object, FLinearColor(0.32f, 0.92f, 0.72f, 1.0f));
            ApplyHeroTint(TorSilhouette, BasicShapeMaterial.Object, FLinearColor(1.0f, 0.62f, 0.18f, 1.0f));
        }
    }

    if (CubeMesh.Succeeded())
    {
        HeroCape->SetStaticMesh(CubeMesh.Object);
        HeroSword->SetStaticMesh(CubeMesh.Object);
        HeroShield->SetStaticMesh(CubeMesh.Object);
        if (BasicShapeMaterial.Succeeded())
        {
            ApplyHeroTint(HeroCape, BasicShapeMaterial.Object, FLinearColor(0.08f, 0.20f, 0.86f, 1.0f));
            ApplyHeroTint(HeroSword, BasicShapeMaterial.Object, FLinearColor(1.0f, 0.86f, 0.34f, 1.0f));
            ApplyHeroTint(HeroShield, BasicShapeMaterial.Object, FLinearColor(0.18f, 0.36f, 0.92f, 1.0f));
        }
    }

    CombatStats = CreateDefaultSubobject<UEOCombatStatsComponent>(TEXT("CombatStats"));
    AbilityRuntime = CreateDefaultSubobject<UEOAbilityRuntimeComponent>(TEXT("AbilityRuntime"));
    QuestLog = CreateDefaultSubobject<UEOQuestLogComponent>(TEXT("QuestLog"));
    Progression = CreateDefaultSubobject<UEOHeroProgressionComponent>(TEXT("Progression"));
    CompanionRoster = CreateDefaultSubobject<UEOCompanionRosterComponent>(TEXT("CompanionRoster"));
    Inventory = CreateDefaultSubobject<UEOInventoryComponent>(TEXT("Inventory"));
}

void AEOHeroCharacter::BeginPlay()
{
    Super::BeginPlay();

    FEOHeroClassDefinition DefaultClass;
    if (FEOWildClassCatalog::TryGetClassDefinition(EEOHeroClass::Runeblade, DefaultClass))
    {
        CombatStats->InitializeFromStats(DefaultClass.BaseStats);
        AbilityRuntime->InitializeAbilities(DefaultClass.StartingAbilities);
        GetCharacterMovement()->MaxWalkSpeed = DefaultClass.BaseStats.MoveSpeed;
    }

    FEOQuestRecord ArrivalQuest;
    ArrivalQuest.QuestId = TEXT("dawnwatch.starfall_arrival");
    ArrivalQuest.DisplayName = FText::FromString(TEXT("The Starfall Gate"));
    ArrivalQuest.State = EEOQuestState::Available;
    ArrivalQuest.ReportedToFaction = EEOFaction::DawnwatchCommand;

    FEOWorldEventRecord RelicSurge;
    RelicSurge.EventId = TEXT("starfall.relic_surge");
    RelicSurge.DisplayName = FText::FromString(TEXT("Relic Surge"));
    RelicSurge.State = EEOWorldEventState::Warning;
    RelicSurge.RespawnSeconds = 180.0f;

    QuestLog->InitializeQuests({ ArrivalQuest });
    QuestLog->InitializeWorldEvents({ RelicSurge });

    FEOGearItem TrainingBlade;
    TrainingBlade.ItemId = TEXT("weapon.dawnwatch_training_blade");
    TrainingBlade.DisplayName = FText::FromString(TEXT("Dawnwatch Training Blade"));
    TrainingBlade.Slot = EEOGearSlot::Weapon;
    TrainingBlade.Rarity = EEOGearRarity::Common;
    TrainingBlade.Power = 4;

    Inventory->AddItem(TrainingBlade);
    Inventory->EquipItem(TrainingBlade.ItemId);

    FEOCompanionProfile Mira;
    Mira.CompanionId = TEXT("companion.mira");
    Mira.DisplayName = FText::FromString(TEXT("Mira of the Dawnwatch"));
    Mira.Role = EEOPartyRole::Support;
    Mira.FollowDistance = 320.0f;
    Mira.AbilityCheckInterval = 1.0f;

    FEOCompanionProfile Tor;
    Tor.CompanionId = TEXT("companion.tor");
    Tor.DisplayName = FText::FromString(TEXT("Tor Valeguard"));
    Tor.Role = EEOPartyRole::Defender;
    Tor.FollowDistance = 380.0f;
    Tor.AbilityCheckInterval = 1.4f;

    CompanionRoster->AddCompanion(Mira);
    CompanionRoster->AddCompanion(Tor);
    CompanionRoster->ActivateCompanion(Mira.CompanionId);
    CompanionRoster->ActivateCompanion(Tor.CompanionId);
}

void AEOHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AEOHeroCharacter::MoveForward);
    PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AEOHeroCharacter::MoveRight);
    PlayerInputComponent->BindAxis(TEXT("CameraYaw"), this, &AEOHeroCharacter::CameraYaw);
    PlayerInputComponent->BindAxis(TEXT("CameraPitch"), this, &AEOHeroCharacter::CameraPitch);

    PlayerInputComponent->BindAction(TEXT("BasicAttack"), IE_Pressed, this, &AEOHeroCharacter::BasicAttack);
    PlayerInputComponent->BindAction(TEXT("TargetCycle"), IE_Pressed, this, &AEOHeroCharacter::TargetCycle);
    PlayerInputComponent->BindAction(TEXT("Dodge"), IE_Pressed, this, &AEOHeroCharacter::Dodge);
    PlayerInputComponent->BindAction(TEXT("Block"), IE_Pressed, this, &AEOHeroCharacter::BlockPressed);
    PlayerInputComponent->BindAction(TEXT("Block"), IE_Released, this, &AEOHeroCharacter::BlockReleased);
    PlayerInputComponent->BindAction(TEXT("Interact"), IE_Pressed, this, &AEOHeroCharacter::Interact);
    PlayerInputComponent->BindAction(TEXT("AbilityOne"), IE_Pressed, this, &AEOHeroCharacter::AbilityOne);
    PlayerInputComponent->BindAction(TEXT("AbilityTwo"), IE_Pressed, this, &AEOHeroCharacter::AbilityTwo);
}

void AEOHeroCharacter::MoveForward(float Value)
{
    if (!FMath::IsNearlyZero(Value))
    {
        const FVector Direction = FRotationMatrix(FRotator(0.0f, DefaultCameraYaw, 0.0f)).GetUnitAxis(EAxis::X);
        AddMovementInput(Direction, Value);
    }
}

void AEOHeroCharacter::MoveRight(float Value)
{
    if (!FMath::IsNearlyZero(Value))
    {
        const FVector Direction = FRotationMatrix(FRotator(0.0f, DefaultCameraYaw, 0.0f)).GetUnitAxis(EAxis::Y);
        AddMovementInput(Direction, Value);
    }
}

void AEOHeroCharacter::CameraYaw(float Value)
{
    if (CameraBoom != nullptr && !FMath::IsNearlyZero(Value))
    {
        FRotator CameraRotation = CameraBoom->GetRelativeRotation();
        CameraRotation.Yaw = FRotator::NormalizeAxis(CameraRotation.Yaw + Value);
        CameraBoom->SetRelativeRotation(CameraRotation);
    }
}

void AEOHeroCharacter::CameraPitch(float Value)
{
    if (CameraBoom != nullptr && !FMath::IsNearlyZero(Value))
    {
        FRotator CameraRotation = CameraBoom->GetRelativeRotation();
        CameraRotation.Pitch = FMath::Clamp(CameraRotation.Pitch + Value, -28.0f, -16.0f);
        CameraBoom->SetRelativeRotation(CameraRotation);
    }
}

void AEOHeroCharacter::BasicAttack()
{
    AEOEnemyCharacter* Target = FindNearestEnemyInRange(DefaultBasicAttackRange);
    if (Target == nullptr)
    {
        UE_LOG(LogTemp, Log, TEXT("Everwild hero basic attack missed: no enemy in range"));
        return;
    }

    const float RawDamage = FEOCombatResolution::CalculateBasicAttackDamage(CombatStats->GetStats(), Inventory->GetTotalEquippedPower());
    const float AppliedDamage = Target->ApplyIncomingHit(RawDamage);
    UE_LOG(LogTemp, Log, TEXT("Everwild hero basic attack hit %s for %.1f"), *Target->GetEnemyId().ToString(), AppliedDamage);
    AwardEnemyIfDefeated(Target);
}

AEOEnemyCharacter* AEOHeroCharacter::FindNearestEnemyInRange(float Range) const
{
    TArray<AActor*> EnemyActors;
    UGameplayStatics::GetAllActorsOfClass(this, AEOEnemyCharacter::StaticClass(), EnemyActors);

    AEOEnemyCharacter* BestEnemy = nullptr;
    float BestDistanceSquared = FMath::Square(FMath::Max(0.0f, Range));

    for (AActor* EnemyActor : EnemyActors)
    {
        AEOEnemyCharacter* Enemy = Cast<AEOEnemyCharacter>(EnemyActor);
        if (Enemy == nullptr || Enemy->GetCombatStatsComponent() == nullptr || !Enemy->GetCombatStatsComponent()->IsAlive())
        {
            continue;
        }

        const float DistanceSquared = FVector::DistSquared(GetActorLocation(), Enemy->GetActorLocation());
        if (DistanceSquared <= BestDistanceSquared)
        {
            BestDistanceSquared = DistanceSquared;
            BestEnemy = Enemy;
        }
    }

    return BestEnemy;
}

void AEOHeroCharacter::ActivateAbilityAndDamageNearestEnemy(int32 AbilityIndex, bool bUltimate)
{
    const EEOAbilityActivationResult Result = AbilityRuntime->TryActivateAbilityByIndex(AbilityIndex, CombatStats);
    if (Result != EEOAbilityActivationResult::Activated)
    {
        UE_LOG(LogTemp, Log, TEXT("Everwild hero ability failed: %s"), *UEnum::GetValueAsString(Result));
        return;
    }

    AEOEnemyCharacter* Target = FindNearestEnemyInRange(DefaultAbilityAttackRange);
    if (Target == nullptr)
    {
        UE_LOG(LogTemp, Log, TEXT("Everwild hero ability activated but found no enemy in range"));
        return;
    }

    const float RawDamage = FEOCombatResolution::CalculateAbilityDamage(CombatStats->GetStats(), Inventory->GetTotalEquippedPower(), bUltimate);
    const float AppliedDamage = Target->ApplyIncomingHit(RawDamage);
    UE_LOG(LogTemp, Log, TEXT("Everwild hero ability hit %s for %.1f"), *Target->GetEnemyId().ToString(), AppliedDamage);
    AwardEnemyIfDefeated(Target);
}

void AEOHeroCharacter::AwardEnemyIfDefeated(AEOEnemyCharacter* Enemy)
{
    if (Enemy == nullptr)
    {
        return;
    }

    const int32 Reward = Enemy->ClaimExperienceReward();
    if (Reward > 0)
    {
        const EEOProgressionResult Result = Progression->AddExperience(Reward);
        RelicShardCount = FMath::Min(RelicShardCount + 1, RequiredRelicShardsForGate);
        if (QuestLog->GetQuestState(TEXT("dawnwatch.starfall_arrival")) == EEOQuestState::Available)
        {
            QuestLog->StartQuest(TEXT("dawnwatch.starfall_arrival"));
        }
        QuestLog->SetWorldEventState(TEXT("starfall.relic_surge"), EEOWorldEventState::Active);
        QuestLog->AdvanceQuest(TEXT("dawnwatch.starfall_arrival"));
        RefreshStarfallQuestProgress();
        UE_LOG(LogTemp, Log, TEXT("Everwild hero gained %d XP: %s"), Reward, *UEnum::GetValueAsString(Result));
    }
}

void AEOHeroCharacter::TargetCycle()
{
    UE_LOG(LogTemp, Verbose, TEXT("Everwild hero target cycle requested"));
}

void AEOHeroCharacter::Dodge()
{
    LaunchCharacter(GetActorForwardVector() * 420.0f, true, false);
}

void AEOHeroCharacter::BlockPressed()
{
    UE_LOG(LogTemp, Log, TEXT("Everwild hero block started"));
}

void AEOHeroCharacter::BlockReleased()
{
    UE_LOG(LogTemp, Log, TEXT("Everwild hero block ended"));
}

void AEOHeroCharacter::Interact()
{
    const EEOQuestActionResult Result = QuestLog->StartQuest(TEXT("dawnwatch.starfall_arrival"));
    if (Result == EEOQuestActionResult::Applied || Result == EEOQuestActionResult::AlreadyActive)
    {
        QuestLog->SetWorldEventState(TEXT("starfall.relic_surge"), EEOWorldEventState::Active);
    }
    RefreshStarfallQuestProgress();
    UE_LOG(LogTemp, Log, TEXT("Everwild hero interact quest result: %s"), *UEnum::GetValueAsString(Result));
}

void AEOHeroCharacter::AbilityOne()
{
    ActivateAbilityAndDamageNearestEnemy(2, false);
}

void AEOHeroCharacter::AbilityTwo()
{
    ActivateAbilityAndDamageNearestEnemy(3, false);
}

void AEOHeroCharacter::RefreshStarfallQuestProgress()
{
    if (RelicShardCount < RequiredRelicShardsForGate || bStarfallGateStabilized)
    {
        return;
    }

    QuestLog->CompleteQuest(TEXT("dawnwatch.starfall_arrival"));
    QuestLog->SetWorldEventState(TEXT("starfall.relic_surge"), EEOWorldEventState::Resolved);
    CombatStats->RestoreHealth(25.0f);
    bStarfallGateStabilized = true;
    UE_LOG(LogTemp, Log, TEXT("Starfall Gate stabilized with %d relic shards"), RelicShardCount);
}
