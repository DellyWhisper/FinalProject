# WAS_JRPG GAS Foundation Plan

## Skill Routing
- Primary skill: `ue5-architecture`
- Secondary skill: `ue5-cpp-gameplay`
- Route confidence: `0.97`
- Route reason: this task is mainly a module-boundary and runtime architecture design for a UE5 plugin, with one required exported gameplay base class that will later become the parent of a project class.

## Current Repo Snapshot
- `Plugins/WAS_JRPG` already exists as a runtime plugin scaffold.
- `Plugins/WAS_JRPG/WAS_JRPG.uplugin` already allows plugin content.
- `Source/FinalProject/Public/Characters/WAS_Character_Base.h` currently defines `AWAS_Character_Base` as a plain `AActor`.
- `FinalProject.uproject` already enables the `GameplayAbilities` plugin.
- There is no existing GAS runtime code in `WAS_JRPG` yet.
- The current task is architecture and planning only. Do not modify the main project module yet.

## Architecture Decision
- Do not create a separate `WAS_GAS` plugin.
- Put the full base GAS foundation inside `Plugins/WAS_JRPG`.
- `WAS_JRPG` must own its own exported parent class: `AWAS_JRPG_CharacterBase`.
- `AWAS_Character_Base` in the main project is expected to become a child of `AWAS_JRPG_CharacterBase` later, but that migration is explicitly out of scope for the current phase.
- The plugin should expose a clean runtime GAS API that the main project can consume later without the main project directly owning the GAS core.

## Design Goals
- Keep all base GAS code inside `WAS_JRPG`.
- Keep the plugin runtime-safe and reusable for player characters and AI units.
- Make `AWAS_JRPG_CharacterBase` the only required parent for GAS-aware gameplay characters.
- Keep the initial foundation generic enough for reuse, even though it lives in a JRPG-named plugin.
- Keep project-specific content, player wiring, and UI integration out of the plugin core.
- Avoid cross-dependencies from `WAS_JRPG` back into `FinalProject`.

## Explicit Non-Goals For This Phase
- Do not reparent `AWAS_Character_Base` yet.
- Do not edit `FinalProject.Build.cs` yet.
- Do not add project-specific input mapping, widgets, HUD, or player controller code yet.
- Do not add turn-order systems, battle flow managers, or menu logic yet.
- Do not split editor tooling into a second module yet.

## Target Dependency Graph
```text
Engine
  + Core / CoreUObject / Engine
  + GameplayAbilities / GameplayTags / GameplayTasks
            |
            v
        WAS_JRPG
            |
            v
   FinalProject (later integration only)
```

Rules:
- `WAS_JRPG` may depend on Unreal GAS modules.
- `FinalProject` may later depend on `WAS_JRPG`.
- `WAS_JRPG` must never depend on `FinalProject`.
- Core GAS foundation and future JRPG battle extensions should live in different folders inside the same plugin, even if they share one runtime module for now.

## Runtime Layering Inside `WAS_JRPG`
Even though everything lives in one plugin, the code should still be layered internally:

1. `AbilitySystem/Core`
   - ability system component subclass
   - base attribute set
   - base gameplay ability
   - startup ability/effect grant data
   - native gameplay tags
   - common helper structs

2. `Characters`
   - `AWAS_JRPG_CharacterBase`
   - character lifecycle and GAS bootstrap
   - no project UI or map-specific behavior

3. `Battle` or `JRPG`
   - future turn-based, AP, initiative, targeting, battle states
   - should build on top of the core GAS layer
   - not required in the first implementation pass

This keeps the plugin logically split without introducing multiple modules too early.

