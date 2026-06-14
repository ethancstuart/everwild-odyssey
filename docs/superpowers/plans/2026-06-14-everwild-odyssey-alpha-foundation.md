# Everwild Odyssey Alpha Foundation Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Build the Unreal Engine C++ foundation for Everwild Odyssey's first single-player MMO-scale vertical slice.

**Architecture:** Create a C++ first Unreal project with small, focused gameplay systems: class definitions, input, combat attributes, abilities, inventory, quest state, companions, world events, and save data. The first execution pass creates the repository-safe project skeleton and pure C++ gameplay model; subsequent passes connect those systems to Unreal runtime actors, maps, UI, controller input, and playable content after Unreal Engine is installed locally.

**Tech Stack:** Unreal Engine 5.x, C++, Enhanced Input, Unreal Automation Testing, macOS Xcode/clang, Git, optional Git LFS for future large assets.

---

## Current Environment

- Project root: `/Users/ethanstuart/Projects/everwild-odyssey`
- GitHub repo: `https://github.com/ethancstuart/everwild-odyssey`
- Unreal Engine: not found in `/Applications`, `/Users/Shared`, or `/Users/ethanstuart/Applications`.
- Epic Games Launcher: not found locally.
- Xcode: installed, `Xcode 26.3`.
- Apple clang: installed, `Apple clang version 17.0.0`.
- CMake: not installed.
- Git LFS: not installed.

Epic's current Unreal docs state that Unreal Engine includes prerequisite installers when installed through Epic Games Launcher, and their hardware/software page for Unreal Engine 5.7 recommends 32 GB RAM for development-class machines. Use the local installed Unreal version if one appears before execution; otherwise install Unreal Engine through the Epic Games Launcher or source workflow before build verification.

Docs:

- Unreal installation: `https://dev.epicgames.com/documentation/en-us/unreal-engine/installing-unreal-engine`
- Unreal hardware/software specs: `https://dev.epicgames.com/documentation/en-us/unreal-engine/hardware-and-software-specifications-for-unreal-engine`

## Scope Breakdown

The full design spec contains many subsystems. To keep each plan independently testable, this plan covers the alpha foundation:

- Repo hygiene for Unreal.
- Unreal C++ project scaffold.
- Core data model for classes, abilities, inventory, quests, world events, companions, and save state.
- Initial automation tests for the pure gameplay model.
- Setup scripts that detect Unreal and guide installation when absent.
- A documented path from scaffold to playable editor project.

Separate follow-up plans should cover:

- Playable character/controller/camera implementation.
- Combat actors, enemies, and boss.
- Dawnwatch Keep and Starfall Vale blockout.
- Quest content and faction state.
- UI/HUD/controller prompts.
- Companion AI and dungeon flow.
- Playtest polish and packaging.

## File Structure

Create and modify these files:

- `.gitignore`: Unreal-specific generated files and optional local asset exclusions.
- `.gitattributes`: future Git LFS rules for Unreal binary assets.
- `README.md`: setup and project intent.
- `EverwildOdyssey.uproject`: Unreal project descriptor.
- `Config/DefaultEngine.ini`: project metadata and map placeholders.
- `Config/DefaultGame.ini`: game project settings.
- `Config/DefaultInput.ini`: basic input action names and controller-friendly intent.
- `Source/EverwildOdyssey.Target.cs`: game target.
- `Source/EverwildOdysseyEditor.Target.cs`: editor target.
- `Source/EverwildOdyssey/EverwildOdyssey.Build.cs`: module dependencies.
- `Source/EverwildOdyssey/Public/EverwildOdyssey.h`: module umbrella header.
- `Source/EverwildOdyssey/Private/EverwildOdyssey.cpp`: module implementation.
- `Source/EverwildOdyssey/Public/Core/EOClassTypes.h`: class, role, rarity, resource, and faction enums.
- `Source/EverwildOdyssey/Public/Core/EOAbilityDefinition.h`: ability definition struct.
- `Source/EverwildOdyssey/Public/Core/EOHeroClassDefinition.h`: hero class definition struct and factory API.
- `Source/EverwildOdyssey/Private/Core/EOHeroClassDefinition.cpp`: four starting class definitions.
- `Source/EverwildOdyssey/Public/Core/EOCombatStats.h`: combat attribute struct.
- `Source/EverwildOdyssey/Public/Core/EOInventoryTypes.h`: gear and inventory structs.
- `Source/EverwildOdyssey/Public/Core/EOQuestTypes.h`: quest and faction state structs.
- `Source/EverwildOdyssey/Public/Core/EOWorldEventTypes.h`: living-world event structs.
- `Source/EverwildOdyssey/Public/Core/EOCompanionTypes.h`: companion role/behavior structs.
- `Source/EverwildOdyssey/Public/Core/EOSaveTypes.h`: save-profile structs.
- `Source/EverwildOdyssey/Public/Tests/EOTestData.h`: deterministic test helpers.
- `Source/EverwildOdyssey/Private/Tests/EOHeroClassDefinitionTests.cpp`: automation tests for class definitions.
- `Source/EverwildOdyssey/Private/Tests/EOProgressionModelTests.cpp`: automation tests for inventory, quest, event, and save structs.
- `Scripts/check-unreal.sh`: local setup check for Unreal and toolchain.
- `Docs/setup-unreal-macos.md`: human-readable Unreal setup instructions.

## Task 1: Unreal Repo Hygiene

**Files:**

- Modify: `/Users/ethanstuart/Projects/everwild-odyssey/.gitignore`
- Create: `/Users/ethanstuart/Projects/everwild-odyssey/.gitattributes`
- Modify: `/Users/ethanstuart/Projects/everwild-odyssey/README.md`

- [ ] **Step 1: Update `.gitignore` for Unreal-generated files**

Replace `.gitignore` with:

```gitignore
.DS_Store
.env
.env.*
!.env.example

# Local brainstorming/visual companion artifacts.
.superpowers/
.worktrees/

# Node/web tooling if support scripts are added later.
node_modules/
dist/
build/
.next/
coverage/

# Unreal generated files.
Binaries/
Build/
DerivedDataCache/
Intermediate/
Saved/
.vs/
.vscode/
.idea/
*.VC.db
*.opensdf
*.opendb
*.sdf
*.suo
*.xcodeproj
*.xcworkspace

# Local-only Unreal and Marketplace content.
Content/LocalOnly/
Content/Marketplace/
Plugins/Marketplace/

# Packaged builds.
Releases/
PackagedBuilds/
```

- [ ] **Step 2: Add `.gitattributes` for future binary assets**

