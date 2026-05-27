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
    boat_cls = require_class("DSPrototypeBoatActor")
    spawned = []

    try:
        character = ACTOR_EDITOR.spawn_actor_from_class(
            reel_cls,
            unreal.Vector(0.0, -700.0, 120.0),
            unreal.Rotator(0.0, 0.0, 0.0),
        )
        spawned.append(character)
        if character is None:
            fail("could not spawn DSReelPrototypeCharacter")

        required_methods = [
            "execute_reel_action_on_target",
            "cast_reel_line_at_target",
            "start_reel_hold",
            "stop_reel_hold",
            "detach_reel_line",
            "update_reel_line",
            "is_reel_line_attached",
            "is_reel_hold_active",
            "get_reel_line_state_text",
            "get_reel_snap_count",
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

        if not character.cast_reel_line_at_target(grapple):
            fail("continuous Reel cast did not attach to grapple target")
        if not character.is_reel_line_attached():
            fail("continuous Reel line should be attached after cast")

        cast_state = character.get_reel_line_state_text()
        if cast_state not in ["CAST", "STRAIN", "DANGER"]:
            fail(f"unexpected line state after cast: {cast_state}")

        tension_before_reel = character.get_line_tension()
        character.start_reel_hold()
        character.update_reel_line(0.25)
        if not character.is_reel_hold_active():
            fail("Reel hold should be active while holding R")
        if character.get_line_tension() <= tension_before_reel:
            fail("line tension should increase while reeling")

        reel_state = character.get_reel_line_state_text()
        if reel_state not in ["REELING", "STRAIN", "DANGER", "SNAPPED"]:
            fail(f"unexpected line state while reeling: {reel_state}")

        character.stop_reel_hold()
        tension_before_ease = character.get_line_tension()
        character.update_reel_line(0.5)
        if character.get_line_tension() > tension_before_ease:
            fail("line tension should not increase after releasing R")

        character.detach_reel_line()
        if character.is_reel_line_attached():
            fail("line should detach after explicit detach")
        if "LINE DETACHED" not in character.get_last_reel_result():
            fail(f"unexpected detach result: {character.get_last_reel_result()}")

        snap_boat = ACTOR_EDITOR.spawn_actor_from_class(
            boat_cls,
            unreal.Vector(1500.0, -700.0, 0.0),
            unreal.Rotator(0.0, 0.0, 0.0),
        )
        spawned.append(snap_boat)
        if snap_boat is None:
            fail("could not spawn snap-test boat")
        snap_boat.set_water_depth(30.0)

        if not character.cast_reel_line_at_target(snap_boat):
            fail("continuous Reel cast did not attach to snap-test boat")

        snap_count_before = character.get_reel_snap_count()
        character.start_reel_hold()
        for _ in range(8):
            character.update_reel_line(0.5)

        if character.get_reel_snap_count() <= snap_count_before:
            fail("line should snap after sustained over-tension")
        if character.is_reel_line_attached():
            fail("line should detach when snapped")
        if "LINE SNAP" not in character.get_last_reel_result():
            fail(f"unexpected snap result: {character.get_last_reel_result()}")
        if "SNAPPED" not in character.get_reel_line_state_text():
            fail(f"expected snapped line state, got {character.get_reel_line_state_text()}")

        character.update_reel_line(4.0)
        if character.get_reel_line_state_text() != "IDLE":
            fail(f"line state should reset after snap tension eases, got {character.get_reel_line_state_text()}")

    finally:
        for actor in reversed(spawned):
            if actor:
                ACTOR_EDITOR.destroy_actor(actor)

    unreal.log("DockShield Reel smoke validation passed: invalid target, grapple, rescue, continuous reel, and snap states executed.")


main()
