import unreal


MAP_PATH = "/Game/DockShield/Maps/M_Test_Targeting"
MATERIAL_DIR = "/Game/DockShield/Enemies/Mutation/Materials"
RESET_PREFIX = "DS_MutationEnemy_"

LEVEL_EDITOR = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
ACTOR_EDITOR = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

SPHERE_MESH_PATHS = ["/Engine/BasicShapes/Sphere.Sphere"]
CYLINDER_MESH_PATHS = ["/Engine/BasicShapes/Cylinder.Cylinder"]
CUBE_MESH_PATHS = ["/Engine/BasicShapes/Cube.Cube"]

MATERIAL_SPECS = {
    "M_DS_MutationHide_Proto": {
        "base": unreal.LinearColor(0.08, 0.12, 0.10, 1.0),
        "roughness": 0.18,
        "metallic": 0.0,
        "emissive": unreal.LinearColor(0.0, 0.025, 0.01, 1.0),
    },
    "M_DS_MutationCoreGlow_Proto": {
        "base": unreal.LinearColor(0.12, 0.55, 0.14, 1.0),
        "roughness": 0.10,
        "metallic": 0.0,
        "emissive": unreal.LinearColor(0.25, 1.0, 0.10, 1.0),
    },
    "M_DS_MutationWarningRed_Proto": {
        "base": unreal.LinearColor(0.74, 0.06, 0.02, 1.0),
        "roughness": 0.22,
        "metallic": 0.05,
        "emissive": unreal.LinearColor(1.0, 0.08, 0.02, 1.0),
    },
    "M_DS_MutationToxicWake_Proto": {
        "base": unreal.LinearColor(0.18, 0.48, 0.08, 1.0),
        "roughness": 0.06,
        "metallic": 0.0,
        "emissive": unreal.LinearColor(0.18, 0.70, 0.04, 1.0),
    },
}


def fail(message):
    raise RuntimeError(f"DockShield mutation enemy pass failed: {message}")


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
        unreal.log_warning(f"MaterialProperty.{name} unavailable; skipping connection")
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
            fail(f"could not create material {asset_path}")

    material.modify()
    unreal.MaterialEditingLibrary.delete_all_material_expressions(material)
    connect_material_constant(material, unreal.MaterialExpressionConstant3Vector, "constant", spec["base"], "MP_BASE_COLOR", -520, -120)
    connect_material_constant(material, unreal.MaterialExpressionConstant, "r", float(spec["roughness"]), "MP_ROUGHNESS", -520, 60)
    connect_material_constant(material, unreal.MaterialExpressionConstant, "r", float(spec["metallic"]), "MP_METALLIC", -520, 190)
    if spec["emissive"] is not None:
        connect_material_constant(material, unreal.MaterialExpressionConstant3Vector, "constant", spec["emissive"], "MP_EMISSIVE_COLOR", -520, 330)

    set_prop(material, "used_with_static_mesh", True)
    set_prop(material, "use_emissive_for_dynamic_area_lighting", spec["emissive"] is not None)
    unreal.MaterialEditingLibrary.recompile_material(material)
    unreal.EditorAssetLibrary.save_asset(asset_path)
    return material


def static_mesh_component(actor):
    return actor.get_component_by_class(unreal.StaticMeshComponent)


def set_mesh(actor, mesh, scale):
    component = static_mesh_component(actor)
    if component and mesh:
        component.set_static_mesh(mesh)
        set_prop(component, "cast_shadow", True)
        set_prop(component, "mobility", unreal.ComponentMobility.MOVABLE)
    actor.set_actor_scale3d(scale)


def set_material(actor, material):
    component = static_mesh_component(actor)
    if component and material:
        component.set_material(0, material)


def destroy_existing_pass():
    for actor in list(ACTOR_EDITOR.get_all_level_actors()):
        if actor.get_actor_label().startswith(RESET_PREFIX):
            ACTOR_EDITOR.destroy_actor(actor)


def spawn_static(label, location, rotation, scale, mesh, material):
    actor = ACTOR_EDITOR.spawn_actor_from_class(unreal.StaticMeshActor, location, rotation)
    actor.set_actor_label(label)
    actor.set_folder_path("DockShield/Enemies/Mutation")
    set_mesh(actor, mesh, scale)
    set_material(actor, material)
    return actor