Create `.gitattributes`:

```gitattributes
*.uasset filter=lfs diff=lfs merge=lfs -text
*.umap filter=lfs diff=lfs merge=lfs -text
*.wav filter=lfs diff=lfs merge=lfs -text
*.mp3 filter=lfs diff=lfs merge=lfs -text
*.fbx filter=lfs diff=lfs merge=lfs -text
*.png filter=lfs diff=lfs merge=lfs -text
*.jpg filter=lfs diff=lfs merge=lfs -text
*.jpeg filter=lfs diff=lfs merge=lfs -text
*.tga filter=lfs diff=lfs merge=lfs -text
*.exr filter=lfs diff=lfs merge=lfs -text
```

- [ ] **Step 3: Expand `README.md`**

Replace `README.md` with:

```markdown
# Everwild Odyssey

Everwild Odyssey is an original heroic high-fantasy Unreal Engine C++ project. The first target is a single-player vertical slice that feels MMO-scale: class choice, controller-friendly combat, companions, quests, gear, a living hub, an explorable region, and a dungeon boss.

## Current Direction

- Engine: Unreal Engine 5.x.
- Language: C++ first.
- Target: PC/computer.
- Input: keyboard/mouse and plugged-in controller.
- First region: Dawnwatch Keep and Starfall Vale.
- First mode: single-player alpha foundation with MMO-style systems.

## Setup

1. Install Unreal Engine through Epic Games Launcher or a source build.
2. Run `Scripts/check-unreal.sh` from the project root.
3. Open `EverwildOdyssey.uproject` with the installed Unreal version.
4. Generate project files if Unreal prompts for them.
5. Build the editor target.

Detailed macOS setup notes live in `Docs/setup-unreal-macos.md`.

## Design Docs

- `docs/superpowers/specs/2026-06-14-everwild-odyssey-design.md`
- `docs/superpowers/plans/2026-06-14-everwild-odyssey-alpha-foundation.md`

## Asset Policy

The public repo should contain repo-safe original placeholders and source code only. Local-only premium or Marketplace assets belong in ignored paths such as `Content/LocalOnly/`, `Content/Marketplace/`, or `Plugins/Marketplace/`.
```

- [ ] **Step 4: Verify files**

Run:

```bash
git diff -- .gitignore .gitattributes README.md
```

Expected: the diff shows Unreal ignore rules, LFS patterns, and setup instructions.

- [ ] **Step 5: Commit repo hygiene**

Run:

```bash
git add .gitignore .gitattributes README.md
git commit -m "chore: prepare repo for Unreal project"
```

Expected: commit succeeds.

## Task 2: Unreal Project Descriptor And Config

**Files:**

- Create: `/Users/ethanstuart/Projects/everwild-odyssey/EverwildOdyssey.uproject`
- Create: `/Users/ethanstuart/Projects/everwild-odyssey/Config/DefaultEngine.ini`
- Create: `/Users/ethanstuart/Projects/everwild-odyssey/Config/DefaultGame.ini`
- Create: `/Users/ethanstuart/Projects/everwild-odyssey/Config/DefaultInput.ini`

- [ ] **Step 1: Create Unreal project descriptor**

Create `EverwildOdyssey.uproject`:

```json
{
  "FileVersion": 3,
  "EngineAssociation": "",
  "Category": "Games",
  "Description": "A heroic high-fantasy C++ action RPG foundation for a single-player MMO-scale vertical slice.",
  "Modules": [
    {
      "Name": "EverwildOdyssey",
      "Type": "Runtime",
      "LoadingPhase": "Default"
    }
  ],
  "Plugins": [
    {
      "Name": "EnhancedInput",
      "Enabled": true
    },
    {
      "Name": "CommonUI",
      "Enabled": true
    }
  ]
}
```

- [ ] **Step 2: Create `Config/DefaultEngine.ini`**

Create `Config/DefaultEngine.ini`:

```ini
[/Script/EngineSettings.GameMapsSettings]
EditorStartupMap=/Game/Everwild/Maps/M_DawnwatchPrototype.M_DawnwatchPrototype
GameDefaultMap=/Game/Everwild/Maps/M_DawnwatchPrototype.M_DawnwatchPrototype
GlobalDefaultGameMode=/Script/EverwildOdyssey.EOGameMode

[/Script/Engine.RendererSettings]
r.DynamicGlobalIlluminationMethod=1
r.ReflectionMethod=1
r.DefaultFeature.AutoExposure=False
r.DefaultFeature.MotionBlur=False

[/Script/Engine.Engine]
NearClipPlane=3.000000
```

- [ ] **Step 3: Create `Config/DefaultGame.ini`**

Create `Config/DefaultGame.ini`:

```ini
[/Script/EngineSettings.GeneralProjectSettings]
ProjectID=7BB0ABAF4F3E4E7FA82D4F07F5E2F001
ProjectName=Everwild Odyssey
CompanyName=Ethan Stuart
ProjectDisplayedTitle=NSLOCTEXT("[/Script/EngineSettings]", "EverwildOdysseyTitle", "Everwild Odyssey")
ProjectDebugTitleInfo=NSLOCTEXT("[/Script/EngineSettings]", "EverwildOdysseyDebugTitle", "Everwild Odyssey Alpha")
Description=Single-player heroic fantasy vertical slice with MMO-scale systems.
CopyrightNotice=
```

- [ ] **Step 4: Create `Config/DefaultInput.ini`**

Create `Config/DefaultInput.ini`:

```ini
[/Script/Engine.InputSettings]
bEnableMouseSmoothing=False
bViewAccelerationEnabled=False

+ActionMappings=(ActionName="Interact",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=E)
+ActionMappings=(ActionName="Interact",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=Gamepad_FaceButton_Bottom)
+ActionMappings=(ActionName="BasicAttack",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=LeftMouseButton)
+ActionMappings=(ActionName="BasicAttack",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=Gamepad_FaceButton_Left)
+ActionMappings=(ActionName="Dodge",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=SpaceBar)
+ActionMappings=(ActionName="Dodge",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=Gamepad_FaceButton_Right)
+ActionMappings=(ActionName="Block",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=RightMouseButton)
+ActionMappings=(ActionName="Block",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=Gamepad_LeftTrigger)
+ActionMappings=(ActionName="AbilityOne",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=One)
+ActionMappings=(ActionName="AbilityOne",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=Gamepad_RightShoulder)
+ActionMappings=(ActionName="AbilityTwo",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=Two)
+ActionMappings=(ActionName="AbilityTwo",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=Gamepad_RightTrigger)

+AxisMappings=(AxisName="MoveForward",Scale=1.000000,Key=W)
+AxisMappings=(AxisName="MoveForward",Scale=-1.000000,Key=S)
+AxisMappings=(AxisName="MoveForward",Scale=1.000000,Key=Gamepad_LeftY)
+AxisMappings=(AxisName="MoveRight",Scale=1.000000,Key=D)
+AxisMappings=(AxisName="MoveRight",Scale=-1.000000,Key=A)
+AxisMappings=(AxisName="MoveRight",Scale=1.000000,Key=Gamepad_LeftX)
+AxisMappings=(AxisName="CameraYaw",Scale=1.000000,Key=MouseX)
+AxisMappings=(AxisName="CameraYaw",Scale=1.000000,Key=Gamepad_RightX)
+AxisMappings=(AxisName="CameraPitch",Scale=1.000000,Key=MouseY)
+AxisMappings=(AxisName="CameraPitch",Scale=1.000000,Key=Gamepad_RightY)
```

