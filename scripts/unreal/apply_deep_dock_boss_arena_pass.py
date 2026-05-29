import math

import unreal


MAP_PATH = "/Game/DockShield/Maps/M_Test_Targeting"
MATERIAL_DIR = "/Game/DockShield/Environment/DeepDock/Materials"

LEVEL_EDITOR = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
ACTOR_EDITOR = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

RESET_PREFIX = "DS_DeepDock_"

CUBE_MESH_PATHS = ["/Engine/BasicShapes/Cube.Cube", "/Game/LevelPrototyping/Meshes/SM_Cube.SM_Cube"]
PLANE_MESH_PATHS = ["/Engine/BasicShapes/Plane.Plane", "/Game/LevelPrototyping/Meshes/SM_Plane.SM_Plane"]
SPHERE_MESH_PATHS = ["/Engine/BasicShapes/Sphere.Sphere"]
CYLINDER_MESH_PATHS = ["/Engine/BasicShapes/Cylinder.Cylinder"]

MATERIAL_SPECS = {
    "M_DS_DeepDockWetDeck_Proto": {
        "base": unreal.LinearColor(0.025, 0.035, 0.035, 1.0),
        "roughness": 0.20,
        "metallic": 0.18,
        "emissive": None,
    },
    "M_DS_DeepDockWarningRed_Proto": {
        "base": unreal.LinearColor(0.38, 0.025, 0.018, 1.0),
        "roughness": 0.16,
        "metallic": 0.35,
        "emissive": unreal.LinearColor(0.95, 0.025, 0.01, 1.0),
    },
    "M_DS_DeepDockBioCore_Proto": {
        "base": unreal.LinearColor(0.018, 0.18, 0.06, 1.0),
        "roughness": 0.08,
        "metallic": 0.0,
        "emissive": unreal.LinearColor(0.0, 0.95, 0.24, 1.0),
    },
    "M_DS_DeepDockWeakPoint_Proto": {
        "base": unreal.LinearColor(0.78, 0.42, 0.08, 1.0),
        "roughness": 0.18,
        "metallic": 0.45,
        "emissive": unreal.LinearColor(1.0, 0.38, 0.02, 1.0),
    },
    "M_DS_DeepDockBlackWater_Proto": {
        "base": unreal.LinearColor(0.01, 0.028, 0.032, 1.0),
        "roughness": 0.04,
        "metallic": 0.0,
        "emissive": unreal.LinearColor(0.0, 0.08, 0.10, 1.0),
    },
}


def fail(message):
    raise RuntimeError(f"DockShield Deep Dock boss arena pass failed: {message}")


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
        material = unreal.AssetToolsHelpers.get_asset_tools().create_asset(
            asset_name,
            MATERIAL_DIR,
            unreal.Material,
            unreal.MaterialFactoryNew(),
        )
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
    return {name: create_or_update_material(name, spec) for name, spec in MATERIAL_SPECS.items()}


def static_mesh_component(actor):
    return actor.get_component_by_class(unreal.StaticMeshComponent)


def set_mesh(actor, mesh, scale):
    component = static_mesh_component(actor)
    if component and mesh:
        component.set_static_mesh(mesh)
        set_prop(component, "cast_shadow", True)
    actor.set_actor_scale3d(scale)


def set_material(actor, material):
    component = static_mesh_component(actor)
    if component and material:
        component.set_material(0, material)


def spawn_static(label, location, scale, mesh, material, rotation=None):
    actor = ACTOR_EDITOR.spawn_actor_from_class(
        unreal.StaticMeshActor,
        location,
        rotation or unreal.Rotator(0.0, 0.0, 0.0),
    )
    actor.set_actor_label(label)
    actor.set_folder_path("DockShield/DeepDock")
    set_mesh(actor, mesh, scale)
    set_material(actor, material)
    return actor


def spawn_target(label, location, scale, tags, mesh, material):
    target_class = getattr(unreal, "DSTargetActor", unreal.StaticMeshActor)
    actor = ACTOR_EDITOR.spawn_actor_from_class(target_class, location, unreal.Rotator(0.0, 0.0, 0.0))
    actor.set_actor_label(label)
    actor.set_folder_path("DockShield/DeepDock/Targets")
    actor.tags = [unreal.Name("DockShieldTarget")] + [unreal.Name(tag) for tag in tags]
    set_mesh(actor, mesh, scale)
    set_material(actor, material)
    targetable = actor.get_component_by_class(getattr(unreal, "DSTargetableComponent", unreal.ActorComponent))
    if targetable:
        set_prop(targetable, "interaction_range", 2400.0)
        try:
            targetable.configure_from_owner_tags()
        except Exception as exc:
            unreal.log_warning(f"Could not configure targetable {label}: {exc}")
    return actor


def spawn_point_light(label, location, color, intensity, radius):
    light = ACTOR_EDITOR.spawn_actor_from_class(unreal.PointLight, location, unreal.Rotator(0.0, 0.0, 0.0))
    light.set_actor_label(label)
    light.set_folder_path("DockShield/DeepDock/Lighting")
    component = light.get_component_by_class(unreal.PointLightComponent)
    if component:
        set_prop(component, "light_color", color)
        set_prop(component, "intensity", intensity)
        set_prop(component, "attenuation_radius", radius)
        set_prop(component, "source_radius", 120.0)
    return light


