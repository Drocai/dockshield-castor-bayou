import os

import unreal


MAP_PATH = "/Game/DockShield/Maps/M_Test_Targeting"
DEST_DIR = "/Game/DockShield/Characters/Reel/Proxy/Copilot3D"
MATERIAL_DIR = "/Game/DockShield/Characters/Reel/Materials"
SOURCE_RELATIVE_PATH = os.path.join("SourceAssets", "Private", "Copilot3D", "SM_Reel_Copilot3D_Proxy.glb")
STATIC_MESH_NAME = "SM_Reel_Copilot3D_Proxy"

PROXY_ACTOR_LABEL = "DS_Reel_Proxy_Copilot3D_Static"
BASE_ACTOR_LABEL = "DS_Reel_Proxy_Review_Base"
ACCENT_ACTOR_LABEL = "DS_Reel_Proxy_Reel_Accent"
TOXIC_ACTOR_LABEL = "DS_Reel_Proxy_Toxic_Accent"
LIGHT_ACTOR_LABEL = "DS_Reel_Proxy_Toxic_Glow"

LEVEL_EDITOR = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
ACTOR_EDITOR = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

CUBE_MESH_PATHS = ["/Engine/BasicShapes/Cube.Cube", "/Game/LevelPrototyping/Meshes/SM_Cube.SM_Cube"]
CYLINDER_MESH_PATHS = ["/Engine/BasicShapes/Cylinder.Cylinder"]
SPHERE_MESH_PATHS = ["/Engine/BasicShapes/Sphere.Sphere"]

MATERIAL_SPECS = {
    "M_DS_ReelProxy_WetBlueBlack_Proto": {
        "base": unreal.LinearColor(0.018, 0.033, 0.048, 1.0),
        "roughness": 0.18,
        "metallic": 0.06,
        "emissive": None,
    },
    "M_DS_ReelProxy_RedBrass_Proto": {
        "base": unreal.LinearColor(0.56, 0.095, 0.045, 1.0),
        "roughness": 0.22,
        "metallic": 0.58,
        "emissive": unreal.LinearColor(0.10, 0.018, 0.006, 1.0),
    },
    "M_DS_ReelProxy_ToxicGreen_Proto": {
        "base": unreal.LinearColor(0.015, 0.30, 0.075, 1.0),
        "roughness": 0.14,
        "metallic": 0.0,
        "emissive": unreal.LinearColor(0.0, 0.82, 0.18, 1.0),
    },
}


def fail(message):
    raise RuntimeError(f"DockShield Reel proxy pass failed: {message}")


def repo_root():
    return os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))


def source_glb_path():
    return os.path.join(repo_root(), SOURCE_RELATIVE_PATH)


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
    return {name: create_or_update_material(name, spec) for name, spec in MATERIAL_SPECS.items()}


def find_imported_static_mesh():
    if unreal.EditorAssetLibrary.does_directory_exist(DEST_DIR):
        preferred_path = (
            f"{DEST_DIR}/{STATIC_MESH_NAME}/StaticMeshes/"
            f"{STATIC_MESH_NAME}.{STATIC_MESH_NAME}"
        )
        preferred_asset = unreal.load_asset(preferred_path)
        if isinstance(preferred_asset, unreal.StaticMesh):
            return preferred_asset

        for path in unreal.EditorAssetLibrary.list_assets(DEST_DIR, recursive=True, include_folder=False):
            asset = unreal.load_asset(path)
            if isinstance(asset, unreal.StaticMesh):
                return asset

    return None


def import_proxy_static_mesh():
    existing_mesh = find_imported_static_mesh()
    if existing_mesh is not None:
        unreal.log(f"Using existing Reel proxy static mesh: {existing_mesh.get_path_name()}")
        return existing_mesh

    source_path = source_glb_path()
    if not os.path.exists(source_path):
        fail(f"missing source GLB: {source_path}")

    unreal.EditorAssetLibrary.make_directory(DEST_DIR)

    task = unreal.AssetImportTask()
    task.set_editor_property("filename", source_path)
    task.set_editor_property("destination_path", DEST_DIR)
    task.set_editor_property("destination_name", STATIC_MESH_NAME)
    task.set_editor_property("automated", True)
    task.set_editor_property("replace_existing", True)
    task.set_editor_property("save", True)
    try:
        task.set_editor_property("replace_existing_settings", True)
    except Exception:
        pass

    unreal.AssetToolsHelpers.get_asset_tools().import_asset_tasks([task])

    imported_paths = []
    try:
        imported_paths = list(task.get_editor_property("imported_object_paths"))
    except Exception:
        imported_paths = []

    static_mesh = find_imported_static_mesh()

    if static_mesh is None:
        fail(f"no static mesh was imported under {DEST_DIR}; imported paths: {imported_paths}")

    unreal.EditorAssetLibrary.save_directory(DEST_DIR)
    return static_mesh