- [ ] **Step 5: Verify JSON and config files exist**

Run:

```bash
test -f EverwildOdyssey.uproject
test -f Config/DefaultEngine.ini
test -f Config/DefaultGame.ini
test -f Config/DefaultInput.ini
```

Expected: all commands exit successfully.

- [ ] **Step 6: Commit project descriptor**

Run:

```bash
git add EverwildOdyssey.uproject Config
git commit -m "feat: add Unreal project descriptor"
```

Expected: commit succeeds.

## Task 3: C++ Module Skeleton

**Files:**

- Create: `/Users/ethanstuart/Projects/everwild-odyssey/Source/EverwildOdyssey.Target.cs`
- Create: `/Users/ethanstuart/Projects/everwild-odyssey/Source/EverwildOdysseyEditor.Target.cs`
- Create: `/Users/ethanstuart/Projects/everwild-odyssey/Source/EverwildOdyssey/EverwildOdyssey.Build.cs`
- Create: `/Users/ethanstuart/Projects/everwild-odyssey/Source/EverwildOdyssey/Public/EverwildOdyssey.h`
- Create: `/Users/ethanstuart/Projects/everwild-odyssey/Source/EverwildOdyssey/Private/EverwildOdyssey.cpp`

- [ ] **Step 1: Create game target**

Create `Source/EverwildOdyssey.Target.cs`:

```csharp
using UnrealBuildTool;
using System.Collections.Generic;

public class EverwildOdysseyTarget : TargetRules
{
    public EverwildOdysseyTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        ExtraModuleNames.Add("EverwildOdyssey");
    }
}
```

- [ ] **Step 2: Create editor target**

Create `Source/EverwildOdysseyEditor.Target.cs`:

```csharp
using UnrealBuildTool;
using System.Collections.Generic;

public class EverwildOdysseyEditorTarget : TargetRules
{
    public EverwildOdysseyEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        ExtraModuleNames.Add("EverwildOdyssey");
    }
}
```

- [ ] **Step 3: Create module build rules**

Create `Source/EverwildOdyssey/EverwildOdyssey.Build.cs`:

```csharp
using UnrealBuildTool;

public class EverwildOdyssey : ModuleRules
{
    public EverwildOdyssey(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "EnhancedInput",
            "UMG",
            "CommonUI",
            "AIModule",
            "GameplayTasks",
            "NavigationSystem"
        });
    }
}
```

- [ ] **Step 4: Create module header**

Create `Source/EverwildOdyssey/Public/EverwildOdyssey.h`:

```cpp
#pragma once

#include "CoreMinimal.h"
```

- [ ] **Step 5: Create module implementation**

Create `Source/EverwildOdyssey/Private/EverwildOdyssey.cpp`:

```cpp
#include "EverwildOdyssey.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE(FDefaultGameModuleImpl, EverwildOdyssey, "EverwildOdyssey");
```

- [ ] **Step 6: Verify source files**

Run:

```bash
find Source -type f | sort
```

Expected output includes the five files from this task.

- [ ] **Step 7: Commit module skeleton**

Run:

```bash
git add Source
git commit -m "feat: add Unreal C++ module skeleton"
```

Expected: commit succeeds.

## Task 4: Core Type Definitions

**Files:**

- Create: `/Users/ethanstuart/Projects/everwild-odyssey/Source/EverwildOdyssey/Public/Core/EOClassTypes.h`
- Create: `/Users/ethanstuart/Projects/everwild-odyssey/Source/EverwildOdyssey/Public/Core/EOCombatStats.h`

- [ ] **Step 1: Create class/faction enums**

Create `Source/EverwildOdyssey/Public/Core/EOClassTypes.h`:

```cpp
#pragma once

#include "CoreMinimal.h"
#include "EOClassTypes.generated.h"

UENUM(BlueprintType)
enum class EEOHeroClass : uint8
{
    Runeblade UMETA(DisplayName = "Runeblade"),
    Starshaper UMETA(DisplayName = "Starshaper"),
    Pathwarden UMETA(DisplayName = "Pathwarden"),
    LightboundVanguard UMETA(DisplayName = "Lightbound Vanguard")
};

UENUM(BlueprintType)
enum class EEOPartyRole : uint8
{
    Striker UMETA(DisplayName = "Striker"),
    Defender UMETA(DisplayName = "Defender"),
    Support UMETA(DisplayName = "Support"),
    Controller UMETA(DisplayName = "Controller")
};

UENUM(BlueprintType)
enum class EEOResourceType : uint8
{
    Stamina UMETA(DisplayName = "Stamina"),
    Mana UMETA(DisplayName = "Mana"),
    Focus UMETA(DisplayName = "Focus"),
    Valor UMETA(DisplayName = "Valor")
};

UENUM(BlueprintType)
enum class EEOGearRarity : uint8
{
    Common UMETA(DisplayName = "Common"),
    Uncommon UMETA(DisplayName = "Uncommon"),
    Rare UMETA(DisplayName = "Rare"),
    Relic UMETA(DisplayName = "Relic")
};

UENUM(BlueprintType)
enum class EEOFaction : uint8
{
    DawnwatchCommand UMETA(DisplayName = "Dawnwatch Command"),
    StarfallCollegium UMETA(DisplayName = "Starfall Collegium"),
    ValeVillagers UMETA(DisplayName = "Vale Villagers")
};
```

- [ ] **Step 2: Create combat stat struct**

Create `Source/EverwildOdyssey/Public/Core/EOCombatStats.h`:

