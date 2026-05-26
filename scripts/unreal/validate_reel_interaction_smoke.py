import math

import unreal


MAP_PATH = "/Game/DockShield/Maps/M_Test_Targeting"

LEVEL_EDITOR = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
ACTOR_EDITOR = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)


def fail(message):
    raise RuntimeError(f"DockShield Reel smoke validation failed: {message}")


def require_class(name):
    cls = getattr(unreal, name, None)
    if cls is None:
        fail(f"missing reflected class unreal.{name}")
    return cls


def actor_by_label(label):
    actor = next(
        (item for item in ACTOR_EDITOR.get_all_level_actors() if item.get_actor_label() == label),
        None,
    )
    if actor is None:
        fail(f"missing actor {label}")
    return actor


def distance(a, b):
    return math.sqrt(
        ((a.x - b.x) ** 2)
        + ((a.y - b.y) ** 2)
        + ((a.z - b.z) ** 2)
    )


def main():
    if not unreal.EditorAssetLibrary.does_asset_exist(MAP_PATH):
        fail(f"missing map asset {MAP_PATH}")

    LEVEL_EDITOR.load_level(MAP_PATH)

    reel_cls = require_class("DSReelPrototypeCharacter")
    character = ACTOR_EDITOR.spawn_actor_from_class(
        reel_cls,
        unreal.Vector(0.0, -700.0, 120.0),
        unreal.Rotator(0.0, 0.0, 0.0),
    )
    if character is None:
        fail("could not spawn DSReelPrototypeCharacter")

    try:
        required_methods = [
            "execute_reel_action_on_target",
            "get_grapple_pull_count",
            "get_civilian_rescue_count",
            "get_last_reel_result",
            "get_line_tension",
        ]
        for method_name in required_methods:
            if not hasattr(character, method_name):
                fail(f"DSReelPrototypeCharacter missing Python method {method_name}")

        grapple = actor_by_label("Target_GrapplePull_Debug")
        civilian = actor_by_label("Target_CivilianRescue_Debug")
        hazard = actor_by_label("Hazard_ToxicWater_Debug")

        if character.execute_reel_action_on_target(hazard):
            fail("hazard should reject Reel action")
        if "NO VALID TARGET" not in character.get_last_reel_result():
            fail(f"unexpected invalid target result: {character.get_last_reel_result()}")

        if not character.execute_reel_action_on_target(grapple):
            fail("grapple target did not execute")
        if character.get_grapple_pull_count() != 1:
            fail(f"expected one grapple pull, got {character.get_grapple_pull_count()}")

        if not character.execute_reel_action_on_target(civilian):
            fail("civilian rescue target did not execute")
        if character.get_civilian_rescue_count() != 1:
            fail(f"expected one civilian rescue, got {character.get_civilian_rescue_count()}")

        rescue_distance = distance(character.get_actor_location(), civilian.get_actor_location())
        if rescue_distance > 260.0:
            fail(f"civilian was not pulled near the character, distance={rescue_distance:.1f}")

        line_tension = character.get_line_tension()
        if line_tension < 0.0 or line_tension > 1.0:
            fail(f"line tension outside 0..1: {line_tension}")

    finally:
        ACTOR_EDITOR.destroy_actor(character)

    unreal.log("DockShield Reel smoke validation passed: invalid target, grapple, and rescue actions executed.")


main()
