#include "Gameplay/EOHeroCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Core/EOHeroClassDefinition.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Gameplay/EOAbilityRuntimeComponent.h"
#include "Gameplay/EOCombatStatsComponent.h"
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
    UE_LOG(LogTemp, Log, TEXT("Everwild hero basic attack"));
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
    UE_LOG(LogTemp, Log, TEXT("Everwild hero interact"));
}

void AEOHeroCharacter::AbilityOne()
{
    const EEOAbilityActivationResult Result = AbilityRuntime->TryActivateAbilityByIndex(2, CombatStats);
    UE_LOG(LogTemp, Log, TEXT("Everwild hero ability one: %s"), *UEnum::GetValueAsString(Result));
}

void AEOHeroCharacter::AbilityTwo()
{
    const EEOAbilityActivationResult Result = AbilityRuntime->TryActivateAbilityByIndex(3, CombatStats);
    UE_LOG(LogTemp, Log, TEXT("Everwild hero ability two: %s"), *UEnum::GetValueAsString(Result));
}