```cpp
#pragma once

#include "CoreMinimal.h"
#include "EOCombatStats.generated.h"

USTRUCT(BlueprintType)
struct FEOCombatStats
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float MaxHealth = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float MaxResource = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float AttackPower = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float SpellPower = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float Armor = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float MoveSpeed = 600.0f;

    bool IsValidForAlpha() const
    {
        return MaxHealth > 0.0f
            && MaxResource > 0.0f
            && AttackPower >= 0.0f
            && SpellPower >= 0.0f
            && Armor >= 0.0f
            && MoveSpeed > 0.0f;
    }
};
```

- [ ] **Step 3: Verify no placeholder markers**

Run:

```bash
rg -n "T[B]D|T[O]DO|implement[ ]later|fill[ ]in" Source/EverwildOdyssey/Public/Core
```

Expected: no matches.

- [ ] **Step 4: Commit core types**

Run:

```bash
git add Source/EverwildOdyssey/Public/Core/EOClassTypes.h Source/EverwildOdyssey/Public/Core/EOCombatStats.h
git commit -m "feat: add core class and stat types"
```

Expected: commit succeeds.

## Task 5: Hero Class Definitions

**Files:**

- Create: `/Users/ethanstuart/Projects/everwild-odyssey/Source/EverwildOdyssey/Public/Core/EOAbilityDefinition.h`
- Create: `/Users/ethanstuart/Projects/everwild-odyssey/Source/EverwildOdyssey/Public/Core/EOHeroClassDefinition.h`
- Create: `/Users/ethanstuart/Projects/everwild-odyssey/Source/EverwildOdyssey/Private/Core/EOHeroClassDefinition.cpp`

- [ ] **Step 1: Create ability definition struct**

Create `Source/EverwildOdyssey/Public/Core/EOAbilityDefinition.h`:

```cpp
#pragma once

#include "CoreMinimal.h"
#include "EOAbilityDefinition.generated.h"

USTRUCT(BlueprintType)
struct FEOAbilityDefinition
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
    FName AbilityId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
    float CooldownSeconds = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
    float ResourceCost = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
    bool bIsUltimate = false;

    bool IsValidForAlpha() const
    {
        return !AbilityId.IsNone()
            && !DisplayName.IsEmpty()
            && CooldownSeconds >= 0.0f
            && ResourceCost >= 0.0f;
    }
};
```

- [ ] **Step 2: Create hero class definition API**

Create `Source/EverwildOdyssey/Public/Core/EOHeroClassDefinition.h`:

```cpp
#pragma once

#include "CoreMinimal.h"
#include "Core/EOAbilityDefinition.h"
#include "Core/EOClassTypes.h"
#include "Core/EOCombatStats.h"
#include "EOHeroClassDefinition.generated.h"

USTRUCT(BlueprintType)
struct FEOHeroClassDefinition
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class")
    EEOHeroClass HeroClass = EEOHeroClass::Runeblade;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class")
    FText FantasySummary;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class")
    EEOPartyRole PrimaryRole = EEOPartyRole::Striker;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class")
    EEOResourceType ResourceType = EEOResourceType::Stamina;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class")
    FEOCombatStats BaseStats;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class")
    TArray<FEOAbilityDefinition> StartingAbilities;

    bool IsValidForAlpha() const
    {
        if (DisplayName.IsEmpty() || FantasySummary.IsEmpty() || !BaseStats.IsValidForAlpha())
        {
            return false;
        }

        int32 UltimateCount = 0;
        for (const FEOAbilityDefinition& Ability : StartingAbilities)
        {
            if (!Ability.IsValidForAlpha())
            {
                return false;
            }
            UltimateCount += Ability.bIsUltimate ? 1 : 0;
        }

        return StartingAbilities.Num() == 5 && UltimateCount == 1;
    }
};

class FEOWildClassCatalog
{
public:
    static TArray<FEOHeroClassDefinition> BuildStartingClasses();
    static bool TryGetClassDefinition(EEOHeroClass HeroClass, FEOHeroClassDefinition& OutDefinition);
};
```

- [ ] **Step 3: Implement four class definitions**

Create `Source/EverwildOdyssey/Private/Core/EOHeroClassDefinition.cpp`:

