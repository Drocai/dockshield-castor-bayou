import unreal


MAP_PATH = "/Game/DockShield/Maps/M_Test_Targeting"
MUTATION_LABEL = "DS_MutationEnemy_BayouBruiser_Debug"
REQUIRED_LABELS = [
    MUTATION_LABEL,
    "DS_MutationEnemy_CoreGlow",
    "DS_MutationEnemy_WarningMark",
    "DS_MutationEnemy_ToxicWake",
    "DS_MutationEnemy_ToxicGlow",
    "DS_MutationEnemy_WarningLight",
]

LEVEL_EDITOR = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
ACTOR_EDITOR = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)


def fail(message):
    raise RuntimeError(f"DockShield mutation enemy smoke failed: {message}")


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


def call_or_value(obj, name):
    value = getattr(obj, name, None)
    if value is None:
        fail(f"{obj.get_name()} missing reflected attribute {name}")
    return value() if callable(value) else value


def validate_mutation_cdo():
    mutation_cls = require_class("DSMutationEnemyActor")
    cdo = unreal.get_default_object(mutation_cls)
    for method_name in [
        "apply_reel_impact",
        "apply_fly_pressure",
        "apply_lilly_pressure",
        "evaluate_hero_combo",
        "reset_mutation_encounter",
        "get_mutation_status_text",
        "get_threat_state_text",
        "get_health",
        "get_aggression",
        "get_vulnerability",
        "get_reel_impact_count",
        "get_fly_pressure_count",
        "get_lilly_pressure_count",
        "get_combo_impact_count",
        "is_defeated",
    ]:
        require_method(cdo, method_name)


def validate_mutation_map_actor():
    mutation_cls = require_class("DSMutationEnemyActor")
    targetable_cls = require_class("DSTargetableComponent")

    for label in REQUIRED_LABELS:
        require_actor(label)

    mutation = require_actor(MUTATION_LABEL)
    if not isinstance(mutation, mutation_cls):
        fail(f"{MUTATION_LABEL} is not DSMutationEnemyActor")

    targetable = mutation.get_component_by_class(targetable_cls)
    if targetable is None:
        fail(f"{MUTATION_LABEL} missing DSTargetableComponent")

    targetable.configure_from_owner_tags()
    if not call_or_value(targetable, "can_reel_pull"):
        fail("mutation enemy must support Reel pull/stagger")
    if not call_or_value(targetable, "can_fly_mark"):
        fail("mutation enemy must support Fly mark")
    if not call_or_value(targetable, "can_lilly_bind"):
        fail("mutation enemy must support Lilly bind")

    prompt = str(targetable.get_reel_prompt())
    if "Reel" not in prompt or "stagger" not in prompt:
        fail(f"mutation Reel prompt missing action language: {prompt}")

    mutation.reset_mutation_encounter()
    start_health = mutation.get_health()
    if start_health < 0.99:
        fail(f"mutation did not reset to full health: {start_health}")

    if not mutation.apply_reel_impact(1.0):
        fail("apply_reel_impact returned false")
    if mutation.get_reel_impact_count() != 1:
        fail("Reel impact count did not increment")
    if mutation.get_health() >= start_health:
        fail("Reel impact did not reduce health")

    if not mutation.apply_fly_pressure(1.0):
        fail("apply_fly_pressure returned false")
    if mutation.get_fly_pressure_count() != 1:
        fail("Fly pressure count did not increment")

    if not mutation.apply_lilly_pressure(1.0):
        fail("apply_lilly_pressure returned false")
    if mutation.get_lilly_pressure_count() != 1:
        fail("Lilly pressure count did not increment")
    if mutation.get_combo_impact_count() < 1:
        fail("Hook, Line & Sinker combo did not trigger after all three hero actions")

    for _ in range(3):
        if mutation.is_defeated():
            break
        mutation.apply_reel_impact(1.0)
        mutation.apply_fly_pressure(1.0)
        mutation.apply_lilly_pressure(1.0)

    if not mutation.is_defeated():
        fail(f"mutation should be defeated after repeated hero combo cycles: {mutation.get_mutation_status_text()}")

    status = str(mutation.get_mutation_status_text())
    if "DEFEATED" not in status or "COMBO" not in status:
        fail(f"mutation status missing defeat/combo state: {status}")


def main():
    if not unreal.EditorAssetLibrary.does_asset_exist(MAP_PATH):
        fail(f"missing map asset {MAP_PATH}")

    LEVEL_EDITOR.load_level(MAP_PATH)
    validate_mutation_cdo()
    validate_mutation_map_actor()
    unreal.log("DockShield mutation enemy smoke passed: Reel stagger, Fly mark, Lilly bind, combo damage, and defeat all work headlessly.")


main()
