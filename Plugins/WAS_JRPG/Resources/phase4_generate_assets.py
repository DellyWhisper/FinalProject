import unreal

ASSET_TOOLS = unreal.AssetToolsHelpers.get_asset_tools()
EDITOR_ASSET_LIB = unreal.EditorAssetLibrary

EFFECTS_DIR = "/WAS_JRPG/Effects"
ABILITY_SETS_DIR = "/WAS_JRPG/AbilitySets"
GE_ASSET_PATH = f"{EFFECTS_DIR}/GE_JRPG_InitAttributes"
DA_ASSET_PATH = f"{ABILITY_SETS_DIR}/DA_JRPG_DefaultAbilitySet"


def ensure_directory(path: str) -> None:
    if not EDITOR_ASSET_LIB.does_directory_exist(path):
        unreal.log(f"Creating directory {path}")
        EDITOR_ASSET_LIB.make_directory(path)


def create_or_load_gameplay_effect_blueprint():
    ensure_directory(EFFECTS_DIR)

    if EDITOR_ASSET_LIB.does_asset_exist(GE_ASSET_PATH):
        unreal.log(f"Gameplay effect asset already exists: {GE_ASSET_PATH}")
        return EDITOR_ASSET_LIB.load_asset(GE_ASSET_PATH)

    factory = unreal.BlueprintFactory()
    factory.set_editor_property("parent_class", unreal.WAS_JRPG_InitAttributesEffect)

    asset = ASSET_TOOLS.create_asset(
        asset_name="GE_JRPG_InitAttributes",
        package_path=EFFECTS_DIR,
        asset_class=unreal.Blueprint,
        factory=factory,
    )

    if not asset:
        raise RuntimeError("Failed to create GE_JRPG_InitAttributes blueprint asset.")

    EDITOR_ASSET_LIB.save_loaded_asset(asset)
    unreal.log(f"Created gameplay effect asset: {GE_ASSET_PATH}")
    return asset


def create_or_load_ability_set():
    ensure_directory(ABILITY_SETS_DIR)

    if EDITOR_ASSET_LIB.does_asset_exist(DA_ASSET_PATH):
        unreal.log(f"Ability set asset already exists: {DA_ASSET_PATH}")
        return EDITOR_ASSET_LIB.load_asset(DA_ASSET_PATH)

    factory = unreal.DataAssetFactory()
    factory.set_editor_property("data_asset_class", unreal.WAS_JRPG_AbilitySet)

    asset = ASSET_TOOLS.create_asset(
        asset_name="DA_JRPG_DefaultAbilitySet",
        package_path=ABILITY_SETS_DIR,
        asset_class=unreal.WAS_JRPG_AbilitySet,
        factory=factory,
    )

    if not asset:
        raise RuntimeError("Failed to create DA_JRPG_DefaultAbilitySet data asset.")

    EDITOR_ASSET_LIB.save_loaded_asset(asset)
    unreal.log(f"Created ability set asset: {DA_ASSET_PATH}")
    return asset


def configure_ability_set(asset) -> None:
    ge_class = EDITOR_ASSET_LIB.load_blueprint_class(GE_ASSET_PATH)
    if not ge_class:
        raise RuntimeError(f"Failed to resolve generated class for {GE_ASSET_PATH}")

    asset.configure_default_init_effect(ge_class, 1.0)

    EDITOR_ASSET_LIB.save_loaded_asset(asset)
    unreal.log(f"Configured ability set asset: {DA_ASSET_PATH}")


try:
    create_or_load_gameplay_effect_blueprint()
    ability_set_asset = create_or_load_ability_set()
    configure_ability_set(ability_set_asset)
    unreal.log("WAS_JRPG Phase 4 asset generation completed.")
except Exception as exc:
    unreal.log_error(f"WAS_JRPG Phase 4 asset generation failed: {exc}")
    raise
finally:
    unreal.SystemLibrary.quit_editor()
