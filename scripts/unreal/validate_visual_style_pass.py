import unreal


MAP_PATH = "/Game/DockShield/Maps/M_Test_Targeting"
MATERIAL_DIR = "/Game/DockShield/Environment/Bayou/Materials"

EXPECTED_MATERIALS = [
    "M_DS_WetDockWood_Proto",
    "M_DS_SwampMud_Proto",
    "M_DS_ToxicAlgaeWater_Proto",
    "M_DS_MossyReed_Proto",
    "M_DS_CrackedNeonMetal_Proto",
    "M_DS_GrappleAnchor_Proto",
    "M_DS_CivilianMarker_Proto",
    "M_DS_RescueBoat_Proto",
    "M_DS_StormBackdrop_Proto",
]

EXPECTED_ACTORS = [
    "DS_Visual_Water_Surface",
    "DS_Visual_Dock_Plank_01",
    "DS_Visual_Dock_Plank_10",
    "DS_Visual_Reed_01",
    "DS_Visual_Reed_08",
    "DS_Visual_Backdrop_Shack_01",
    "DS_Visual_Neon_Depth_Sign",
    "DS_Visual_Toxic_Glow_Plate",
    "DS_Light_Moon_Key",
    "DS_Light_Toxic_Glow",
    "DS_Light_Rescue_Amber",
    "DS_Light_Depth_Neon",
    "DS_Fog_Bayou_Visual",
    "DS_PostProcess_StormGrade",
]

EXPECTED_EXISTING_ASSIGNMENTS = {
    "Graybox_Floor": "M_DS_SwampMud_Proto",
    "Target_GrapplePull_Debug": "M_DS_GrappleAnchor_Proto",
    "Target_CivilianRescue_Debug": "M_DS_CivilianMarker_Proto",
    "Hazard_ToxicWater_Debug": "M_DS_ToxicAlgaeWater_Proto",
    "DS_Boat_Prototype_Tow": "M_DS_RescueBoat_Proto",
}

LEVEL_EDITOR = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
ACTOR_EDITOR = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)


def fail(message):
    raise RuntimeError(f"DockShield visual style validation failed: {message}")


def actor_by_label(label):
    return next((actor for actor in ACTOR_EDITOR.get_all_level_actors() if actor.get_actor_label() == label), None)


def static_mesh_component(actor):
    return actor.get_component_by_class(unreal.StaticMeshComponent) if actor else None


def material_name_for_actor(actor):
    component = static_mesh_component(actor)
    if not component:
        return ""
    material = component.get_material(0)
    return material.get_name() if material else ""


def main():
    if not unreal.EditorAssetLibrary.does_asset_exist(MAP_PATH):
        fail(f"missing map asset {MAP_PATH}")

    for material_name in EXPECTED_MATERIALS:
        path = f"{MATERIAL_DIR}/{material_name}"
        if not unreal.EditorAssetLibrary.does_asset_exist(path):
            fail(f"missing visual material asset {path}")

    LEVEL_EDITOR.load_level(MAP_PATH)

    for label in EXPECTED_ACTORS:
        if actor_by_label(label) is None:
            fail(f"missing visual pass actor {label}")

    for label, expected_material in EXPECTED_EXISTING_ASSIGNMENTS.items():
        actor = actor_by_label(label)
        if actor is None:
            fail(f"missing actor {label}")
        actual_material = material_name_for_actor(actor)
        if expected_material not in actual_material:
            fail(f"{label} expected material {expected_material}, got {actual_material}")

    water_surface = actor_by_label("DS_Visual_Water_Surface")
    water_material = material_name_for_actor(water_surface)
    if "M_DS_ToxicAlgaeWater_Proto" not in water_material:
        fail(f"water surface material mismatch: {water_material}")

    unreal.log("DockShield visual style validation passed: materials, map dressing, lighting markers, and assignments are present.")


main()
