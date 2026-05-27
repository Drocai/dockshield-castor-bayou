import unreal


MAP_PATH = "/Game/DockShield/Maps/M_Test_Targeting"

LEVEL_EDITOR = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
ACTOR_EDITOR = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)


def fail(message):
    raise RuntimeError(f"DockShield character switching validation failed: {message}")


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


def validate_controller_methods(controller_cdo):
    for method_name in [
        "switch_to_reel",
        "switch_to_fly",
        "switch_to_lilly",
        "cycle_hero",
        "get_active_hero_label",
        "get_switch_status_text",
        "get_switch_count",
        "get_reel_pawn_actor",
        "get_fly_pawn_actor",
        "get_lilly_pawn_actor",
    ]:
        if not hasattr(controller_cdo, method_name):
            fail(f"DSPrototypePlayerController missing Python method {method_name}")


def validate_hero_visuals(hero, expected_profile_fragment):
    for method_name in [
        "get_prototype_visual_kit_component_count",
        "get_prototype_visual_profile",
    ]:
        if not hasattr(hero, method_name):
            fail(f"{hero.get_name()} missing Python method {method_name}")

    kit_count = hero.get_prototype_visual_kit_component_count()
    if kit_count < 4:
        fail(f"{hero.get_name()} expected at least four prototype visual kit components, got {kit_count}")

    visual_profile = hero.get_prototype_visual_profile()
    if expected_profile_fragment not in visual_profile:
        fail(f"{hero.get_name()} visual profile mismatch: {visual_profile}")


def main():
    if not unreal.EditorAssetLibrary.does_asset_exist(MAP_PATH):
        fail(f"missing map asset {MAP_PATH}")

    LEVEL_EDITOR.load_level(MAP_PATH)

    game_mode_cls = require_class("DSPrototypeGameMode")
    controller_cls = require_class("DSPrototypePlayerController")
    reel_cls = require_class("DSReelPrototypeCharacter")
    fly_cls = require_class("DSFlyPrototypeCharacter")
    lilly_cls = require_class("DSLillyPrototypeCharacter")

    game_mode_cdo = unreal.get_default_object(game_mode_cls)
    player_controller_class = require_property(game_mode_cdo, "player_controller_class")
    if "DSPrototypePlayerController" not in reflected_text(player_controller_class):
        fail(f"DSPrototypeGameMode player controller mismatch: {reflected_text(player_controller_class)}")

    controller_cdo = unreal.get_default_object(controller_cls)
    validate_controller_methods(controller_cdo)

    spawned = []
    try:
        reel = ACTOR_EDITOR.spawn_actor_from_class(
            reel_cls,
            unreal.Vector(-240.0, -700.0, 120.0),
            unreal.Rotator(0.0, 0.0, 0.0),
        )
        fly = ACTOR_EDITOR.spawn_actor_from_class(
            fly_cls,
            unreal.Vector(120.0, -700.0, 120.0),
            unreal.Rotator(0.0, 0.0, 0.0),
        )
        lilly = ACTOR_EDITOR.spawn_actor_from_class(
            lilly_cls,
            unreal.Vector(480.0, -700.0, 120.0),
            unreal.Rotator(0.0, 0.0, 0.0),
        )
        spawned.extend([reel, fly, lilly])

        if reel is None:
            fail("could not spawn DSReelPrototypeCharacter")
        if fly is None:
            fail("could not spawn DSFlyPrototypeCharacter")
        if lilly is None:
            fail("could not spawn DSLillyPrototypeCharacter")

        validate_hero_visuals(reel, "THE REEL")
        validate_hero_visuals(fly, "THE FLY")
        validate_hero_visuals(lilly, "LILLY LOCH")

    finally:
        for actor in reversed(spawned):
            if actor:
                ACTOR_EDITOR.destroy_actor(actor)

    unreal.log("DockShield character switching validation passed: controller class, switch methods, and Reel/Fly/Lilly visual kits are wired.")


main()
