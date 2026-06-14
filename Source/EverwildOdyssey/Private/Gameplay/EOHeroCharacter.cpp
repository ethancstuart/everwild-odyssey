#include "Gameplay/EOHeroCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Core/EOHeroClassDefinition.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Gameplay/EOAbilityRuntimeComponent.h"
#include "Gameplay/EOCombatResolution.h"
#include "Gameplay/EOCombatStatsComponent.h"
#include "Gameplay/EOEnemyCharacter.h"
#include "Gameplay/EOInventoryComponent.h"
#include "Gameplay/EOQuestLogComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

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

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("IsometricCameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = DefaultCameraBoomLength;
    CameraBoom->SetRelativeRotation(FRotator(DefaultCameraPitch, DefaultCameraYaw, 0.0f));
    CameraBoom->bDoCollisionTest = false;
    CameraBoom->bUsePawnControlRotation = false;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("IsometricCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    HeroSilhouette = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeroSilhouette"));
    HeroSilhouette->SetupAttachment(RootComponent);
    HeroSilhouette->SetRelativeLocation(FVector(0.0f, 0.0f, -46.0f));
    HeroSilhouette->SetRelativeScale3D(FVector(0.85f, 0.85f, 1.45f));
    HeroSilhouette->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderMesh(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
    if (CylinderMesh.Succeeded())
    {
        HeroSilhouette->SetStaticMesh(CylinderMesh.Object);
    }

    CombatStats = CreateDefaultSubobject<UEOCombatStatsComponent>(TEXT("CombatStats"));
    AbilityRuntime = CreateDefaultSubobject<UEOAbilityRuntimeComponent>(TEXT("AbilityRuntime"));
    QuestLog = CreateDefaultSubobject<UEOQuestLogComponent>(TEXT("QuestLog"));
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
}

void AEOHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AEOHeroCharacter::MoveForward);
    PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AEOHeroCharacter::MoveRight);

    PlayerInputComponent->BindAction(TEXT("BasicAttack"), IE_Pressed, this, &AEOHeroCharacter::BasicAttack);
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
