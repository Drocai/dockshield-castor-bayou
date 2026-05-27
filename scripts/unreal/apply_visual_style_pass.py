import math

import unreal


MAP_PATH = "/Game/DockShield/Maps/M_Test_Targeting"
MATERIAL_DIR = "/Game/DockShield/Environment/Bayou/Materials"

LEVEL_EDITOR = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
ACTOR_EDITOR = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

RESET_VISUAL_PREFIX = "DS_Visual_"
RESET_LIGHTING_LABELS = {
    "DS_Light_Key_Storm",
    "DS_Light_Toxic_Hazard",
    "DS_Light_Rescue_Warm",
    "DS_Fog_Bayou_Test",
    "DS_Light_Moon_Key",
    "DS_Light_Toxic_Glow",
    "DS_Light_Rescue_Amber",
    "DS_Light_Depth_Neon",
    "DS_Fog_Bayou_Visual",
    "DS_PostProcess_StormGrade",
}

CUBE_MESH_PATHS = ["/Engine/BasicShapes/Cube.Cube", "/Game/LevelPrototyping/Meshes/SM_Cube.SM_Cube"]
PLANE_MESH_PATHS = ["/Engine/BasicShapes/Plane.Plane", "/Game/LevelPrototyping/Meshes/SM_Plane.SM_Plane"]
CYLINDER_MESH_PATHS = ["/Engine/BasicShapes/Cylinder.Cylinder"]


MATERIAL_SPECS = {
    "M_DS_WetDockWood_Proto": {
        "base": unreal.LinearColor(0.105, 0.067, 0.035, 1.0),
        "roughness": 0.24,
        "metallic": 0.0,
        "emissive": None,
    },
    "M_DS_SwampMud_Proto": {
        "base": unreal.LinearColor(0.052, 0.068, 0.043, 1.0),
        "roughness": 0.48,
        "metallic": 0.0,
        "emissive": None,
    },
    "M_DS_ToxicAlgaeWater_Proto": {
        "base": unreal.LinearColor(0.018, 0.17, 0.055, 1.0),
        "roughness": 0.12,
        "metallic": 0.0,
        "emissive": unreal.LinearColor(0.0, 0.72, 0.16, 1.0),
    },
    "M_DS_MossyReed_Proto": {
        "base": unreal.LinearColor(0.10, 0.22, 0.075, 1.0),
        "roughness": 0.65,
        "metallic": 0.0,
        "emissive": None,
    },
    "M_DS_CrackedNeonMetal_Proto": {
        "base": unreal.LinearColor(0.025, 0.045, 0.047, 1.0),
        "roughness": 0.18,
        "metallic": 0.72,
        "emissive": unreal.LinearColor(0.9, 0.06, 0.02, 1.0),
    },
    "M_DS_GrappleAnchor_Proto": {
        "base": unreal.LinearColor(0.72, 0.47, 0.12, 1.0),
        "roughness": 0.20,
        "metallic": 0.6,
        "emissive": unreal.LinearColor(0.36, 0.22, 0.02, 1.0),
    },
    "M_DS_CivilianMarker_Proto": {
        "base": unreal.LinearColor(0.68, 0.92, 0.72, 1.0),
        "roughness": 0.35,
        "metallic": 0.0,
        "emissive": unreal.LinearColor(0.08, 0.38, 0.13, 1.0),
    },
    "M_DS_RescueBoat_Proto": {
        "base": unreal.LinearColor(0.78, 0.12, 0.055, 1.0),
        "roughness": 0.27,
        "metallic": 0.18,
        "emissive": unreal.LinearColor(0.16, 0.02, 0.01, 1.0),
    },
    "M_DS_StormBackdrop_Proto": {
        "base": unreal.LinearColor(0.025, 0.035, 0.052, 1.0),
        "roughness": 0.62,
        "metallic": 0.0,
        "emissive": None,
    },
}


def fail(message):
    raise RuntimeError(f"DockShield visual style pass failed: {message}")


