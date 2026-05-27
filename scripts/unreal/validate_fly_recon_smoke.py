import math

import unreal


MAP_PATH = "/Game/DockShield/Maps/M_Test_Targeting"

LEVEL_EDITOR = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
ACTOR_EDITOR = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)


def fail(message):
    raise RuntimeError(f"DockShield Fly recon smoke validation failed: {message}")


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


def read_bool_property(obj, property_name):
    try:
        return bool(obj.get_editor_property(property_name))
    except Exception as exc:
        fail(f"{obj.get_name()} missing bool property {property_name}: {exc}")


def main():
    if not unreal.EditorAssetLibrary.does_asset_exist(MAP_PATH):
        fail(f"missing map asset {MAP_PATH}")

    LEVEL_EDITOR.load_level(MAP_PATH)

    fly_cls = require_class("DSFlyPrototypeCharacter")
    targetable_cls = require_class("DSTargetableComponent")
    spawned = []

    try:
        fly = ACTOR_EDITOR.spawn_actor_from_class(
            fly_cls,
            unreal.Vector(0.0, -700.0, 120.0),
            unreal.Rotator(0.0, 0.0, 0.0),
        )
        spawned.append(fly)
        if fly is None:
            fail("could not spawn DSFlyPrototypeCharacter")

        for method_name in [
            "sonar_pulse",
            "execute_fly_mark_on_target",
            "try_fly_mark",
            "get_last_sonar_hit_count",
            "get_marked_target_count",
            "get_last_recon_result",
            "get_sonar_range",
            "get_sonar_cone_degrees",
        ]:
            if not hasattr(fly, method_name):
                fail(f"DSFlyPrototypeCharacter missing Python method {method_name}")

        hazard = actor_by_label("Hazard_ToxicWater_Debug")
        targetable = hazard.get_component_by_class(targetable_cls)
        if targetable is None:
            fail("Hazard_ToxicWater_Debug missing DSTargetableComponent")

        for method_name in [
            "configure_from_owner_tags",
            "mark_for_fly",
            "clear_fly_mark",
            "get_fly_prompt",
        ]:
            if not hasattr(targetable, method_name):
                fail(f"DSTargetableComponent missing Python method {method_name}")

        targetable.configure_from_owner_tags()
        if not read_bool_property(targetable, "can_fly_mark"):
            fail("hazard should be available to The Fly as a markable recon target")

        if fly.get_sonar_range() < distance(fly.get_actor_location(), hazard.get_actor_location()):
            fail("hazard is outside The Fly sonar range smoke setup")

        sonar_hits = fly.sonar_pulse()
        if sonar_hits < 1:
            fail("sonar pulse should detect at least one DockShield target")
        if fly.get_last_sonar_hit_count() != sonar_hits:
            fail("sonar hit accessor did not retain the latest sonar pulse count")
        if "SONAR PULSE" not in fly.get_last_recon_result():
            fail(f"unexpected sonar result text: {fly.get_last_recon_result()}")

        if not fly.execute_fly_mark_on_target(hazard):
            fail("Fly mark should succeed on the toxic hazard")
        if not read_bool_property(targetable, "is_fly_marked"):
            fail("targetable component did not retain Fly mark state")
        if fly.get_marked_target_count() != 1:
            fail(f"expected one marked target, got {fly.get_marked_target_count()}")
        if "MARKED" not in targetable.get_fly_prompt():
            fail(f"Fly prompt did not reflect marked state: {targetable.get_fly_prompt()}")

        if not fly.execute_fly_mark_on_target(hazard):
            fail("re-marking an already marked target should still report success")
        if fly.get_marked_target_count() != 1:
            fail("re-marking one target should not inflate unique marked target count")

        targetable.clear_fly_mark()
        if read_bool_property(targetable, "is_fly_marked"):
            fail("clear_fly_mark should remove Fly mark state")
        if "UNMARKED" not in targetable.get_fly_prompt():
            fail(f"Fly prompt did not reflect cleared state: {targetable.get_fly_prompt()}")

    finally:
        for actor in reversed(spawned):
            if actor:
                ACTOR_EDITOR.destroy_actor(actor)

    unreal.log("DockShield Fly recon smoke validation passed: sonar, mark, prompt, and clear states executed.")


main()
