import unreal


def fail(message):
    raise RuntimeError(f"DockShield meta systems validation failed: {message}")


def require_class(name):
    cls = getattr(unreal, name, None)
    if cls is None:
        fail(f"missing reflected class unreal.{name}")
    return cls


def require_method(obj, name):
    method = getattr(obj, name, None)
    if method is None or not callable(method):
        fail(f"{obj.get_name()} missing callable {name}()")
    return method


def validate_player_controller_meta():
    controller_cls = require_class("DSPrototypePlayerController")
    cdo = unreal.get_default_object(controller_cls)

    required_methods = [
        "get_beta_credits",
        "get_prototype_xp",
        "get_depth_samples",
        "get_achievement_unlock_count",
        "get_economy_status_text",
        "get_achievement_status_text",
        "get_settings_status_text",
        "get_visual_quality_label",
        "is_settings_panel_open",
        "get_hud_scale",
        "get_gamma",
        "get_boss_arena_status_text",
    ]
    for method_name in required_methods:
        require_method(cdo, method_name)

    if cdo.get_beta_credits() < 0:
        fail("default beta credits are negative")
    if cdo.get_hud_scale() <= 0.0:
        fail("default HUD scale is invalid")
    if cdo.get_gamma() <= 0.0:
        fail("default gamma is invalid")

    settings_text = str(cdo.get_settings_status_text())
    if "HUD" not in settings_text or "VISUAL" not in settings_text:
        fail(f"settings status text missing expected fragments: {settings_text}")

    economy_text = str(cdo.get_economy_status_text())
    if "CREDITS" not in economy_text or "XP" not in economy_text:
        fail(f"economy status text missing expected fragments: {economy_text}")


def validate_boss_arena_actor():
    arena_cls = require_class("DSDeepDockBossArenaActor")
    cdo = unreal.get_default_object(arena_cls)

    for method_name in [
        "get_arena_status_text",
        "get_boss_integrity",
        "get_lab_core_integrity",
        "get_threat_level",
        "is_arena_armed",
        "is_boss_defeated",
        "get_boss_phase_text",
        "get_resolved_weak_point_count",
        "get_combo_trigger_count",
        "get_hook_line_sinker_readiness",
        "evaluate_boss_weak_point_combos",
        "apply_hook_line_sinker_combo",
        "reset_boss_encounter",
    ]:
        require_method(cdo, method_name)

    status = str(cdo.get_arena_status_text())
    if "DEEP DOCK" not in status or "THREAT" not in status:
        fail(f"boss arena status text missing expected fragments: {status}")
    if cdo.get_boss_integrity() <= 0.0:
        fail("boss integrity should start above zero")
    if not cdo.is_arena_armed():
        fail("boss arena should start armed")
    if cdo.get_combo_trigger_count() != 0:
        fail("boss combo count should start at zero")
    if cdo.get_resolved_weak_point_count() != 0:
        fail("resolved weak point count should start at zero")


def main():
    validate_player_controller_meta()
    validate_boss_arena_actor()
    unreal.log("DockShield meta systems validation passed: economy, achievements, settings, and boss arena state.")


main()