def get_class(name):
    cls = getattr(unreal, name, None)
    if cls is None:
        fail(f"Unreal Python class not available: {name}")
    return cls


def set_prop(obj, name, value):
    try:
        obj.set_editor_property(name, value)
    except Exception as exc:
        unreal.log_warning(f"Could not set {obj.get_name()}.{name}: {exc}")


def load_first(paths):
    for path in paths:
        asset = unreal.load_asset(path)
        if asset:
            return asset
    fail(f"none of these assets could be loaded: {paths}")


def material_property(name):
    prop = getattr(unreal.MaterialProperty, name, None)
    if prop is None:
        unreal.log_warning(f"MaterialProperty.{name} is unavailable; skipping that connection.")
    return prop


def connect_material_constant(material, expression_class, value_property, value, property_name, x, y):
    prop = material_property(property_name)
    if prop is None:
        return

    expr = unreal.MaterialEditingLibrary.create_material_expression(material, expression_class, x, y)
    expr.set_editor_property(value_property, value)
    unreal.MaterialEditingLibrary.connect_material_property(expr, "", prop)


def create_or_update_material(asset_name, spec):
    unreal.EditorAssetLibrary.make_directory(MATERIAL_DIR)
    asset_path = f"{MATERIAL_DIR}/{asset_name}"
    material = unreal.load_asset(asset_path) if unreal.EditorAssetLibrary.does_asset_exist(asset_path) else None
    if material is None:
        asset_tools = unreal.AssetToolsHelpers.get_asset_tools()
        material = asset_tools.create_asset(asset_name, MATERIAL_DIR, unreal.Material, unreal.MaterialFactoryNew())
        if material is None:
            fail(f"could not create material asset {asset_path}")

    material.modify()
    unreal.MaterialEditingLibrary.delete_all_material_expressions(material)
    connect_material_constant(
        material,
        unreal.MaterialExpressionConstant3Vector,
        "constant",
        spec["base"],
        "MP_BASE_COLOR",
        -520,
        -120,
    )
    connect_material_constant(
        material,
        unreal.MaterialExpressionConstant,
        "r",
        float(spec["roughness"]),
        "MP_ROUGHNESS",
        -520,
        60,
    )
    connect_material_constant(
        material,
        unreal.MaterialExpressionConstant,
        "r",
        float(spec["metallic"]),
        "MP_METALLIC",
        -520,
        190,
    )
    if spec["emissive"] is not None:
        connect_material_constant(
            material,
            unreal.MaterialExpressionConstant3Vector,
            "constant",
            spec["emissive"],
            "MP_EMISSIVE_COLOR",
            -520,
            330,
        )

    set_prop(material, "used_with_static_mesh", True)
    set_prop(material, "use_emissive_for_dynamic_area_lighting", spec["emissive"] is not None)
    unreal.MaterialEditingLibrary.recompile_material(material)
    unreal.EditorAssetLibrary.save_asset(asset_path)
    return material


def build_materials():
    return {
        name: create_or_update_material(name, spec)
        for name, spec in MATERIAL_SPECS.items()
    }


def destroy_existing_visual_pass():
    for actor in list(ACTOR_EDITOR.get_all_level_actors()):
        label = actor.get_actor_label()
        if label.startswith(RESET_VISUAL_PREFIX) or label in RESET_LIGHTING_LABELS:
            ACTOR_EDITOR.destroy_actor(actor)


def static_mesh_component(actor):
    return actor.get_component_by_class(unreal.StaticMeshComponent)


def set_actor_material(actor, material):
    component = static_mesh_component(actor)
    if not component or not material:
        return
    component.set_material(0, material)
    set_prop(component, "cast_shadow", True)


def actor_by_label(label):
    return next((actor for actor in ACTOR_EDITOR.get_all_level_actors() if actor.get_actor_label() == label), None)