## Recommended Plugin Folder Layout
```text
Plugins/WAS_JRPG/
  WAS_JRPG.uplugin
  Config/
    DefaultGameplayTags.ini
  Content/
    AbilitySets/
    Abilities/
    Effects/
  Source/WAS_JRPG/
    WAS_JRPG.Build.cs
    Public/
      WAS_JRPG.h
      Characters/
        WAS_JRPG_CharacterBase.h
      AbilitySystem/Core/
        WAS_JRPG_AbilitySystemComponent.h
        WAS_JRPG_AttributeSetBase.h
        WAS_JRPG_GameplayAbility.h
        WAS_JRPG_AbilitySet.h
        WAS_JRPG_GameplayTags.h
      AbilitySystem/Types/
        WAS_JRPG_GrantedAbility.h
        WAS_JRPG_AbilityActivationPolicy.h
    Private/
      WAS_JRPG.cpp
      Characters/
        WAS_JRPG_CharacterBase.cpp
      AbilitySystem/Core/
        WAS_JRPG_AbilitySystemComponent.cpp
        WAS_JRPG_AttributeSetBase.cpp
        WAS_JRPG_GameplayAbility.cpp
        WAS_JRPG_AbilitySet.cpp
        WAS_JRPG_GameplayTags.cpp
```

Notes:
- Keep only stable exported types in `Public/`.
- Keep implementation helpers and registration details in `Private/`.
- Do not expose project-only helpers through the plugin public API.

## `WAS_JRPG.Build.cs` Target State
The current build file is still template-like and includes `Slate`/`SlateCore` by default. For a runtime GAS foundation, the target should be cleaned up.

Recommended dependency policy:

```csharp
PublicDependencyModuleNames.AddRange(new string[]
{
    "Core",
    "CoreUObject",
    "Engine",
    "GameplayAbilities",
    "GameplayTags",
    "GameplayTasks"
});

PrivateDependencyModuleNames.AddRange(new string[]
{
});
```

Optional dependencies only when truly needed:
- `EnhancedInput`: only if the plugin itself owns input binding helpers.
- `AIModule`: only if AI-specific runtime code is added to the plugin.
- `DeveloperSettings`: only if plugin-wide settings objects are introduced.
- `NetCore`: only if a concrete replication helper requires it.

Rules:
- Remove `Slate` and `SlateCore` unless the plugin actually implements UI widgets.
- Keep the runtime module lean.
- Do not add `FinalProject` as a dependency under any circumstance.

## `WAS_JRPG.uplugin` Expectations
The plugin should remain a single runtime plugin for now with content enabled.

Recommended expectations:
- `Type`: `Runtime`
- `LoadingPhase`: `Default`
- `CanContainContent`: `true`
- Add plugin dependency metadata for `GameplayAbilities` if needed for portability

The plugin should be the owner of:
- core GAS code
- plugin content assets for common abilities/effects/ability sets
- gameplay tags used by the GAS foundation

## Core Exported Types

### 1. `AWAS_JRPG_CharacterBase`
Base class for any GAS-aware playable or AI character that should live under the plugin.

Inheritance:
- `ACharacter`
- `IAbilitySystemInterface`

Why `ACharacter` instead of `AActor`:
- built-in movement support
- correct avatar model for most character abilities
- cleaner path for animation and locomotion later
- avoids redoing standard pawn behavior in the project layer

Core responsibilities:
- own or expose the ability system component
- own the default base attribute set
- initialize GAS actor info at the correct lifecycle points
- grant startup abilities on the server
- apply startup effects on the server
- provide safe virtual hooks for derived classes
- expose minimal Blueprint-safe getters and events

Must not own:
- project HUD logic
- map-specific setup
- turn manager rules
- inventory UI
- quest logic
- player input mapping assets from `FinalProject`

Required members:
- `TObjectPtr<UWAS_JRPG_AbilitySystemComponent> AbilitySystemComponent`
- `TObjectPtr<UWAS_JRPG_AttributeSetBase> AttributeSetBase`
- startup data references such as one or more `UWAS_JRPG_AbilitySet`
- guard state such as `bStartupAbilitiesGranted`

Recommended lifecycle methods:
- constructor: create components, disable unnecessary tick by default, set replication flags
- `BeginPlay()`: final safe bootstrap checks
- `PossessedBy(AController*)`: authoritative init path
- `OnRep_Controller()`: client-side actor info refresh when needed
- `GetAbilitySystemComponent() const`: interface implementation
- optional protected hooks:
  - `InitializeAbilityActorInfo()`
  - `GrantStartupAbilities()`
  - `ApplyStartupEffects()`
  - `HandleDeath()`
  - `OnAbilitySystemInitialized()`

