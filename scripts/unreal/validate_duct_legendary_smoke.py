import unreal


MAP_PATH = "/Game/DockShield/Maps/M_Test_Targeting"
DUCT_LABEL = "DS_Duct_Legendary_Sighting_Debug"
REQUIRED_LABELS = [
    DUCT_LABEL,
    "DS_Duct_Tape_Trace_01",
    "DS_Duct_Wake_Ring",
    "DS_Duct_Legend_Glow",
]

LEVEL_EDITOR = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
ACTOR_EDITOR = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)


def fail(message):
    raise RuntimeError(f"DockShield Duct legendary smoke failed: {message}")


def require_class(name):
    cls = getattr(unreal, name, None)
    if cls is None:
        fail(f"missing reflected class unreal.{name}")
    return cls


def actor_by_label(label):
    return next((actor for actor in ACTOR_EDITOR.get_all_level_actors() if actor.get_actor_label() == label), None)


def require_actor(label):
    actor = actor_by_label(label)
    if actor is None:
        fail(f"missing actor {label}")
    return actor


def require_method(obj, name):
    method = getattr(obj, name, None)
    if method is None or not callable(method):
        fail(f"{obj.get_name()} missing callable {name}()")
    return method


def validate_duct_cdo():
    duct_cls = require_class("DSDuctLegendaryEncounterActor")
    cdo = unreal.get_default_object(duct_cls)
    for method_name in [
        "begin_sighting",
        "apply_bait",
        "latch_with_reel",
        "advance_reel_battle",
        "resolve_slip_away",
        "reset_duct_encounter",
        "get_duct_status_text",
        "get_encounter_state_text",
        "is_catchable",
        "has_slipped_away",
        "get_encounter_progress",
        "get_best_near_catch_progress",
        "get_sighting_count",
        "get_attempt_count",
        "get_near_catch_count",
        "get_duct_tape_trace_count",
    ]:
        require_method(cdo, method_name)

    if cdo.is_catchable():
        fail("Duct CDO must not be catchable in this lore version")


def validate_duct_map_actor():
    duct_cls = require_class("DSDuctLegendaryEncounterActor")
    targetable_cls = require_class("DSTargetableComponent")

    for label in REQUIRED_LABELS:
        require_actor(label)

    duct = require_actor(DUCT_LABEL)
    if not isinstance(duct, duct_cls):
        fail(f"{DUCT_LABEL} is not DSDuctLegendaryEncounterActor")

    targetable = duct.get_component_by_class(targetable_cls)
    if targetable is None:
        fail(f"{DUCT_LABEL} missing DSTargetableComponent")
    targetable.configure_from_owner_tags()
    prompt = str(targetable.get_reel_prompt())
    if "DUCT" not in prompt or "impossible" not in prompt:
        fail(f"Duct Reel prompt missing lore guardrail: {prompt}")

    duct.reset_duct_encounter()
    if duct.is_catchable():
        fail("Duct actor must not become catchable after reset")
    if not duct.begin_sighting():
        fail("Duct begin_sighting returned false")
    if duct.get_sighting_count() != 1:
        fail(f"expected one sighting, got {duct.get_sighting_count()}")
    if not duct.apply_bait(unreal.Name("BobberBouncer")):
        fail("Duct apply_bait returned false")
    if not duct.latch_with_reel(0.4):
        fail("Duct latch_with_reel returned false")

    for _ in range(36):
        if not duct.advance_reel_battle(0.25, 0.72):
            break

    if not duct.has_slipped_away():
        fail(f"Duct should slip away after sustained reel battle, state={duct.get_encounter_state_text()}")
    if duct.get_near_catch_count() < 1:
        fail("Duct near-catch count did not increment")
    if duct.get_duct_tape_trace_count() < 1:
        fail("Duct tape trace evidence did not increment")
    if duct.get_best_near_catch_progress() < 0.86:
        fail("Duct best near-catch progress did not reach threshold")

    status = str(duct.get_duct_status_text())
    if "UNCAUGHT" not in status or "SLIPPED" not in status:
        fail(f"Duct status missing uncatchable outcome: {status}")


def main():
    if not unreal.EditorAssetLibrary.does_asset_exist(MAP_PATH):
        fail(f"missing map asset {MAP_PATH}")

    LEVEL_EDITOR.load_level(MAP_PATH)
    validate_duct_cdo()
    validate_duct_map_actor()
    unreal.log("DockShield Duct legendary smoke passed: sighting, bait, latch, near-catch, and forced slip-away remain uncatchable.")


main()