def apply_existing_actor_materials(materials):
    assignments = {
        "Graybox_Floor": "M_DS_SwampMud_Proto",
        "Target_GrapplePull_Debug": "M_DS_GrappleAnchor_Proto",
        "Target_CivilianRescue_Debug": "M_DS_CivilianMarker_Proto",
        "Hazard_ToxicWater_Debug": "M_DS_ToxicAlgaeWater_Proto",
        "DS_Boat_Prototype_Tow": "M_DS_RescueBoat_Proto",
    }
    for label, material_name in assignments.items():
        actor = actor_by_label(label)
        if actor:
            set_actor_material(actor, materials[material_name])


def spawn_static(label, location, rotation, scale, mesh, material, folder):
    actor = ACTOR_EDITOR.spawn_actor_from_class(unreal.StaticMeshActor, location, rotation)
    if actor is None:
        fail(f"could not spawn {label}")
    actor.set_actor_label(label)
    actor.set_folder_path(folder)
    component = static_mesh_component(actor)
    if component:
        component.set_static_mesh(mesh)
        component.set_material(0, material)
        set_prop(component, "cast_shadow", True)
    actor.set_actor_scale3d(scale)
    return actor


def spawn_visual_dressing(materials):
    cube = load_first(CUBE_MESH_PATHS)
    plane = load_first(PLANE_MESH_PATHS)
    cylinder = load_first(CYLINDER_MESH_PATHS)

    spawn_static(
        "DS_Visual_Water_Surface",
        unreal.Vector(-150.0, -450.0, 42.0),
        unreal.Rotator(0.0, 0.0, 0.0),
        unreal.Vector(7.8, 5.4, 1.0),
        plane,
        materials["M_DS_ToxicAlgaeWater_Proto"],
        "DockShield/VisualPass/Water",
    )

    for index in range(10):
        x = -570.0 + (index * 126.0)
        y = -145.0 + ((index % 2) * 18.0)
        yaw = -2.0 + ((index % 3) * 2.0)
        spawn_static(
            f"DS_Visual_Dock_Plank_{index + 1:02d}",
            unreal.Vector(x, y, 9.0),
            unreal.Rotator(0.0, yaw, 0.0),
            unreal.Vector(1.22, 0.11, 0.035),
            cube,
            materials["M_DS_WetDockWood_Proto"],
            "DockShield/VisualPass/Dock",
        )

    for index in range(8):
        x = -700.0 + (index * 205.0)
        y = -690.0 + (45.0 * math.sin(index))
        yaw = (index * 19.0) % 360.0
        spawn_static(
            f"DS_Visual_Reed_{index + 1:02d}",
            unreal.Vector(x, y, 42.0),
            unreal.Rotator(0.0, yaw, 7.0),
            unreal.Vector(0.025, 0.025, 0.84 + (0.08 * (index % 3))),
            cylinder,
            materials["M_DS_MossyReed_Proto"],
            "DockShield/VisualPass/Reeds",
        )

    for index, x in enumerate([-540.0, 470.0]):
        spawn_static(
            f"DS_Visual_Backdrop_Shack_{index + 1:02d}",
            unreal.Vector(x, 610.0, 118.0),
            unreal.Rotator(0.0, -6.0 + (index * 12.0), 0.0),
            unreal.Vector(1.8, 0.22, 1.35),
            cube,
            materials["M_DS_StormBackdrop_Proto"],
            "DockShield/VisualPass/Backdrop",
        )

    spawn_static(
        "DS_Visual_Neon_Depth_Sign",
        unreal.Vector(250.0, 594.0, 214.0),
        unreal.Rotator(0.0, 180.0, 0.0),
        unreal.Vector(1.7, 0.05, 0.46),
        cube,
        materials["M_DS_CrackedNeonMetal_Proto"],
        "DockShield/VisualPass/Backdrop",
    )

    spawn_static(
        "DS_Visual_Toxic_Glow_Plate",
        unreal.Vector(650.0, 0.0, 24.0),
        unreal.Rotator(0.0, 0.0, 0.0),
        unreal.Vector(3.0, 3.0, 1.0),
        plane,
        materials["M_DS_ToxicAlgaeWater_Proto"],
        "DockShield/VisualPass/Water",
    )