def spawn_point_light(label, location, color, intensity, radius):
    light = ACTOR_EDITOR.spawn_actor_from_class(unreal.PointLight, location, unreal.Rotator(0.0, 0.0, 0.0))
    light.set_actor_label(label)
    light.set_folder_path("DockShield/Enemies/Mutation/Lighting")
    component = light.get_component_by_class(unreal.PointLightComponent)
    if component:
        set_prop(component, "light_color", color)
        set_prop(component, "intensity", intensity)
        set_prop(component, "attenuation_radius", radius)
        set_prop(component, "source_radius", 55.0)
    return light


def apply_pass():
    if not unreal.EditorAssetLibrary.does_asset_exist(MAP_PATH):
        fail(f"missing map: {MAP_PATH}")

    LEVEL_EDITOR.load_level(MAP_PATH)
    destroy_existing_pass()

    materials = {name: create_or_update_material(name, spec) for name, spec in MATERIAL_SPECS.items()}
    sphere = load_first(SPHERE_MESH_PATHS)
    cylinder = load_first(CYLINDER_MESH_PATHS)
    cube = load_first(CUBE_MESH_PATHS)

    mutation_class = getattr(unreal, "DSMutationEnemyActor", None)
    if mutation_class is None:
        fail("DSMutationEnemyActor class is unavailable. Build DockShieldEditor first.")

    mutation = ACTOR_EDITOR.spawn_actor_from_class(
        mutation_class,
        unreal.Vector(1180.0, -180.0, 128.0),
        unreal.Rotator(0.0, -38.0, 0.0),
    )
    mutation.set_actor_label("DS_MutationEnemy_BayouBruiser_Debug")
    mutation.set_folder_path("DockShield/Enemies/Mutation")
    mutation.tags = [unreal.Name("DockShieldTarget"), unreal.Name("Enemy"), unreal.Name("MutationEnemy")]
    set_mesh(mutation, sphere, unreal.Vector(0.82, 0.56, 1.22))
    set_material(mutation, materials["M_DS_MutationHide_Proto"])

    targetable_cls = getattr(unreal, "DSTargetableComponent", None)
    if targetable_cls:
        targetable = mutation.get_component_by_class(targetable_cls)
        if targetable:
            set_prop(targetable, "interaction_range", 1850.0)
            targetable.configure_from_owner_tags()

    spawn_static(
        "DS_MutationEnemy_CoreGlow",
        unreal.Vector(1180.0, -180.0, 190.0),
        unreal.Rotator(0.0, 0.0, 0.0),
        unreal.Vector(0.18, 0.18, 0.18),
        sphere,
        materials["M_DS_MutationCoreGlow_Proto"],
    )
    spawn_static(
        "DS_MutationEnemy_WarningMark",
        unreal.Vector(1180.0, -180.0, 284.0),
        unreal.Rotator(90.0, 0.0, 0.0),
        unreal.Vector(0.54, 0.54, 0.018),
        cylinder,
        materials["M_DS_MutationWarningRed_Proto"],
    )
    spawn_static(
        "DS_MutationEnemy_ToxicWake",
        unreal.Vector(1180.0, -180.0, 54.0),
        unreal.Rotator(90.0, 0.0, 0.0),
        unreal.Vector(1.20, 0.86, 0.018),
        cylinder,
        materials["M_DS_MutationToxicWake_Proto"],
    )
    spawn_static(
        "DS_MutationEnemy_RustedCover",
        unreal.Vector(1064.0, -262.0, 82.0),
        unreal.Rotator(0.0, 28.0, 0.0),
        unreal.Vector(0.72, 0.10, 0.38),
        cube,
        materials["M_DS_MutationWarningRed_Proto"],
    )
    spawn_point_light(
        "DS_MutationEnemy_ToxicGlow",
        unreal.Vector(1180.0, -180.0, 236.0),
        unreal.Color(85, 255, 52, 255),
        2800.0,
        740.0,
    )
    spawn_point_light(
        "DS_MutationEnemy_WarningLight",
        unreal.Vector(1136.0, -228.0, 320.0),
        unreal.Color(255, 48, 22, 255),
        1200.0,
        520.0,
    )

    LEVEL_EDITOR.save_current_level()
    unreal.log("Applied DockShield mutation enemy prototype pass.")


apply_pass()