```cpp
#include "Core/EOHeroClassDefinition.h"

namespace
{
FEOAbilityDefinition Ability(FName Id, const TCHAR* Name, const TCHAR* Description, float Cooldown, float Cost, bool bUltimate = false)
{
    FEOAbilityDefinition Result;
    Result.AbilityId = Id;
    Result.DisplayName = FText::FromString(Name);
    Result.Description = FText::FromString(Description);
    Result.CooldownSeconds = Cooldown;
    Result.ResourceCost = Cost;
    Result.bIsUltimate = bUltimate;
    return Result;
}
}

TArray<FEOHeroClassDefinition> FEOWildClassCatalog::BuildStartingClasses()
{
    TArray<FEOHeroClassDefinition> Classes;
    Classes.Reserve(4);

    FEOHeroClassDefinition Runeblade;
    Runeblade.HeroClass = EEOHeroClass::Runeblade;
    Runeblade.DisplayName = FText::FromString(TEXT("Runeblade"));
    Runeblade.FantasySummary = FText::FromString(TEXT("A durable melee striker who carves runes into the battlefield."));
    Runeblade.PrimaryRole = EEOPartyRole::Striker;
    Runeblade.ResourceType = EEOResourceType::Stamina;
    Runeblade.BaseStats = { 135.0f, 100.0f, 16.0f, 8.0f, 8.0f, 610.0f };
    Runeblade.StartingAbilities = {
        Ability(TEXT("runeblade.basic_combo"), TEXT("Iron Verse"), TEXT("A three-hit sword chain that builds rune charge."), 0.0f, 0.0f),
        Ability(TEXT("runeblade.guard"), TEXT("Rune Guard"), TEXT("Raise a warding blade to reduce frontal damage."), 4.0f, 15.0f),
        Ability(TEXT("runeblade.cleave"), TEXT("Rune Cleave"), TEXT("Release a crescent slash that damages enemies in an arc."), 6.0f, 25.0f),
        Ability(TEXT("runeblade.dash"), TEXT("Carve Step"), TEXT("Dash through danger and strike the nearest marked foe."), 8.0f, 20.0f),
        Ability(TEXT("runeblade.ultimate"), TEXT("Relic Surge"), TEXT("Overload the blade with starfire for a devastating finisher."), 45.0f, 100.0f, true)
    };
    Classes.Add(Runeblade);

    FEOHeroClassDefinition Starshaper;
    Starshaper.HeroClass = EEOHeroClass::Starshaper;
    Starshaper.DisplayName = FText::FromString(TEXT("Starshaper"));
    Starshaper.FantasySummary = FText::FromString(TEXT("A ranged spellcaster who bends fallen starlight into shields and lances."));
    Starshaper.PrimaryRole = EEOPartyRole::Support;
    Starshaper.ResourceType = EEOResourceType::Mana;
    Starshaper.BaseStats = { 95.0f, 130.0f, 6.0f, 19.0f, 3.0f, 590.0f };
    Starshaper.StartingAbilities = {
        Ability(TEXT("starshaper.bolt"), TEXT("Star Bolt"), TEXT("Fire a clean bolt of celestial energy."), 0.0f, 0.0f),
        Ability(TEXT("starshaper.field"), TEXT("Arcane Field"), TEXT("Create a zone that slows foes and empowers allies."), 9.0f, 30.0f),
        Ability(TEXT("starshaper.barrier"), TEXT("Barrier Pulse"), TEXT("Shield nearby party members from the next burst of damage."), 12.0f, 35.0f),
        Ability(TEXT("starshaper.lance"), TEXT("Celestial Lance"), TEXT("Pierce a line of enemies with concentrated starlight."), 7.0f, 28.0f),
        Ability(TEXT("starshaper.ultimate"), TEXT("Astral Nova"), TEXT("Collapse gathered starfire into a brilliant area detonation."), 50.0f, 100.0f, true)
    };
    Classes.Add(Starshaper);

    FEOHeroClassDefinition Pathwarden;
    Pathwarden.HeroClass = EEOHeroClass::Pathwarden;
    Pathwarden.DisplayName = FText::FromString(TEXT("Pathwarden"));
    Pathwarden.FantasySummary = FText::FromString(TEXT("A mobile explorer who controls the field with marks, traps, and precision."));
    Pathwarden.PrimaryRole = EEOPartyRole::Controller;
    Pathwarden.ResourceType = EEOResourceType::Focus;
    Pathwarden.BaseStats = { 110.0f, 110.0f, 14.0f, 9.0f, 5.0f, 640.0f };
    Pathwarden.StartingAbilities = {
        Ability(TEXT("pathwarden.shot"), TEXT("Warden Shot"), TEXT("Loose a quick shot or close spear jab based on range."), 0.0f, 0.0f),
        Ability(TEXT("pathwarden.roll_shot"), TEXT("Roll Shot"), TEXT("Evade and fire at the nearest threat while moving."), 5.0f, 18.0f),
        Ability(TEXT("pathwarden.snare"), TEXT("Snare Trap"), TEXT("Place a trap that roots the first enemy to cross it."), 10.0f, 25.0f),
        Ability(TEXT("pathwarden.mark"), TEXT("Hunter's Mark"), TEXT("Mark a priority target for increased party damage."), 8.0f, 20.0f),
        Ability(TEXT("pathwarden.ultimate"), TEXT("Wild Hunt"), TEXT("Call a spectral volley against marked enemies."), 45.0f, 100.0f, true)
    };
    Classes.Add(Pathwarden);

    FEOHeroClassDefinition Vanguard;
    Vanguard.HeroClass = EEOHeroClass::LightboundVanguard;
    Vanguard.DisplayName = FText::FromString(TEXT("Lightbound Vanguard"));
    Vanguard.FantasySummary = FText::FromString(TEXT("A radiant defender who anchors the party with shields, heals, and challenge."));
    Vanguard.PrimaryRole = EEOPartyRole::Defender;
    Vanguard.ResourceType = EEOResourceType::Valor;
    Vanguard.BaseStats = { 155.0f, 100.0f, 12.0f, 12.0f, 12.0f, 575.0f };
    Vanguard.StartingAbilities = {
        Ability(TEXT("vanguard.strike"), TEXT("Shield Strike"), TEXT("Bash a foe and generate valor."), 0.0f, 0.0f),
        Ability(TEXT("vanguard.block"), TEXT("Dawnwall"), TEXT("Brace behind a radiant shield and reduce incoming damage."), 5.0f, 15.0f),
        Ability(TEXT("vanguard.heal"), TEXT("Radiant Mend"), TEXT("Restore health to yourself or the weakest companion."), 12.0f, 35.0f),
        Ability(TEXT("vanguard.challenge"), TEXT("Sunlit Challenge"), TEXT("Force nearby enemies to focus on you briefly."), 10.0f, 25.0f),
        Ability(TEXT("vanguard.ultimate"), TEXT("Dawn Banner"), TEXT("Plant a banner that shields allies and burns enemies."), 55.0f, 100.0f, true)
    };
    Classes.Add(Vanguard);

    return Classes;
}

bool FEOWildClassCatalog::TryGetClassDefinition(EEOHeroClass HeroClass, FEOHeroClassDefinition& OutDefinition)
{
    const TArray<FEOHeroClassDefinition> Classes = BuildStartingClasses();
    for (const FEOHeroClassDefinition& Definition : Classes)
    {
        if (Definition.HeroClass == HeroClass)
        {
            OutDefinition = Definition;
            return true;
        }
    }

    return false;
}
```

- [ ] **Step 4: Verify definitions have four classes**

Run:

```bash
rg -n "DisplayName = FText::FromString" Source/EverwildOdyssey/Private/Core/EOHeroClassDefinition.cpp
```

Expected: output includes Runeblade, Starshaper, Pathwarden, and Lightbound Vanguard class names.

- [ ] **Step 5: Commit class catalog**

Run:

```bash
git add Source/EverwildOdyssey/Public/Core/EOAbilityDefinition.h Source/EverwildOdyssey/Public/Core/EOHeroClassDefinition.h Source/EverwildOdyssey/Private/Core/EOHeroClassDefinition.cpp
git commit -m "feat: define four starting hero classes"
```

Expected: commit succeeds.

## Task 6: Progression, Quest, Companion, Event, And Save Data

**Files:**

- Create: `/Users/ethanstuart/Projects/everwild-odyssey/Source/EverwildOdyssey/Public/Core/EOInventoryTypes.h`
- Create: `/Users/ethanstuart/Projects/everwild-odyssey/Source/EverwildOdyssey/Public/Core/EOQuestTypes.h`
- Create: `/Users/ethanstuart/Projects/everwild-odyssey/Source/EverwildOdyssey/Public/Core/EOWorldEventTypes.h`
- Create: `/Users/ethanstuart/Projects/everwild-odyssey/Source/EverwildOdyssey/Public/Core/EOCompanionTypes.h`
- Create: `/Users/ethanstuart/Projects/everwild-odyssey/Source/EverwildOdyssey/Public/Core/EOSaveTypes.h`