Design rule:
- keep the character base generic
- use virtual hooks instead of hardcoding project rules into the plugin

### 2. `UWAS_JRPG_AbilitySystemComponent`
Runtime ASC subclass for plugin-level helpers.

Responsibilities:
- central GAS access point
- ability grant helper methods
- gameplay tag event binding helpers
- optional utility wrappers for ability activation by input tags
- debugging helpers for development logging

Must not become:
- a container for project-specific combat state
- a replacement for character or controller logic

Recommended helper surface:
- `GrantAbilitySet(...)`
- `ClearAbilitySet(...)`
- `TryActivateAbilitiesByTag(...)`
- tag listener registration helpers

### 3. `UWAS_JRPG_AttributeSetBase`
Minimal shared attribute foundation.

Recommended first-pass attributes:
- `Health`
- `MaxHealth`
- `Mana`
- `MaxMana`
- `Stamina`
- `MaxStamina`
- optional `MoveSpeed`

Why keep it minimal:
- these are safe shared runtime resources
- JRPG-only battle values such as initiative or action points can be layered later without bloating the base set

Responsibilities:
- replicated attribute properties
- `OnRep_` functions
- clamping rules
- post-effect processing
- consistent death/out-of-health trigger points

Do not put these into the first base set:
- turn order
- initiative queues
- menu state
- target selection state
- battle phase flags

Those belong in a later JRPG battle layer, even if still inside the same plugin.

### 4. `UWAS_JRPG_GameplayAbility`
Base ability class for shared plugin-level defaults.

Responsibilities:
- common activation policy
- input tag ownership
- standard block/cancel tags
- utility helpers shared across plugin abilities

Recommended properties:
- input tag
- startup activation policy enum
- ability cost and cooldown configuration references

Design rule:
- keep actual tuning data in assets and effects
- do not hardcode numbers in the ability base class

### 5. `UWAS_JRPG_AbilitySet`
Data-driven startup grant container.

Recommended base class:
- `UPrimaryDataAsset` if the team wants asset-manager-friendly discovery
- `UDataAsset` if a simpler first pass is preferred

Responsibilities:
- list of gameplay ability classes to grant
- ability levels
- optional input tags
- startup gameplay effects
- optional attribute initialization effects

This keeps character construction data-driven and prevents hardcoded grant logic in `AWAS_JRPG_CharacterBase`.

### 6. `FWAS_JRPG_GrantedAbility`
Simple data struct stored inside `UWAS_JRPG_AbilitySet`.

Recommended fields:
- `TSubclassOf<UWAS_JRPG_GameplayAbility>` or `TSubclassOf<UGameplayAbility>`
- `int32 AbilityLevel`
- `FGameplayTag InputTag`
- `bool bGrantOnSpawn`

### 7. `FWAS_JRPG_GameplayTags`
Central native tag registry owned by the plugin.

Recommended namespaces:
- `Input.Ability.*`
- `Ability.Activate.*`
- `Ability.Fail.*`
- `State.*`
- `Status.*`
- `Event.*`

Examples:
- `Input.Ability.Primary`
- `Input.Ability.Secondary`
- `Ability.Fail.Cooldown`
- `Ability.Fail.Cost`
- `Status.Dead`
- `Status.Stunned`

Rule:
- the plugin owns its own baseline tags
- later project-specific tags can extend them without moving base ownership out of the plugin

## GAS Ownership Strategy
For the first implementation pass, use a character-owned ASC inside `AWAS_JRPG_CharacterBase`.

Reason:
- simpler setup
- easier to validate in isolation inside the plugin
- avoids immediate `PlayerState` complexity before the project layer is integrated

Deferred decision:
- if later gameplay requires persistent player GAS state across respawns, add a project-specific or plugin-extended `PlayerState` pattern later
- do not overdesign for that requirement in phase one