def destroy_existing_proxy_actors():
    for actor in list(ACTOR_EDITOR.get_all_level_actors()):
        if actor.get_actor_label().startswith("DS_Reel_Proxy_"):
            ACTOR_EDITOR.destroy_actor(actor)


def static_mesh_component(actor):
    return actor.get_component_by_class(unreal.StaticMeshComponent)


def spawn_static(label, location, rotation, scale, mesh, material, folder):
    actor = ACTOR_EDITOR.spawn_actor_from_class(unreal.StaticMeshActor, location, rotation)
    if actor is None:
        fail(f"could not spawn {label}")
    actor.set_actor_label(label)
    actor.set_folder_path(folder)
    component = static_mesh_component(actor)
    if component:
        component.set_static_mesh(mesh)
        if material:
            component.set_material(0, material)
        set_prop(component, "cast_shadow", True)
    actor.set_actor_scale3d(scale)
    return actor


def spawn_point_light(label, location, color, intensity, radius):
    light = ACTOR_EDITOR.spawn_actor_from_class(unreal.PointLight, location, unreal.Rotator(0.0, 0.0, 0.0))
    if light is None:
        fail(f"could not spawn {label}")
    light.set_actor_label(label)
    light.set_folder_path("DockShield/ReelProxy/Lighting")
    component = light.get_component_by_class(get_class("PointLightComponent"))
    if component:
        set_prop(component, "intensity", intensity)
        set_prop(component, "attenuation_radius", radius)
        set_prop(component, "light_color", color)
        set_prop(component, "cast_shadows", True)
    return light


def tag_actor(actor, *tags):
    actor.tags = list({*actor.tags, *[unreal.Name(tag) for tag in tags]})


def place_proxy(static_mesh, materials):
    cylinder = load_first(CYLINDER_MESH_PATHS)
    sphere = load_first(SPHERE_MESH_PATHS)

    proxy = spawn_static(
        PROXY_ACTOR_LABEL,
        unreal.Vector(-600.0, 410.0, 95.0),
        unreal.Rotator(0.0, -55.0, 0.0),
        unreal.Vector(1.9, 1.9, 1.9),
        static_mesh,
        None,
        "DockShield/ReelProxy/Imported",
    )
    tag_actor(proxy, "DockShieldVisualProxy", "ReelProxy", "PrivateReference")

    component = static_mesh_component(proxy)
    if component and component.get_material(0) is None:
        component.set_material(0, materials["M_DS_ReelProxy_WetBlueBlack_Proto"])

    base = spawn_static(
        BASE_ACTOR_LABEL,
        unreal.Vector(-600.0, 410.0, 4.0),
        unreal.Rotator(0.0, 0.0, 0.0),
        unreal.Vector(1.15, 1.15, 0.04),
        cylinder,
        materials["M_DS_ReelProxy_WetBlueBlack_Proto"],
        "DockShield/ReelProxy/Review",
    )
    tag_actor(base, "DockShieldVisualProxy", "ReelProxy")

    accent = spawn_static(
        ACCENT_ACTOR_LABEL,
        unreal.Vector(-498.0, 346.0, 116.0),
        unreal.Rotator(0.0, -55.0, 88.0),
        unreal.Vector(0.16, 0.16, 0.065),
        cylinder,
        materials["M_DS_ReelProxy_RedBrass_Proto"],
        "DockShield/ReelProxy/Review",
    )
    tag_actor(accent, "DockShieldVisualProxy", "ReelProxy")

    toxic = spawn_static(
        TOXIC_ACTOR_LABEL,
        unreal.Vector(-525.0, 360.0, 148.0),
        unreal.Rotator(0.0, 0.0, 0.0),
        unreal.Vector(0.10, 0.10, 0.10),
        sphere,
        materials["M_DS_ReelProxy_ToxicGreen_Proto"],
        "DockShield/ReelProxy/Review",
    )
    tag_actor(toxic, "DockShieldVisualProxy", "ReelProxy")

    spawn_point_light(
        LIGHT_ACTOR_LABEL,
        unreal.Vector(-525.0, 360.0, 158.0),
        unreal.Color(24, 255, 92, 255),
        1150.0,
        250.0,
    )


def main():
    if not unreal.EditorAssetLibrary.does_asset_exist(MAP_PATH):
        fail(f"missing map asset {MAP_PATH}")

    materials = build_materials()
    static_mesh = import_proxy_static_mesh()

    LEVEL_EDITOR.load_level(MAP_PATH)
    destroy_existing_proxy_actors()
    place_proxy(static_mesh, materials)
    LEVEL_EDITOR.save_current_level()
    unreal.EditorAssetLibrary.save_directory(MATERIAL_DIR)

    unreal.log(
        "DockShield Reel proxy pass applied: imported static Copilot3D proxy, "
        "placed scale/lighting review actors, and preserved playable pawn."
    )


main()
