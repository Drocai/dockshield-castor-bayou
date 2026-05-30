import unreal


MAP_PATH = "/Game/DockShield/Maps/M_Test_Targeting"
MATERIAL_DIR = "/Game/DockShield/Environment/Bayou/Materials"

RESET_ACTOR_LABELS = {
    "DS_Boat_Fly_Blackwater",
    "DS_Boat_Lilly_Swamp",
    "DS_Gadget_Reel_Gauntlet_Debug",
    "DS_Gadget_Fly_SonarProbe_Debug",
    "DS_Gadget_Lilly_VineReel_Debug",
}

THEME_MATERIALS = {
    "M_DS_ReelTheme_Proto": {
        "base": unreal.LinearColor(0.82, 0.05, 0.035, 1.0),
        "roughness": 0.32,
        "metallic": 0.42,
        "emissive": unreal.LinearColor(0.22, 0.08, 0.0, 1.0),
    },
    "M_DS_FlyTheme_Proto": {
        "base": unreal.LinearColor(0.006, 0.018, 0.018, 1.0),
        "roughness": 0.24,
        "metallic": 0.62,
        "emissive": unreal.LinearColor(0.0, 0.38, 0.34, 1.0),
    },
    "M_DS_LillyTheme_Proto": {
        "base": unreal.LinearColor(0.12, 0.18, 0.055, 1.0),
        "roughness": 0.46,
        "metallic": 0.12,
        "emissive": unreal.LinearColor(0.32, 0.72, 0.06, 1.0),
    },
}

BOAT_SPECS = [
    {
        "label": "DS_Boat_Prototype_Tow",
        "theme": "Reel",
        "location": unreal.Vector(-150.0, -450.0, -105.0),
        "rotation": unreal.Rotator(0.0, 0.0, 0.0),
        "material": "M_DS_ReelTheme_Proto",
    },
    {
        "label": "DS_Boat_Fly_Blackwater",
        "theme": "Fly",
        "location": unreal.Vector(170.0, -560.0, -105.0),
        "rotation": unreal.Rotator(0.0, -12.0, 0.0),
        "material": "M_DS_FlyTheme_Proto",
    },
    {
        "label": "DS_Boat_Lilly_Swamp",
        "theme": "Lilly",
        "location": unreal.Vector(-470.0, -560.0, -105.0),
        "rotation": unreal.Rotator(0.0, 12.0, 0.0),
        "material": "M_DS_LillyTheme_Proto",
    },
]

GADGET_SPECS = [
    {
        "label": "DS_Gadget_Reel_Gauntlet_Debug",
        "location": unreal.Vector(-150.0, -255.0, 38.0),
        "rotation": unreal.Rotator(0.0, 0.0, 90.0),
        "scale": unreal.Vector(0.13, 0.13, 0.70),
        "mesh": "/Engine/BasicShapes/Cylinder.Cylinder",
        "material": "M_DS_ReelTheme_Proto",
        "tags": ["DockShieldGadget", "ReelGear", "ReelGauntlet"],
    },
    {
        "label": "DS_Gadget_Fly_SonarProbe_Debug",
        "location": unreal.Vector(170.0, -345.0, 42.0),
        "rotation": unreal.Rotator(0.0, -12.0, 90.0),
        "scale": unreal.Vector(0.085, 0.085, 0.86),
        "mesh": "/Engine/BasicShapes/Cylinder.Cylinder",
        "material": "M_DS_FlyTheme_Proto",
        "tags": ["DockShieldGadget", "FlyGear", "SonarProbe"],
    },
    {
        "label": "DS_Gadget_Lilly_VineReel_Debug",
        "location": unreal.Vector(-470.0, -345.0, 42.0),
        "rotation": unreal.Rotator(0.0, 12.0, 90.0),
        "scale": unreal.Vector(0.10, 0.10, 0.78),
        "mesh": "/Engine/BasicShapes/Cylinder.Cylinder",
        "material": "M_DS_LillyTheme_Proto",
        "tags": ["DockShieldGadget", "LillyGear", "VineReel"],
    },
]

LEVEL_EDITOR = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
ACTOR_EDITOR = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)


def fail(message):
    raise RuntimeError(f"DockShield hero boat theme pass failed: {message}")


