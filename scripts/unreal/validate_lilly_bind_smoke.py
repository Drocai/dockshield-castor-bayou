import unreal


MAP_PATH = "/Game/DockShield/Maps/M_Test_Targeting"

LEVEL_EDITOR = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
ACTOR_EDITOR = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)


def fail(message):
    raise RuntimeError(f"DockShield Lilly bind validation failed: {message}")


def require_class(name):
    cls = getattr(unreal, name, None)
    if cls is None:
        fail(f"missing reflected class unreal.{name}")
    return cls


def require_method(obj, method_name):
    if not hasattr(obj, method_name):
        fail(f"{obj.get_name()} missing Python method {method_name}")


def get_actor(label):
    actor = next(
        (item for item in ACTOR_EDITOR.get_all_level_actors() if item.get_actor_label() == label),
        None,
    )
    if actor is None:
        fail(f"missing actor {label}")
    return actor


def get_bool_property(obj, property_name):
    try:
        return bool(obj.get_editor_property(property_name))
    except Exception as exc:
        fail(f"{obj.get_name()} missing bool property {property_name}: {exc}")


def main():
    if not unreal.EditorAssetLibrary.does_asset_exist(MAP_PATH):
        fail(f"missing map asset {MAP_PATH}")

    LEVEL_EDITOR.load_level(MAP_PATH)

    lilly_cls = require_class("DSLillyPrototypeCharacter")
    targetable_cls = require_class("DSTargetableComponent")

    hazard = get_actor("Hazard_ToxicWater_Debug")
    targetable = hazard.get_component_by_class(targetable_cls)
    if targetable is None:
        fail("Hazard_ToxicWater_Debug missing DSTargetableComponent")

    targetable.configure_from_owner_tags()
    if not get_bool_property(targetable, "can_lilly_bind"):
        fail("Hazard_ToxicWater_Debug should be Lilly-bindable")

    hazard_location = hazard.get_actor_location()
    spawn_location = unreal.Vector(hazard_location.x - 700.0, hazard_location.y, hazard_location.z + 100.0)
    spawned = []
    try:
        lilly = ACTOR_EDITOR.spawn_actor_from_class(
            lilly_cls,
            spawn_location,
            unreal.Rotator(0.0, 0.0, 0.0),
        )
        if lilly is None:
            fail("could not spawn DSLillyPrototypeCharacter")
        spawned.append(lilly)

        for method_name in [
            "swamp_pulse",
            "execute_lilly_bind_on_target",
            "try_lilly_bind",
            "get_last_swamp_pulse_hit_count",
            "get_bound_target_count",
            "get_last_bind_result",
            "get_bind_range",
            "get_root_radius",
            "get_prototype_visual_kit_component_count",
            "get_prototype_visual_profile",
        ]:
            require_method(lilly, method_name)

        kit_count = lilly.get_prototype_visual_kit_component_count()
        if kit_count < 4:
            fail(f"Lilly expected at least four prototype visual kit components, got {kit_count}")

        if "LILLY LOCH" not in lilly.get_prototype_visual_profile():
            fail(f"Lilly visual profile mismatch: {lilly.get_prototype_visual_profile()}")

        pulse_hits = lilly.swamp_pulse()
        if pulse_hits < 1:
            fail(f"expected swamp pulse to detect at least one bindable target, got {pulse_hits}")

        if not lilly.execute_lilly_bind_on_target(hazard):
            fail("execute_lilly_bind_on_target returned false for hazard")

        if not get_bool_property(targetable, "is_lilly_bound"):
            fail("targetable did not store Lilly bound state")

        if lilly.get_bound_target_count() != 1:
            fail(f"expected one unique bound target, got {lilly.get_bound_target_count()}")

        prompt = targetable.get_lilly_prompt()
        if "BOUND" not in prompt:
            fail(f"expected Lilly prompt to report BOUND, got: {prompt}")

        if not lilly.execute_lilly_bind_on_target(hazard):
            fail("rebinding already-bound hazard returned false")

        if lilly.get_bound_target_count() != 1:
            fail(f"rebinding should not inflate unique bound count, got {lilly.get_bound_target_count()}")

        targetable.clear_lilly_bind()
        if get_bool_property(targetable, "is_lilly_bound"):
            fail("clear_lilly_bind did not clear bound state")

        prompt = targetable.get_lilly_prompt()
        if "UNBOUND" not in prompt:
            fail(f"expected Lilly prompt to report UNBOUND after clear, got: {prompt}")

    finally:
        for actor in reversed(spawned):
            if actor:
                ACTOR_EDITOR.destroy_actor(actor)

    unreal.log("DockShield Lilly bind validation passed: pulse, bind state, prompt, and visual kit are wired.")


main()