## Character Initialization Flow
Target lifecycle flow for `AWAS_JRPG_CharacterBase`:

1. Constructor
   - create `AbilitySystemComponent`
   - create `AttributeSetBase`
   - configure replication
   - disable tick unless a real reason exists

2. Server possession path
   - call `InitializeAbilityActorInfo()`
   - grant startup ability sets once
   - apply startup effects once

3. Client refresh path
   - refresh actor info when controller replication becomes valid
   - ensure Blueprint-facing ability queries do not run before init

4. Runtime events
   - gameplay effects modify attributes
   - attribute set handles clamp and death thresholds
   - character exposes overridable death/disabled hooks

Rules:
- startup grants happen on the authority path only
- never duplicate grants on every possession without handles or guard flags
- keep ability bootstrap idempotent

## Input Policy
The plugin should prepare for tag-driven ability input, but it should not own `FinalProject` input actions yet.

Recommended first-pass policy:
- abilities store an `InputTag`
- the plugin supports activation by gameplay tag
- actual `EnhancedInput` mapping can stay outside the plugin until project integration starts

Reason:
- this avoids pushing project input assets into the plugin too early
- it keeps the plugin clean and portable

## Content And Asset Naming
Recommended content naming inside `WAS_JRPG`:
- `GA_` for gameplay abilities
- `GE_` for gameplay effects
- `GC_` for gameplay cues if later needed
- `DA_` for ability set data assets
- `BP_` only for plugin test Blueprints or future derived characters

Examples:
- `DA_JRPG_DefaultAbilitySet`
- `GE_JRPG_InitAttributes`
- `GA_JRPG_BasicAttack`
- `GA_JRPG_Guard`

## Public API Boundary Rules
Put in `Public/` only:
- `AWAS_JRPG_CharacterBase`
- base ASC type
- base attribute set
- base gameplay ability
- ability set data asset
- shared enums/structs/tags needed by consumers

Keep in `Private/`:
- helper implementations
- tag registration internals
- non-exported utility functions
- any glue code not required by child classes

Rules:
- prefer forward declarations in public headers
- include concrete headers in `.cpp`
- avoid giant umbrella includes

## Relationship To `AWAS_Character_Base`
Planned target relationship:

```text
AWAS_JRPG_CharacterBase   (plugin, parent)
        ^
        |
AWAS_Character_Base       (main project, child later)
```

Important constraints:
- do not touch `AWAS_Character_Base` in this phase
- design `AWAS_JRPG_CharacterBase` so the future reparent is straightforward
- `AWAS_Character_Base` should later become a thin project-specific extension, not the owner of core GAS code

That future child class should own only:
- project-specific visuals
- project-specific defaults
- map/game-mode wiring
- any non-plugin content references

## Implementation Order

### Phase 1: Clean Plugin Runtime Dependencies
Deliverables:
- clean `WAS_JRPG.Build.cs`
- ensure plugin dependency on GAS modules is explicit
- keep runtime dependency set minimal

Exit criteria:
- plugin compiles with GAS module dependencies only
- no unnecessary UI modules remain in the runtime build file

### Phase 2: Add Core GAS Types To The Plugin
Deliverables:
- `UWAS_JRPG_AbilitySystemComponent`
- `UWAS_JRPG_AttributeSetBase`
- `UWAS_JRPG_GameplayAbility`
- `FWAS_JRPG_GrantedAbility`
- `UWAS_JRPG_AbilitySet`
- `FWAS_JRPG_GameplayTags`

Exit criteria:
- exported types compile
- base API is available to the plugin consumer layer
- no project references exist

### Phase 3: Add `AWAS_JRPG_CharacterBase`
Deliverables:
- character base class with ASC and base attributes
- authority-safe startup grant path
- init hooks and Blueprint-safe getters

Exit criteria:
- a plugin-owned character can initialize GAS correctly
- startup grants happen once
- no dependency on `FinalProject`

### Phase 4: Add Plugin Config And Starter Content
Deliverables:
- gameplay tags config
- one default ability set asset
- one base init effect asset

