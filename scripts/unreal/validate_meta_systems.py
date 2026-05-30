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
        "get_mutation_status_text",
        "get_duct_status_text",
        "get_combat_feedback_status_text",
        "get_combat_feedback_flash",
        "cycle_weather_state",
        "get_weather_state_label",
        "get_weather_status_text",
        "get_weather_target_range_scale",
        "get_weather_visibility_scale",
        "get_weather_hazard_pressure_scale",
        "get_mission_objective_status_text",
        "get_mission_objective_progress",
        "get_extraction_status_text",
        "get_extraction_secured_count",
        "get_extraction_failure_count",
        "get_audio_cue_status_text",
        "get_audio_cue_event_count",
        "get_visual_cue_status_text",
        "get_visual_cue_event_count",
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

    duct_text = str(cdo.get_duct_status_text())
    if "DUCT" not in duct_text or "UNCAUGHT" not in duct_text:
        fail(f"duct status text missing expected fragments: {duct_text}")

    mutation_text = str(cdo.get_mutation_status_text())
    if "MUTATION" not in mutation_text:
        fail(f"mutation status text missing expected fragment: {mutation_text}")

    combat_feedback_text = str(cdo.get_combat_feedback_status_text())
    if "COMBAT FEEDBACK" not in combat_feedback_text:
        fail(f"combat feedback status text missing expected fragment: {combat_feedback_text}")

    weather_text = str(cdo.get_weather_status_text())
    if "WEATHER" not in weather_text or "VIS" not in weather_text or "RANGE" not in weather_text:
        fail(f"weather status text missing expected fragments: {weather_text}")
    if not 0.1 <= cdo.get_weather_target_range_scale() <= 1.5:
        fail("weather target range scale is outside expected prototype bounds")
    if not 0.1 <= cdo.get_weather_visibility_scale() <= 1.5:
        fail("weather visibility scale is outside expected prototype bounds")
    if cdo.get_weather_hazard_pressure_scale() <= 0.0:
        fail("weather hazard pressure scale is invalid")

    mission_text = str(cdo.get_mission_objective_status_text())
    if "OBJECTIVE" not in mission_text or "PROGRESS" not in mission_text:
        fail(f"mission objective text missing expected fragments: {mission_text}")
    if not 0.0 <= cdo.get_mission_objective_progress() <= 1.0:
        fail("mission objective progress is outside 0..1")

    extraction_text = str(cdo.get_extraction_status_text())
    if "EXTRACTION" not in extraction_text or "SECURED" not in extraction_text or "FAILED" not in extraction_text:
        fail(f"extraction status text missing expected fragments: {extraction_text}")
    if cdo.get_extraction_secured_count() != 0 or cdo.get_extraction_failure_count() != 0:
        fail("default extraction counters should start at zero")

    audio_text = str(cdo.get_audio_cue_status_text())
    if "SFX" not in audio_text or "EVENTS" not in audio_text:
        fail(f"audio cue status text missing expected fragments: {audio_text}")
    visual_text = str(cdo.get_visual_cue_status_text())
    if "VFX" not in visual_text or "EVENTS" not in visual_text:
        fail(f"visual cue status text missing expected fragments: {visual_text}")


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


def validate_mutation_enemy_actor():
    mutation_cls = require_class("DSMutationEnemyActor")
    cdo = unreal.get_default_object(mutation_cls)

    for method_name in [
        "get_mutation_status_text",
        "get_health",
        "get_aggression",
        "get_vulnerability",
        "get_combo_impact_count",
        "apply_reel_impact",
        "apply_fly_pressure",
        "apply_lilly_pressure",
        "evaluate_hero_combo",
        "reset_mutation_encounter",
    ]:
        require_method(cdo, method_name)

    status = str(cdo.get_mutation_status_text())
    if "BAYOU MUTATION" not in status or "HEALTH" not in status:
        fail(f"mutation status text missing expected fragments: {status}")
    if cdo.get_health() <= 0.0:
        fail("mutation CDO should start alive")


def main():
    validate_player_controller_meta()
    validate_boss_arena_actor()
    validate_mutation_enemy_actor()
    unreal.log("DockShield meta systems validation passed: economy, achievements, settings, weather, objective, extraction, audio/visual hooks, boss arena state, and mutation enemy status.")


main()
