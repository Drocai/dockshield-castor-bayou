import os

import unreal


MAP_PATH = "/Game/DockShield/Maps/M_Test_Targeting"
PROJECT_CONFIG = os.path.join(unreal.Paths.project_config_dir(), "DefaultGame.ini")

LEVEL_EDITOR = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
ACTOR_EDITOR = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)


def fail(message):
    raise RuntimeError(f"DockShield runtime validation failed: {message}")


def require_class(name):
    cls = getattr(unreal, name, None)
    if cls is None:
        fail(f"missing reflected class unreal.{name}")
    return cls


def require_property(obj, property_name):
    try:
        return obj.get_editor_property(property_name)
    except Exception as exc:
        fail(f"{obj.get_name()} missing property {property_name}: {exc}")


def reflected_text(value):
    return str(value)


def validate_classes():
    game_mode_cls = require_class("DSPrototypeGameMode")
    reel_cls = require_class("DSReelPrototypeCharacter")
    hud_cls = require_class("DSPrototypeHUD")
    player_controller_cls = require_class("DSPrototypePlayerController")
    require_class("DSTargetableComponent")

    cdo = unreal.get_default_object(game_mode_cls)
    default_pawn_class = require_property(cdo, "default_pawn_class")
    hud_class = require_property(cdo, "hud_class")
    player_controller_class = require_property(cdo, "player_controller_class")

    if "DSReelPrototypeCharacter" not in reflected_text(default_pawn_class):
        fail(
            "DSPrototypeGameMode default pawn mismatch: "
            f"{reflected_text(default_pawn_class)} does not contain DSReelPrototypeCharacter"
        )

    if "DSPrototypeHUD" not in reflected_text(hud_class):
        fail(
            "DSPrototypeGameMode HUD mismatch: "
            f"{reflected_text(hud_class)} does not contain DSPrototypeHUD"
        )

    if "DSPrototypePlayerController" not in reflected_text(player_controller_class):
        fail(
            "DSPrototypeGameMode player controller mismatch: "
            f"{reflected_text(player_controller_class)} does not contain DSPrototypePlayerController"
        )


def validate_project_defaults():
    if not os.path.exists(PROJECT_CONFIG):
        fail(f"missing project config {PROJECT_CONFIG}")

    with open(PROJECT_CONFIG, "r", encoding="utf-8") as handle:
        config_text = handle.read()

    expected_fragments = [
        "EditorStartupMap=/Game/DockShield/Maps/M_Test_Targeting.M_Test_Targeting",
        "GameDefaultMap=/Game/DockShield/Maps/M_Test_Targeting.M_Test_Targeting",
        "GlobalDefaultGameMode=/Script/DockShield.DSPrototypeGameMode",
    ]
    for fragment in expected_fragments:
        if fragment not in config_text:
            fail(f"DefaultGame.ini missing {fragment}")


def validate_target(label, expected_prompt_fragment):
    targetable_cls = require_class("DSTargetableComponent")
    actor = next(
        (item for item in ACTOR_EDITOR.get_all_level_actors() if item.get_actor_label() == label),
        None,
    )
    if actor is None:
        fail(f"missing actor {label}")

    component = actor.get_component_by_class(targetable_cls)
    if component is None:
        fail(f"{label} missing DSTargetableComponent")

    component.configure_from_owner_tags()
    prompt = component.get_reel_prompt()
    if expected_prompt_fragment not in prompt:
        fail(f"{label} prompt mismatch: expected '{expected_prompt_fragment}' in '{prompt}'")


def validate_map():
    if not unreal.EditorAssetLibrary.does_asset_exist(MAP_PATH):
        fail(f"missing map asset {MAP_PATH}")

    LEVEL_EDITOR.load_level(MAP_PATH)
    validate_target("Target_GrapplePull_Debug", "Grapple Pull")
    validate_target("Target_CivilianRescue_Debug", "Rescue Reel")
    validate_target("Hazard_ToxicWater_Debug", "no Reel action")


def main():
    validate_project_defaults()
    validate_classes()
    validate_map()
    unreal.log("DockShield runtime validation passed: game mode, pawn, HUD, and target prompts.")


main()