- [ ] **Step 1: Create inventory and gear structs**

Create `Source/EverwildOdyssey/Public/Core/EOInventoryTypes.h`:

```cpp
#pragma once

#include "CoreMinimal.h"
#include "Core/EOClassTypes.h"
#include "EOInventoryTypes.generated.h"

UENUM(BlueprintType)
enum class EEOGearSlot : uint8
{
    Weapon UMETA(DisplayName = "Weapon"),
    Armor UMETA(DisplayName = "Armor"),
    Trinket UMETA(DisplayName = "Trinket")
};

USTRUCT(BlueprintType)
struct FEOGearItem
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gear")
    FName ItemId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gear")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gear")
    EEOGearSlot Slot = EEOGearSlot::Weapon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gear")
    EEOGearRarity Rarity = EEOGearRarity::Common;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gear")
    int32 Power = 1;

    bool IsValidForAlpha() const
    {
        return !ItemId.IsNone() && !DisplayName.IsEmpty() && Power > 0;
    }
};
```

- [ ] **Step 2: Create quest/faction structs**

Create `Source/EverwildOdyssey/Public/Core/EOQuestTypes.h`:

```cpp
#pragma once

#include "CoreMinimal.h"
#include "Core/EOClassTypes.h"
#include "EOQuestTypes.generated.h"

UENUM(BlueprintType)
enum class EEOQuestState : uint8
{
    Locked UMETA(DisplayName = "Locked"),
    Available UMETA(DisplayName = "Available"),
    Active UMETA(DisplayName = "Active"),
    Completed UMETA(DisplayName = "Completed"),
    Rewarded UMETA(DisplayName = "Rewarded")
};

USTRUCT(BlueprintType)
struct FEOQuestRecord
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    FName QuestId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    EEOQuestState State = EEOQuestState::Locked;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    int32 StepIndex = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    EEOFaction ReportedToFaction = EEOFaction::DawnwatchCommand;

    bool CanAdvance() const
    {
        return State == EEOQuestState::Active && StepIndex >= 0;
    }
};
```

- [ ] **Step 3: Create world event structs**

Create `Source/EverwildOdyssey/Public/Core/EOWorldEventTypes.h`:

```cpp
#pragma once

#include "CoreMinimal.h"
#include "EOWorldEventTypes.generated.h"

UENUM(BlueprintType)
enum class EEOWorldEventState : uint8
{
    Dormant UMETA(DisplayName = "Dormant"),
    Warning UMETA(DisplayName = "Warning"),
    Active UMETA(DisplayName = "Active"),
    Resolved UMETA(DisplayName = "Resolved")
};

USTRUCT(BlueprintType)
struct FEOWorldEventRecord
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World Event")
    FName EventId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World Event")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World Event")
    EEOWorldEventState State = EEOWorldEventState::Dormant;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World Event")
    float RespawnSeconds = 180.0f;

    bool CanRespawn() const
    {
        return RespawnSeconds > 0.0f && State == EEOWorldEventState::Resolved;
    }
};
```

- [ ] **Step 4: Create companion structs**

Create `Source/EverwildOdyssey/Public/Core/EOCompanionTypes.h`:

```cpp
#pragma once

#include "CoreMinimal.h"
#include "Core/EOClassTypes.h"
#include "EOCompanionTypes.generated.h"

USTRUCT(BlueprintType)
struct FEOCompanionProfile
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Companion")
    FName CompanionId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Companion")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Companion")
    EEOPartyRole Role = EEOPartyRole::Support;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Companion")
    float FollowDistance = 300.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Companion")
    float AbilityCheckInterval = 1.25f;

    bool IsValidForAlpha() const
    {
        return !CompanionId.IsNone() && !DisplayName.IsEmpty() && FollowDistance > 0.0f && AbilityCheckInterval > 0.0f;
    }
};
```

- [ ] **Step 5: Create save structs**

Create `Source/EverwildOdyssey/Public/Core/EOSaveTypes.h`:

```cpp
#pragma once

#include "CoreMinimal.h"
#include "Core/EOClassTypes.h"
#include "Core/EOInventoryTypes.h"
#include "Core/EOQuestTypes.h"
#include "Core/EOWorldEventTypes.h"
#include "EOSaveTypes.generated.h"

USTRUCT(BlueprintType)
struct FEOSaveProfile
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save")
    FString SlotName = TEXT("AlphaSlot");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save")
    EEOHeroClass SelectedClass = EEOHeroClass::Runeblade;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save")
    int32 HeroLevel = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save")
    int32 Experience = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save")
    TArray<FEOGearItem> Inventory;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save")
    TArray<FEOQuestRecord> Quests;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save")
    TArray<FEOWorldEventRecord> WorldEvents;

    bool IsValidForAlpha() const
    {
        return !SlotName.IsEmpty() && HeroLevel > 0 && Experience >= 0;
    }
};
```

- [ ] **Step 6: Verify core headers**

Run:

```bash
find Source/EverwildOdyssey/Public/Core -type f | sort
```

Expected: output includes all core headers from Tasks 4, 5, and 6.

- [ ] **Step 7: Commit gameplay data structs**

Run:

```bash
git add Source/EverwildOdyssey/Public/Core
git commit -m "feat: add progression and world state types"
```

Expected: commit succeeds.

## Task 7: Automation Tests For Gameplay Model

**Files:**

- Create: `/Users/ethanstuart/Projects/everwild-odyssey/Source/EverwildOdyssey/Public/Tests/EOTestData.h`
- Create: `/Users/ethanstuart/Projects/everwild-odyssey/Source/EverwildOdyssey/Private/Tests/EOHeroClassDefinitionTests.cpp`
- Create: `/Users/ethanstuart/Projects/everwild-odyssey/Source/EverwildOdyssey/Private/Tests/EOProgressionModelTests.cpp`

- [ ] **Step 1: Create test helper data**

Create `Source/EverwildOdyssey/Public/Tests/EOTestData.h`:

