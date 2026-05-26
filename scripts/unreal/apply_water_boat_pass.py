import unreal


MAP_PATH = "/Game/DockShield/Maps/M_Test_Targeting"
RESET_ACTOR_LABELS = {
    "DS_Water_Depth_Test",
    "DS_Boat_Prototype_Tow",
}

LEVEL_EDITOR = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
ACTOR_EDITOR = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)


def fail(message):
    raise RuntimeError(f"DockShield water/boat map pass failed: {message}")


def require_class(name):
    cls = getattr(unreal, name, None)
    if cls is None:
        fail(f"missing reflected class unreal.{name}")
    return cls


def destroy_existing():
    for actor in ACTOR_EDITOR.get_all_level_actors():
        if actor.get_actor_label() in RESET_ACTOR_LABELS:
            ACTOR_EDITOR.destroy_actor(actor)


def main():
    if not unreal.EditorAssetLibrary.does_asset_exist(MAP_PATH):
        fail(f"missing map asset {MAP_PATH}")

    water_cls = require_class("DSWaterZoneActor")
    boat_cls = require_class("DSPrototypeBoatActor")

    LEVEL_EDITOR.load_level(MAP_PATH)
    destroy_existing()

    water = ACTOR_EDITOR.spawn_actor_from_class(
        water_cls,
        unreal.Vector(-150.0, -450.0, -70.0),
        unreal.Rotator(0.0, 0.0, 0.0),
    )
    if water is None:
        fail("could not spawn DSWaterZoneActor")
    water.set_actor_label("DS_Water_Depth_Test")
    water.set_folder_path("DockShield/Water")
    water.set_editor_property("water_surface_z", 40.0)
    water.set_editor_property("boat_operational_depth", 120.0)
    water.set_editor_property("current_speed", 70.0)
    water_bounds = water.get_component_by_class(unreal.BoxComponent)
    if water_bounds:
        water_bounds.set_box_extent(unreal.Vector(780.0, 540.0, 190.0), True)

    boat = ACTOR_EDITOR.spawn_actor_from_class(
        boat_cls,
        unreal.Vector(-150.0, -450.0, -105.0),
        unreal.Rotator(0.0, 0.0, 0.0),
    )
    if boat is None:
        fail("could not spawn DSPrototypeBoatActor")
    boat.set_actor_label("DS_Boat_Prototype_Tow")
    boat.set_folder_path("DockShield/Water")
    boat.set_editor_property("current_water_depth", 145.0)
    boat.set_editor_property("required_water_depth", 120.0)
    boat.set_editor_property("reel_tow_step_distance", 240.0)

    LEVEL_EDITOR.save_current_level()
    unreal.log("DockShield water/boat map pass applied: DS_Water_Depth_Test and DS_Boat_Prototype_Tow.")


main()
