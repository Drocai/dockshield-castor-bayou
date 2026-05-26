import unreal


MAP_PATH = "/Game/DockShield/Maps/M_Test_Targeting"

REQUIRED_DIRS = [
    "/Game/DockShield/Characters/Reel",
    "/Game/DockShield/Characters/Fly",
    "/Game/DockShield/Characters/Lilly",
    "/Game/DockShield/Core/Abilities",
    "/Game/DockShield/Core/Targeting",
    "/Game/DockShield/Core/Weather",
    "/Game/DockShield/Core/CommandMode",
    "/Game/DockShield/Data",
    "/Game/DockShield/Enemies",
    "/Game/DockShield/Environment/Bayou",
    "/Game/DockShield/Environment/Docks",
    "/Game/DockShield/Environment/Hazards",
    "/Game/DockShield/Maps",
    "/Game/DockShield/Missions",
    "/Game/DockShield/UI",
    "/Game/DockShield/VFX",
    "/Game/DockShield/Audio",
]

REQUIRED_ACTORS = {
    "PlayerStart_Test_Targeting": [],
    "Graybox_Floor": [],
    "Target_GrapplePull_Debug": ["DockShieldTarget", "GrapplePoint"],
    "Target_CivilianRescue_Debug": ["DockShieldTarget", "Civilian"],
    "Hazard_ToxicWater_Debug": ["DockShieldTarget", "Hazard"],
    "Light_Test_Targeting": [],
    "DS_Light_Key_Storm": [],
    "DS_Light_Toxic_Hazard": [],
    "DS_Light_Rescue_Warm": [],
    "DS_Fog_Bayou_Test": [],
}

LEVEL_EDITOR = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
ACTOR_EDITOR = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
TARGETABLE_COMPONENT_CLASS = getattr(unreal, "DSTargetableComponent", None)


def fail(message):
    raise RuntimeError(f"DockShield targeting map validation failed: {message}")


def main():
    if not unreal.EditorAssetLibrary.does_asset_exist(MAP_PATH):
        fail(f"missing map asset {MAP_PATH}")

    for directory in REQUIRED_DIRS:
        if not unreal.EditorAssetLibrary.does_directory_exist(directory):
            fail(f"missing content directory {directory}")

    LEVEL_EDITOR.load_level(MAP_PATH)
    actors_by_label = {
        actor.get_actor_label(): actor for actor in ACTOR_EDITOR.get_all_level_actors()
    }

    for label, required_tags in REQUIRED_ACTORS.items():
        actor = actors_by_label.get(label)
        if actor is None:
            fail(f"missing actor {label}")

        actor_tags = {str(tag) for tag in actor.tags}
        missing_tags = [tag for tag in required_tags if tag not in actor_tags]
        if missing_tags:
            fail(f"{label} missing tags: {', '.join(missing_tags)}")

        if (
            label != "PlayerStart_Test_Targeting"
            and label != "Light_Test_Targeting"
            and not label.startswith("DS_Light_")
            and label != "DS_Fog_Bayou_Test"
        ):
            mesh_component = actor.get_component_by_class(unreal.StaticMeshComponent)
            if mesh_component is None or mesh_component.static_mesh is None:
                fail(f"{label} has no static mesh")

        if TARGETABLE_COMPONENT_CLASS and required_tags:
            targetable_component = actor.get_component_by_class(TARGETABLE_COMPONENT_CLASS)
            if targetable_component is None:
                fail(f"{label} is missing DSTargetableComponent")

    unreal.log(
        "DockShield targeting map validation passed: "
        f"{len(REQUIRED_DIRS)} directories, {len(REQUIRED_ACTORS)} actors."
    )


main()