def destroy_existing_pass():
    for actor in list(ACTOR_EDITOR.get_all_level_actors()):
        if actor.get_actor_label().startswith(RESET_PREFIX):
            ACTOR_EDITOR.destroy_actor(actor)


def create_arena():
    if not unreal.EditorAssetLibrary.does_asset_exist(MAP_PATH):
        fail(f"missing map: {MAP_PATH}")
    LEVEL_EDITOR.load_level(MAP_PATH)
    destroy_existing_pass()

    materials = build_materials()
    cube = load_first(CUBE_MESH_PATHS)
    plane = load_first(PLANE_MESH_PATHS)
    sphere = load_first(SPHERE_MESH_PATHS)
    cylinder = load_first(CYLINDER_MESH_PATHS)

    arena_class = getattr(unreal, "DSDeepDockBossArenaActor", None)
    if arena_class is None:
        fail("DSDeepDockBossArenaActor class is unavailable. Build DockShieldEditor first.")

    arena_state = ACTOR_EDITOR.spawn_actor_from_class(
        arena_class,
        unreal.Vector(1850.0, 0.0, 140.0),
        unreal.Rotator(0.0, 0.0, 0.0),
    )
    arena_state.set_actor_label("DS_DeepDock_BossArena_State")
    arena_state.set_folder_path("DockShield/DeepDock")

    spawn_static(
        "DS_DeepDock_Arena_Floor",
        unreal.Vector(1900.0, 0.0, -12.0),
        unreal.Vector(10.0, 8.0, 0.10),
        plane,
        materials["M_DS_DeepDockBlackWater_Proto"],
    )

    spawn_static(
        "DS_DeepDock_Service_Deck",
        unreal.Vector(1550.0, -420.0, 80.0),
        unreal.Vector(5.5, 0.34, 0.12),
        cube,
        materials["M_DS_DeepDockWetDeck_Proto"],
    )
    spawn_static(
        "DS_DeepDock_Service_Deck_Right",
        unreal.Vector(1550.0, 420.0, 80.0),
        unreal.Vector(5.5, 0.34, 0.12),
        cube,
        materials["M_DS_DeepDockWetDeck_Proto"],
    )
    spawn_static(
        "DS_DeepDock_Back_Rig",
        unreal.Vector(2200.0, 0.0, 260.0),
        unreal.Vector(0.18, 6.5, 3.2),
        cube,
        materials["M_DS_DeepDockWetDeck_Proto"],
    )

    for index, angle in enumerate((0.0, 120.0, 240.0), start=1):
        radians = math.radians(angle)
        x = 1900.0 + math.cos(radians) * 440.0
        y = math.sin(radians) * 360.0
        spawn_static(
            f"DS_DeepDock_Warning_Pylon_{index}",
            unreal.Vector(x, y, 190.0),
            unreal.Vector(0.24, 0.24, 2.2),
            cylinder,
            materials["M_DS_DeepDockWarningRed_Proto"],
        )

    spawn_target(
        "DS_DeepDock_Boss_Core_Target",
        unreal.Vector(2100.0, 0.0, 250.0),
        unreal.Vector(2.8, 2.2, 2.2),
        ["Boss"],
        sphere,
        materials["M_DS_DeepDockBioCore_Proto"],
    )

    weakpoint_specs = [
        ("DS_DeepDock_Boss_WeakPoint_Left", unreal.Vector(1850.0, -360.0, 230.0)),
        ("DS_DeepDock_Boss_WeakPoint_Right", unreal.Vector(1850.0, 360.0, 230.0)),
        ("DS_DeepDock_Boss_WeakPoint_Crown", unreal.Vector(2280.0, 0.0, 430.0)),
    ]
    for label, location in weakpoint_specs:
        spawn_target(
            label,
            location,
            unreal.Vector(0.55, 0.55, 0.55),
            ["WeakPoint"],
            sphere,
            materials["M_DS_DeepDockWeakPoint_Proto"],
        )

    spawn_static(
        "DS_DeepDock_Toxic_Pool",
        unreal.Vector(1900.0, 0.0, 10.0),
        unreal.Vector(3.4, 3.4, 0.05),
        cylinder,
        materials["M_DS_DeepDockBioCore_Proto"],
    )

    spawn_point_light(
        "DS_DeepDock_Light_BioCore",
        unreal.Vector(2100.0, 0.0, 500.0),
        unreal.Color(36, 255, 92, 255),
        8500.0,
        1400.0,
    )
    spawn_point_light(
        "DS_DeepDock_Light_Warning_Left",
        unreal.Vector(1700.0, -520.0, 430.0),
        unreal.Color(255, 32, 8, 255),
        4200.0,
        900.0,
    )
    spawn_point_light(
        "DS_DeepDock_Light_Warning_Right",
        unreal.Vector(1700.0, 520.0, 430.0),
        unreal.Color(255, 32, 8, 255),
        4200.0,
        900.0,
    )

    LEVEL_EDITOR.save_current_level()
    unreal.log("Applied Deep Dock boss arena prototype pass.")


create_arena()
