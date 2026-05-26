import unreal


MAP_PATH = "/Game/DockShield/Maps/M_Test_Targeting"
LEVEL_EDITOR = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
ACTOR_EDITOR = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

RESET_LIGHTING_LABELS = {
    "DS_Light_Key_Storm",
    "DS_Light_Toxic_Hazard",
    "DS_Light_Rescue_Warm",
    "DS_Fog_Bayou_Test",
}


def get_class(name):
    cls = getattr(unreal, name, None)
    if cls is None:
        raise RuntimeError(f"Unreal Python class not available: {name}")
    return cls


def set_prop(obj, name, value):
    try:
        obj.set_editor_property(name, value)
    except Exception as exc:
        unreal.log_warning(f"Could not set {obj.get_name()}.{name}: {exc}")


def destroy_existing():
    for actor in ACTOR_EDITOR.get_all_level_actors():
        if actor.get_actor_label() in RESET_LIGHTING_LABELS:
            ACTOR_EDITOR.destroy_actor(actor)


def spawn_point_light(label, location, color, intensity, radius):
    light = ACTOR_EDITOR.spawn_actor_from_class(
        get_class("PointLight"),
        location,
        unreal.Rotator(0.0, 0.0, 0.0),
    )
    light.set_actor_label(label)
    light.set_folder_path("DockShield/Lighting")
    component = light.get_component_by_class(get_class("PointLightComponent"))
    if component:
        set_prop(component, "intensity", intensity)
        set_prop(component, "attenuation_radius", radius)
        set_prop(component, "light_color", color)
    return light


def spawn_fog():
    fog = ACTOR_EDITOR.spawn_actor_from_class(
        get_class("ExponentialHeightFog"),
        unreal.Vector(0.0, 0.0, 0.0),
        unreal.Rotator(0.0, 0.0, 0.0),
    )
    fog.set_actor_label("DS_Fog_Bayou_Test")
    fog.set_folder_path("DockShield/Lighting")
    component = fog.get_component_by_class(get_class("ExponentialHeightFogComponent"))
    if component:
        set_prop(component, "fog_density", 0.035)
        set_prop(component, "fog_height_falloff", 0.18)
    return fog


def main():
    if not unreal.EditorAssetLibrary.does_asset_exist(MAP_PATH):
        raise RuntimeError(f"Missing map asset {MAP_PATH}")

    LEVEL_EDITOR.load_level(MAP_PATH)
    destroy_existing()

    spawn_point_light(
        "DS_Light_Key_Storm",
        unreal.Vector(-180.0, -420.0, 520.0),
        unreal.Color(125, 170, 255, 255),
        9000.0,
        1600.0,
    )
    spawn_point_light(
        "DS_Light_Toxic_Hazard",
        unreal.Vector(650.0, 0.0, 145.0),
        unreal.Color(70, 255, 95, 255),
        5500.0,
        900.0,
    )
    spawn_point_light(
        "DS_Light_Rescue_Warm",
        unreal.Vector(230.0, 250.0, 260.0),
        unreal.Color(255, 155, 80, 255),
        3500.0,
        700.0,
    )
    spawn_fog()

    LEVEL_EDITOR.save_current_level()
    unreal.log("DockShield targeting test lighting pass applied.")


main()