```cpp
#pragma once

#include "CoreMinimal.h"
#include "Core/EOInventoryTypes.h"
#include "Core/EOQuestTypes.h"
#include "Core/EOWorldEventTypes.h"

namespace EOTestData
{
    inline FEOGearItem MakeRelicWeapon()
    {
        FEOGearItem Item;
        Item.ItemId = TEXT("relic.starforged_blade");
        Item.DisplayName = FText::FromString(TEXT("Starforged Blade"));
        Item.Slot = EEOGearSlot::Weapon;
        Item.Rarity = EEOGearRarity::Relic;
        Item.Power = 12;
        return Item;
    }

    inline FEOQuestRecord MakeActiveMainQuest()
    {
        FEOQuestRecord Quest;
        Quest.QuestId = TEXT("main.starfall_disturbance");
        Quest.DisplayName = FText::FromString(TEXT("The Starfall Disturbance"));
        Quest.State = EEOQuestState::Active;
        Quest.StepIndex = 2;
        Quest.ReportedToFaction = EEOFaction::StarfallCollegium;
        return Quest;
    }

    inline FEOWorldEventRecord MakeResolvedRelicSurge()
    {
        FEOWorldEventRecord Event;
        Event.EventId = TEXT("event.relic_surge");
        Event.DisplayName = FText::FromString(TEXT("Relic Surge"));
        Event.State = EEOWorldEventState::Resolved;
        Event.RespawnSeconds = 180.0f;
        return Event;
    }
}
```

- [ ] **Step 2: Create class catalog tests**

Create `Source/EverwildOdyssey/Private/Tests/EOHeroClassDefinitionTests.cpp`:

```cpp
#if WITH_DEV_AUTOMATION_TESTS

#include "Core/EOHeroClassDefinition.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEOHeroClassCatalogTest, "EverwildOdyssey.Core.HeroClassCatalog", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FEOHeroClassCatalogTest::RunTest(const FString& Parameters)
{
    const TArray<FEOHeroClassDefinition> Classes = FEOWildClassCatalog::BuildStartingClasses();
    TestEqual(TEXT("The alpha exposes four starting classes."), Classes.Num(), 4);

    TSet<EEOHeroClass> UniqueClasses;
    for (const FEOHeroClassDefinition& Definition : Classes)
    {
        TestTrue(TEXT("Class definition is alpha-valid."), Definition.IsValidForAlpha());
        UniqueClasses.Add(Definition.HeroClass);
    }

    TestTrue(TEXT("Runeblade exists."), UniqueClasses.Contains(EEOHeroClass::Runeblade));
    TestTrue(TEXT("Starshaper exists."), UniqueClasses.Contains(EEOHeroClass::Starshaper));
    TestTrue(TEXT("Pathwarden exists."), UniqueClasses.Contains(EEOHeroClass::Pathwarden));
    TestTrue(TEXT("Lightbound Vanguard exists."), UniqueClasses.Contains(EEOHeroClass::LightboundVanguard));

    FEOHeroClassDefinition Vanguard;
    TestTrue(TEXT("Can look up Lightbound Vanguard."), FEOWildClassCatalog::TryGetClassDefinition(EEOHeroClass::LightboundVanguard, Vanguard));
    TestEqual(TEXT("Vanguard role is Defender."), static_cast<uint8>(Vanguard.PrimaryRole), static_cast<uint8>(EEOPartyRole::Defender));

    return true;
}

#endif
```

- [ ] **Step 3: Create progression model tests**

Create `Source/EverwildOdyssey/Private/Tests/EOProgressionModelTests.cpp`:

```cpp
#if WITH_DEV_AUTOMATION_TESTS

#include "Core/EOSaveTypes.h"
#include "Tests/EOTestData.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEOProgressionModelTest, "EverwildOdyssey.Core.ProgressionModel", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FEOProgressionModelTest::RunTest(const FString& Parameters)
{
    const FEOGearItem RelicWeapon = EOTestData::MakeRelicWeapon();
    TestTrue(TEXT("Relic weapon is valid."), RelicWeapon.IsValidForAlpha());
    TestEqual(TEXT("Relic weapon rarity is Relic."), static_cast<uint8>(RelicWeapon.Rarity), static_cast<uint8>(EEOGearRarity::Relic));

    const FEOQuestRecord MainQuest = EOTestData::MakeActiveMainQuest();
    TestTrue(TEXT("Active main quest can advance."), MainQuest.CanAdvance());
    TestEqual(TEXT("Quest records faction choice."), static_cast<uint8>(MainQuest.ReportedToFaction), static_cast<uint8>(EEOFaction::StarfallCollegium));

    const FEOWorldEventRecord Event = EOTestData::MakeResolvedRelicSurge();
    TestTrue(TEXT("Resolved event can respawn."), Event.CanRespawn());

    FEOSaveProfile Save;
    Save.SelectedClass = EEOHeroClass::Pathwarden;
    Save.HeroLevel = 2;
    Save.Experience = 125;
    Save.Inventory.Add(RelicWeapon);
    Save.Quests.Add(MainQuest);
    Save.WorldEvents.Add(Event);

    TestTrue(TEXT("Save profile is alpha-valid."), Save.IsValidForAlpha());
    TestEqual(TEXT("Save tracks inventory."), Save.Inventory.Num(), 1);
    TestEqual(TEXT("Save tracks quests."), Save.Quests.Num(), 1);
    TestEqual(TEXT("Save tracks world events."), Save.WorldEvents.Num(), 1);

    return true;
}

#endif
```

- [ ] **Step 4: Verify test file names**

Run:

```bash
find Source/EverwildOdyssey/Private/Tests Source/EverwildOdyssey/Public/Tests -type f | sort
```

Expected: output includes `EOHeroClassDefinitionTests.cpp`, `EOProgressionModelTests.cpp`, and `EOTestData.h`.

- [ ] **Step 5: Commit tests**

Run:

```bash
git add Source/EverwildOdyssey/Public/Tests Source/EverwildOdyssey/Private/Tests
git commit -m "test: cover alpha gameplay data model"
```

Expected: commit succeeds.

## Task 8: Setup Check Script And Unreal Setup Docs

**Files:**

- Create: `/Users/ethanstuart/Projects/everwild-odyssey/Scripts/check-unreal.sh`
- Create: `/Users/ethanstuart/Projects/everwild-odyssey/Docs/setup-unreal-macos.md`

- [ ] **Step 1: Create setup script**

Create `Scripts/check-unreal.sh`:

