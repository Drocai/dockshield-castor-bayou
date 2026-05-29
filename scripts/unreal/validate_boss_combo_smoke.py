import unreal


MAP_PATH = "/Game/DockShield/Maps/M_Test_Targeting"
WEAKPOINT_LABELS = [
    "DS_DeepDock_Boss_WeakPoint_Left",
    "DS_DeepDock_Boss_WeakPoint_Right",
    "DS_DeepDock_Boss_WeakPoint_Crown",
]

LEVEL_EDITOR = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
ACTOR_EDITOR = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)


def fail(message):
    raise RuntimeError(f"DockShield boss combo smoke failed: {message}")


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


def require_targetable(actor):
    targetable_cls = require_class("DSTargetableComponent")
    component = actor.get_component_by_class(targetable_cls)
    if component is None:
        fail(f"{actor.get_actor_label()} missing DSTargetableComponent")
    component.configure_from_owner_tags()
    return component


def main():
    if not unreal.EditorAssetLibrary.does_asset_exist(MAP_PATH):
        fail(f"missing map asset {MAP_PATH}")

    LEVEL_EDITOR.load_level(MAP_PATH)

    arena_cls = require_class("DSDeepDockBossArenaActor")
    arena = next((actor for actor in ACTOR_EDITOR.get_all_level_actors() if isinstance(actor, arena_cls)), None)
    if arena is None:
        fail("missing DSDeepDockBossArenaActor in test map")

    arena.reset_boss_encounter()
    if arena.get_combo_trigger_count() != 0:
        fail("combo count should be zero after reset")
    if arena.get_resolved_weak_point_count() != 0:
        fail("resolved weak point count should be zero after reset")

    previous_integrity = arena.get_boss_integrity()
    for index, label in enumerate(WEAKPOINT_LABELS, start=1):
        weakpoint = require_actor(label)
        targetable = require_targetable(weakpoint)

        if targetable.is_hook_line_sinker_ready():
            fail(f"{label} should not start combo-ready after reset")

        if not targetable.expose_for_reel(1.0):
            fail(f"{label} failed Reel exposure")
        if not targetable.mark_for_fly(1.0):
            fail(f"{label} failed Fly mark")
        if not targetable.bind_for_lilly(1.0):
            fail(f"{label} failed Lilly bind")
        if not targetable.is_hook_line_sinker_ready():
            fail(f"{label} did not become Hook, Line & Sinker ready")

        applied_count = arena.evaluate_boss_weak_point_combos()
        if applied_count != 1:
            fail(f"{label} expected exactly one applied combo, got {applied_count}")
        if arena.get_combo_trigger_count() != index:
            fail(f"expected combo count {index}, got {arena.get_combo_trigger_count()}")
        if arena.get_resolved_weak_point_count() != index:
            fail(f"expected resolved weak points {index}, got {arena.get_resolved_weak_point_count()}")
        if arena.get_boss_integrity() >= previous_integrity and not arena.is_boss_defeated():
            fail(f"{label} did not reduce boss integrity")

        previous_integrity = arena.get_boss_integrity()

    if not arena.is_boss_defeated():
        fail("boss should be defeated after all three weak-point combos")

    status = str(arena.get_arena_status_text())
    if "DEFEATED" not in status or "BOSS DOWN" not in status:
        fail(f"final arena status missing defeated state: {status}")

    unreal.log("DockShield boss combo smoke passed: Reel expose + Fly mark + Lilly bind triggers Hook, Line & Sinker damage.")


main()
