import unreal


CONTENT_DIRS = [
    "/Game/DockShield",
    "/Game/DockShield/Characters",
    "/Game/DockShield/Characters/Reel",
    "/Game/DockShield/Characters/Fly",
    "/Game/DockShield/Characters/Lilly",
    "/Game/DockShield/Core",
    "/Game/DockShield/Core/Abilities",
    "/Game/DockShield/Core/Targeting",
    "/Game/DockShield/Core/Weather",
    "/Game/DockShield/Core/CommandMode",
    "/Game/DockShield/Data",
    "/Game/DockShield/Enemies",
    "/Game/DockShield/Environment",
    "/Game/DockShield/Environment/Bayou",
    "/Game/DockShield/Environment/Docks",
    "/Game/DockShield/Environment/Hazards",
    "/Game/DockShield/Maps",
    "/Game/DockShield/Missions",
    "/Game/DockShield/UI",
    "/Game/DockShield/VFX",
    "/Game/DockShield/Audio",
]

MAP_PATH = "/Game/DockShield/Maps/M_Test_Targeting"
CUBE_MESH_PATH = "/Game/LevelPrototyping/Meshes/SM_Cube"
PLANE_MESH_PATH = "/Game/LevelPrototyping/Meshes/SM_Plane"

LEVEL_EDITOR = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
ACTOR_EDITOR = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)


def make_dirs():
    for path in CONTENT_DIRS:
        unreal.EditorAssetLibrary.make_directory(path)


def set_mesh(actor, mesh_path, scale):
    mesh = unreal.load_asset(mesh_path)
    component = actor.get_component_by_class(unreal.StaticMeshComponent)
    if component and mesh:
        component.set_static_mesh(mesh)
    actor.set_actor_scale3d(scale)


def spawn_static(label, location, scale, mesh_path=CUBE_MESH_PATH):
    actor = ACTOR_EDITOR.spawn_actor_from_class(
        unreal.StaticMeshActor,
        location,
        unreal.Rotator(0.0, 0.0, 0.0),
    )
    actor.set_actor_label(label)
    set_mesh(actor, mesh_path, scale)
    return actor


def create_map():
    if unreal.EditorAssetLibrary.does_asset_exist(MAP_PATH):
        unreal.log(f"{MAP_PATH} already exists; loading without modifying actors.")
        LEVEL_EDITOR.load_level(MAP_PATH)
        return
    else:
        unreal.log(f"Creating {MAP_PATH}.")
        LEVEL_EDITOR.new_level(MAP_PATH)

    player_start = ACTOR_EDITOR.spawn_actor_from_class(
        unreal.PlayerStart,
        unreal.Vector(-600.0, 0.0, 100.0),
        unreal.Rotator(0.0, 0.0, 0.0),
    )
    player_start.set_actor_label("PlayerStart_Test_Targeting")

    floor = spawn_static(
        "Graybox_Floor",
        unreal.Vector(0.0, 0.0, -10.0),
        unreal.Vector(12.0, 12.0, 0.2),
        PLANE_MESH_PATH,
    )
    floor.set_folder_path("DockShield/Graybox")

    grapple = spawn_static(
        "Target_GrapplePull_Debug",
        unreal.Vector(250.0, -250.0, 100.0),
        unreal.Vector(1.0, 1.0, 2.0),
    )
    grapple.set_folder_path("DockShield/Targets")
    grapple.tags = [unreal.Name("DockShieldTarget"), unreal.Name("GrapplePoint")]

    civilian = spawn_static(
        "Target_CivilianRescue_Debug",
        unreal.Vector(250.0, 250.0, 100.0),
        unreal.Vector(1.0, 1.0, 1.5),
    )
    civilian.set_folder_path("DockShield/Targets")
    civilian.tags = [unreal.Name("DockShieldTarget"), unreal.Name("Civilian")]

    hazard = spawn_static(
        "Hazard_ToxicWater_Debug",
        unreal.Vector(650.0, 0.0, 20.0),
        unreal.Vector(2.5, 2.5, 0.15),
        PLANE_MESH_PATH,
    )
    hazard.set_folder_path("DockShield/Hazards")
    hazard.tags = [unreal.Name("DockShieldTarget"), unreal.Name("Hazard")]

    light = ACTOR_EDITOR.spawn_actor_from_class(
        unreal.PointLight,
        unreal.Vector(0.0, 0.0, 450.0),
        unreal.Rotator(0.0, 0.0, 0.0),
    )
    light.set_actor_label("Light_Test_Targeting")
    light.set_folder_path("DockShield/Lighting")
    light_component = light.get_component_by_class(unreal.PointLightComponent)
    if light_component:
        light_component.set_editor_property("intensity", 5000.0)

    LEVEL_EDITOR.save_current_level()
    unreal.log(f"Saved {MAP_PATH}.")


make_dirs()
create_map()