```bash
#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
PROJECT_FILE="$ROOT_DIR/EverwildOdyssey.uproject"

echo "Everwild Odyssey setup check"
echo "Project: $PROJECT_FILE"

if [[ ! -f "$PROJECT_FILE" ]]; then
  echo "Missing EverwildOdyssey.uproject"
  exit 1
fi

echo
echo "Toolchain:"
if command -v xcodebuild >/dev/null 2>&1; then
  xcodebuild -version
else
  echo "xcodebuild not found"
fi

if command -v clang >/dev/null 2>&1; then
  clang --version | head -n 1
else
  echo "clang not found"
fi

echo
echo "Unreal search:"
FOUND_UNREAL=0
for CANDIDATE in \
  "/Applications/Epic Games Launcher.app" \
  "/Applications/Unreal Engine.app" \
  "/Users/Shared/Epic Games" \
  "$HOME/Applications/Epic Games Launcher.app"; do
  if [[ -e "$CANDIDATE" ]]; then
    echo "Found: $CANDIDATE"
    FOUND_UNREAL=1
  fi
done

if [[ "$FOUND_UNREAL" -eq 0 ]]; then
  echo "No local Unreal/Epic install found in common macOS locations."
  echo "Install Unreal Engine with Epic Games Launcher, then open EverwildOdyssey.uproject."
fi

echo
echo "Git LFS:"
if command -v git-lfs >/dev/null 2>&1; then
  git-lfs --version
else
  echo "git-lfs not found; install it before committing binary Unreal assets."
fi
```

- [ ] **Step 2: Make setup script executable**

Run:

```bash
chmod +x Scripts/check-unreal.sh
```

Expected: command exits successfully.

- [ ] **Step 3: Create macOS Unreal setup doc**

Create `Docs/setup-unreal-macos.md`:

```markdown
# Unreal Setup On macOS

Everwild Odyssey is an Unreal Engine C++ project. Use an installed local Unreal version if one is already available. If Unreal is not installed, install it before trying to build or open the project.

## Current Local Status

At plan creation time, the machine had:

- Xcode 26.3
- Apple clang 17.0.0
- No Unreal Engine install found in common macOS locations
- No Epic Games Launcher found in common macOS locations
- No Git LFS command found

## Setup Steps

1. Install Epic Games Launcher from Epic's official Unreal Engine installation flow.
2. Install the latest stable Unreal Engine 5.x version available in the launcher, unless the project has already been associated with a specific local version.
3. Open `EverwildOdyssey.uproject`.
4. Let Unreal generate project files when prompted.
5. Build the `EverwildOdysseyEditor` target.
6. Run `Scripts/check-unreal.sh` from the project root after installation.

## Git LFS

The repo includes `.gitattributes` rules for Unreal binary assets, but the current first pass should avoid committing binary `.uasset` and `.umap` files until Git LFS is installed and initialized.

## Local-Only Assets

Do not commit Marketplace or premium assets unless their license explicitly allows public redistribution. Put local-only content in:

- `Content/LocalOnly/`
- `Content/Marketplace/`
- `Plugins/Marketplace/`

These paths are ignored by git.
```

- [ ] **Step 4: Run setup check**

Run:

```bash
Scripts/check-unreal.sh
```

Expected: script prints Xcode/clang details, reports Unreal not found until installed, and reports Git LFS status.

- [ ] **Step 5: Commit setup docs**

Run:

```bash
git add Scripts/check-unreal.sh Docs/setup-unreal-macos.md
git commit -m "docs: add Unreal setup checks"
```

Expected: commit succeeds.

## Task 9: Unreal Build Verification Gate

**Files:**

- Read: `/Users/ethanstuart/Projects/everwild-odyssey/EverwildOdyssey.uproject`
- Read: `/Users/ethanstuart/Projects/everwild-odyssey/Source`

- [ ] **Step 1: Locate Unreal after installation**

Run:

```bash
Scripts/check-unreal.sh
```

Expected if Unreal is not installed: script reports no local Unreal/Epic install found and exits successfully. Stop this task until Unreal is installed.

Expected if Unreal is installed: script prints at least one found Unreal/Epic path.

- [ ] **Step 2: Generate project files from Unreal**

Use the installed Unreal version to generate project files for `EverwildOdyssey.uproject`.

Expected: Unreal creates IDE/project metadata in ignored paths such as `.xcodeproj` or intermediate folders.

- [ ] **Step 3: Build editor target**

Build the `EverwildOdysseyEditor` target from Unreal Editor, Xcode, Rider, or Unreal Build Tool.

Expected: the build succeeds. If `BuildSettingsVersion.V5` is incompatible with the installed Unreal version, change both target files to the newest supported build settings version for that engine and commit the compatibility fix.

- [ ] **Step 4: Run automation tests**

Run the Unreal Automation tests:

```text
EverwildOdyssey.Core.HeroClassCatalog
EverwildOdyssey.Core.ProgressionModel
```

Expected: both tests pass.

- [ ] **Step 5: Commit Unreal compatibility fixes**

If any compatibility edits were needed, run:

```bash
git add Source Config EverwildOdyssey.uproject
git commit -m "fix: align project with installed Unreal version"
```

Expected: commit succeeds only if compatibility files changed.

## Task 10: Plan Completion Verification

**Files:**

- Read: `/Users/ethanstuart/Projects/everwild-odyssey/docs/superpowers/specs/2026-06-14-everwild-odyssey-design.md`
- Read: `/Users/ethanstuart/Projects/everwild-odyssey/docs/superpowers/plans/2026-06-14-everwild-odyssey-alpha-foundation.md`

- [ ] **Step 1: Confirm no red-flag placeholders in plan**

Run:

```bash
rg -n "T[B]D|T[O]DO|implement[ ]later|fill[ ]in[ ]details|add[ ]appropriate|write[ ]tests[ ]for[ ]the[ ]above|Similar[ ]to[ ]Task" docs/superpowers/plans/2026-06-14-everwild-odyssey-alpha-foundation.md
```

Expected: no matches.

- [ ] **Step 2: Confirm git status**

Run:

```bash
git status --short
```

Expected after task commits: no uncommitted tracked changes except optional ignored Unreal generated files.

- [ ] **Step 3: Push branch**

Run:

```bash
git push
```

Expected: commits are pushed to `origin/main`.

## Self-Review Notes

Spec coverage in this foundation plan:

- Unreal C++ first: covered by project descriptor, targets, module skeleton, and source files.
- Controller support: covered by input config and later runtime follow-up plan.
- Four classes: covered by class catalog and tests.
- Party/companion direction: covered by companion profile data, with runtime AI deferred to follow-up plan.
- Progression: covered by gear, rarity, save, and tests.
- Quest/faction: covered by quest and faction structs.
- Living world: covered by world event structs.
- Asset policy: covered by `.gitignore`, `.gitattributes`, README, and setup docs.
- Build verification: covered by Unreal setup and automation-test gate.

Known intentional deferments:

- Playable character, maps, UI, boss, content blockout, and companion AI are follow-up plans because Unreal is not currently installed and those systems require editor/runtime integration.