def spawn_point_light(label, location, color, intensity, radius):
    light = ACTOR_EDITOR.spawn_actor_from_class(unreal.PointLight, location, unreal.Rotator(0.0, 0.0, 0.0))
    light.set_actor_label(label)
    light.set_folder_path("DockShield/VisualPass/Lighting")
    component = light.get_component_by_class(get_class("PointLightComponent"))
    if component:
        set_prop(component, "intensity", intensity)
        set_prop(component, "attenuation_radius", radius)
        set_prop(component, "light_color", color)
        set_prop(component, "cast_shadows", True)
    return light


def spawn_storm_lighting():
    moon = ACTOR_EDITOR.spawn_actor_from_class(
        unreal.DirectionalLight,
        unreal.Vector(-460.0, -630.0, 820.0),
        unreal.Rotator(-48.0, -35.0, 0.0),
    )
    moon.set_actor_label("DS_Light_Moon_Key")
    moon.set_folder_path("DockShield/VisualPass/Lighting")
    moon_component = moon.get_component_by_class(get_class("DirectionalLightComponent"))
    if moon_component:
        set_prop(moon_component, "intensity", 1.9)
        set_prop(moon_component, "light_color", unreal.Color(112, 154, 255, 255))
        set_prop(moon_component, "cast_shadows", True)

    spawn_point_light(
        "DS_Light_Toxic_Glow",
        unreal.Vector(650.0, 0.0, 130.0),
        unreal.Color(42, 255, 90, 255),
        8500.0,
        1020.0,
    )
    spawn_point_light(
        "DS_Light_Rescue_Amber",
        unreal.Vector(-135.0, -175.0, 260.0),
        unreal.Color(255, 132, 54, 255),
        4700.0,
        760.0,
    )
    spawn_point_light(
        "DS_Light_Depth_Neon",
        unreal.Vector(250.0, 550.0, 238.0),
        unreal.Color(255, 28, 16, 255),
        6100.0,
        720.0,
    )

    fog = ACTOR_EDITOR.spawn_actor_from_class(
        unreal.ExponentialHeightFog,
        unreal.Vector(0.0, 0.0, 0.0),
        unreal.Rotator(0.0, 0.0, 0.0),
    )
    fog.set_actor_label("DS_Fog_Bayou_Visual")
    fog.set_folder_path("DockShield/VisualPass/Lighting")
    fog_component = fog.get_component_by_class(get_class("ExponentialHeightFogComponent"))
    if fog_component:
        set_prop(fog_component, "fog_density", 0.052)
        set_prop(fog_component, "fog_height_falloff", 0.14)

    post = ACTOR_EDITOR.spawn_actor_from_class(
        unreal.PostProcessVolume,
        unreal.Vector(0.0, 0.0, 110.0),
        unreal.Rotator(0.0, 0.0, 0.0),
    )
    post.set_actor_label("DS_PostProcess_StormGrade")
    post.set_folder_path("DockShield/VisualPass/Lighting")
    set_prop(post, "enabled", True)
    set_prop(post, "unbound", True)
    set_prop(post, "blend_weight", 0.55)


def main():
    if not unreal.EditorAssetLibrary.does_asset_exist(MAP_PATH):
        fail(f"missing map asset {MAP_PATH}")

    LEVEL_EDITOR.load_level(MAP_PATH)
    materials = build_materials()
    destroy_existing_visual_pass()
    apply_existing_actor_materials(materials)
    spawn_visual_dressing(materials)
    spawn_storm_lighting()

    LEVEL_EDITOR.save_current_level()
    unreal.log("DockShield visual style pass applied: prototype materials, wet bayou dressing, storm lighting, toxic glow, and fog.")


main()