Exit criteria:
- the plugin can bootstrap test content without project assets

### Phase 5: Plugin-Only Validation
Deliverables:
- test actor or test Blueprint inside the plugin if needed
- validation notes for ability grant, tag activation, and attribute change flow

Exit criteria:
- the GAS foundation is proven inside `WAS_JRPG`
- the main project still remains untouched

### Phase 6: Future Main Project Adoption
Out of scope for now, but planned later:
- reparent `AWAS_Character_Base` to `AWAS_JRPG_CharacterBase`
- add `WAS_JRPG` dependency to the main project build rules if needed
- migrate project-specific defaults into the child class

## Validation Checklist
- `WAS_JRPG` compiles with `GameplayAbilities`, `GameplayTags`, and `GameplayTasks`.
- No `FinalProject` include or dependency exists inside the plugin.
- `AWAS_JRPG_CharacterBase` lives in plugin public headers and is usable as a future parent class.
- ASC and base attribute set initialize predictably.
- Startup abilities are granted once on authority only.
- Base attributes clamp correctly.
- Plugin gameplay tags load without relying on project-local tags.
- The plugin can be reasoned about in isolation before any main project migration starts.

## Anti-Patterns To Avoid
- Do not put all battle logic directly into `AWAS_JRPG_CharacterBase`.
- Do not keep `Slate` dependencies in a runtime-only GAS module without real need.
- Do not hardcode project asset paths inside plugin C++.
- Do not let the plugin depend on `FinalProject`.
- Do not put turn manager state into the base attribute set.
- Do not grant abilities on both client and server paths.
- Do not store UI logic inside the ASC subclass.

## Recommended First File Set When Implementation Begins
- `Plugins/WAS_JRPG/Source/WAS_JRPG/Public/Characters/WAS_JRPG_CharacterBase.h`
- `Plugins/WAS_JRPG/Source/WAS_JRPG/Private/Characters/WAS_JRPG_CharacterBase.cpp`
- `Plugins/WAS_JRPG/Source/WAS_JRPG/Public/AbilitySystem/Core/WAS_JRPG_AbilitySystemComponent.h`
- `Plugins/WAS_JRPG/Source/WAS_JRPG/Private/AbilitySystem/Core/WAS_JRPG_AbilitySystemComponent.cpp`
- `Plugins/WAS_JRPG/Source/WAS_JRPG/Public/AbilitySystem/Core/WAS_JRPG_AttributeSetBase.h`
- `Plugins/WAS_JRPG/Source/WAS_JRPG/Private/AbilitySystem/Core/WAS_JRPG_AttributeSetBase.cpp`
- `Plugins/WAS_JRPG/Source/WAS_JRPG/Public/AbilitySystem/Core/WAS_JRPG_GameplayAbility.h`
- `Plugins/WAS_JRPG/Source/WAS_JRPG/Private/AbilitySystem/Core/WAS_JRPG_GameplayAbility.cpp`
- `Plugins/WAS_JRPG/Source/WAS_JRPG/Public/AbilitySystem/Core/WAS_JRPG_AbilitySet.h`
- `Plugins/WAS_JRPG/Source/WAS_JRPG/Private/AbilitySystem/Core/WAS_JRPG_AbilitySet.cpp`
- `Plugins/WAS_JRPG/Source/WAS_JRPG/Public/AbilitySystem/Core/WAS_JRPG_GameplayTags.h`
- `Plugins/WAS_JRPG/Source/WAS_JRPG/Private/AbilitySystem/Core/WAS_JRPG_GameplayTags.cpp`
- `Plugins/WAS_JRPG/Config/DefaultGameplayTags.ini`

## Final Decision Summary
- `WAS_JRPG` is the owner of the base GAS runtime.
- `AWAS_JRPG_CharacterBase` is the plugin parent class.
- `AWAS_Character_Base` in the main project will later inherit from it.
- The current phase stops at plugin-side architecture and planning.
- The main project remains untouched until the plugin foundation is stable.