def require_class(name):
    cls = getattr(unreal, name, None)
    if cls is None:
        fail(f"missing reflected class unreal.{name}")
    return cls


def set_prop(obj, name, value):
    try:
        obj.set_editor_property(name, value)
    except Exception as exc:
        unreal.log_warning(f"Could not set {obj.get_name()}.{name}: {exc}")


def material_property(name):
    prop = getattr(unreal.MaterialProperty, name, None)
    if prop is None:
        unreal.log_warning(f"MaterialProperty.{name} is unavailable; skipping.")
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
    set_prop(material, "use_emissive_for_dynamic_area_lighting", True)
    unreal.MaterialEditingLibrary.recompile_material(material)
    unreal.EditorAssetLibrary.save_asset(asset_path)
    return material


def build_materials():
    return {
        name: create_or_update_material(name, spec)
        for name, spec in THEME_MATERIALS.items()
    }


def actor_by_label(label):
    return next((actor for actor in ACTOR_EDITOR.get_all_level_actors() if actor.get_actor_label() == label), None)


def destroy_existing():
    for actor in list(ACTOR_EDITOR.get_all_level_actors()):
        if actor.get_actor_label() in RESET_ACTOR_LABELS:
            ACTOR_EDITOR.destroy_actor(actor)


def static_mesh_component(actor):
    return actor.get_component_by_class(unreal.StaticMeshComponent) if actor else None


def configure_boat(boat, spec, materials):
    boat.set_actor_label(spec["label"])
    boat.set_folder_path("DockShield/Water/HeroBoats")
    boat.set_actor_location(spec["location"], False, False)
    boat.set_actor_rotation(spec["rotation"], False)
    boat.set_editor_property("current_water_depth", 145.0)
    boat.set_editor_property("required_water_depth", 120.0)
    boat.set_editor_property("reel_tow_step_distance", 240.0)
    boat.set_editor_property("current_drift_scale", 0.42)
    boat.set_editor_property("flood_drag_scale", 0.35)
    mesh_component = static_mesh_component(boat)
    if mesh_component:
        mesh_component.set_material(0, materials[spec["material"]])
    boat.set_boat_theme_by_name(unreal.Name(spec["theme"]))


def spawn_or_update_boats(materials):
    boat_cls = require_class("DSPrototypeBoatActor")
    for spec in BOAT_SPECS:
        boat = actor_by_label(spec["label"])
        if boat is None:
            boat = ACTOR_EDITOR.spawn_actor_from_class(boat_cls, spec["location"], spec["rotation"])
        if boat is None:
            fail(f"could not spawn {spec['label']}")
        configure_boat(boat, spec, materials)


def spawn_gadget_marker(spec, materials):
    mesh = unreal.load_asset(spec["mesh"])
    if mesh is None:
        fail(f"could not load mesh {spec['mesh']}")

    actor = ACTOR_EDITOR.spawn_actor_from_class(unreal.StaticMeshActor, spec["location"], spec["rotation"])
    if actor is None:
        fail(f"could not spawn {spec['label']}")
    actor.set_actor_label(spec["label"])
    actor.set_folder_path("DockShield/Water/HeroGadgets")
    actor.set_actor_scale3d(spec["scale"])
    actor.tags = [unreal.Name(tag) for tag in spec["tags"]]

    component = static_mesh_component(actor)
    if component:
        component.set_static_mesh(mesh)
        component.set_material(0, materials[spec["material"]])
        set_prop(component, "cast_shadow", True)
        set_prop(component, "render_custom_depth", True)


def spawn_gadget_markers(materials):
    for spec in GADGET_SPECS:
        spawn_gadget_marker(spec, materials)


def main():
    if not unreal.EditorAssetLibrary.does_asset_exist(MAP_PATH):
        fail(f"missing map asset {MAP_PATH}")

    LEVEL_EDITOR.load_level(MAP_PATH)
    materials = build_materials()
    destroy_existing()
    spawn_or_update_boats(materials)
    spawn_gadget_markers(materials)

    LEVEL_EDITOR.save_current_level()
    unreal.log("DockShield hero boat theme pass applied: Reel/Fly/Lilly boats and themed gadget markers.")


main()
