import unreal


MAP_PATH = "/Game/DockShield/Maps/M_Test_Targeting"
MATERIAL_DIR = "/Game/DockShield/Environment/DeepDock/Materials"

LEVEL_EDITOR = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
ACTOR_EDITOR = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

REQUIRED_ACTORS = {
    "DS_DeepDock_BossArena_State",
    "DS_DeepDock_Arena_Floor",
    "DS_DeepDock_Boss_Core_Target",
    "DS_DeepDock_Boss_WeakPoint_Left",
    "DS_DeepDock_Boss_WeakPoint_Right",
    "DS_DeepDock_Boss_WeakPoint_Crown",
    "DS_DeepDock_Toxic_Pool",
    "DS_DeepDock_Light_BioCore",
}

REQUIRED_MATERIALS = {
    "M_DS_DeepDockWetDeck_Proto",
    "M_DS_DeepDockWarningRed_Proto",
    "M_DS_DeepDockBioCore_Proto",
    "M_DS_DeepDockWeakPoint_Proto",
    "M_DS_DeepDockBlackWater_Proto",
}


def fail(message):
    raise RuntimeError(f"DockShield Deep Dock boss arena validation failed: {message}")


def require_class(name):
    cls = getattr(unreal, name, None)
    if cls is None:
        fail(f"missing reflected class unreal.{name}")
    return cls


def actor_by_label(label):
    return next((actor for actor in ACTOR_EDITOR.get_all_level_actors() if actor.get_actor_label() == label), None)


def validate_assets():
    for material_name in REQUIRED_MATERIALS:
        asset_path = f"{MATERIAL_DIR}/{material_name}"
        if not unreal.EditorAssetLibrary.does_asset_exist(asset_path):
            fail(f"missing material {asset_path}")


def validate_actor_set():
    missing = sorted(label for label in REQUIRED_ACTORS if actor_by_label(label) is None)
    if missing:
        fail(f"missing actors: {', '.join(missing)}")

    arena_cls = require_class("DSDeepDockBossArenaActor")
    arena = actor_by_label("DS_DeepDock_BossArena_State")
    if not isinstance(arena, arena_cls):
        fail("DS_DeepDock_BossArena_State is not a DSDeepDockBossArenaActor")

    status = str(arena.get_arena_status_text())
    if "DEEP DOCK" not in status or "CORE" not in status:
        fail(f"arena status missing expected fragments: {status}")


def validate_target(label, expected_tag, expected_prompt_fragment):
    targetable_cls = require_class("DSTargetableComponent")
    actor = actor_by_label(label)
    if actor is None:
        fail(f"missing actor {label}")

    tags = {str(tag) for tag in actor.tags}
    if expected_tag not in tags:
        fail(f"{label} missing tag {expected_tag}; tags were {tags}")

    component = actor.get_component_by_class(targetable_cls)
    if component is None:
        fail(f"{label} missing DSTargetableComponent")

    component.configure_from_owner_tags()
    prompts = " | ".join(
        [
            str(component.get_reel_prompt()),
            str(component.get_fly_prompt()),
            str(component.get_lilly_prompt()),
            str(component.get_combo_state_text()),
        ]
    )
    if expected_prompt_fragment not in prompts:
        fail(f"{label} prompts missing '{expected_prompt_fragment}': {prompts}")


def validate_targets():
    validate_target("DS_DeepDock_Boss_Core_Target", "Boss", "boss core")
    validate_target("DS_DeepDock_Boss_WeakPoint_Left", "WeakPoint", "weak point")
    validate_target("DS_DeepDock_Boss_WeakPoint_Right", "WeakPoint", "weak point")
    validate_target("DS_DeepDock_Boss_WeakPoint_Crown", "WeakPoint", "weak point")


def main():
    if not unreal.EditorAssetLibrary.does_asset_exist(MAP_PATH):
        fail(f"missing map asset {MAP_PATH}")
    LEVEL_EDITOR.load_level(MAP_PATH)
    validate_assets()
    validate_actor_set()
    validate_targets()
    unreal.log("DockShield Deep Dock boss arena validation passed.")


main()
