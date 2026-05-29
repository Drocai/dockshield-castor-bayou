import os

import unreal


MAP_PATH = "/Game/DockShield/Maps/M_Test_Targeting"
DEST_DIR = "/Game/DockShield/Characters/Reel/Proxy/Copilot3D"
MATERIAL_DIR = "/Game/DockShield/Characters/Reel/Materials"
SOURCE_RELATIVE_PATH = os.path.join("SourceAssets", "Private", "Copilot3D", "SM_Reel_Copilot3D_Proxy.glb")

EXPECTED_MATERIALS = [
    "M_DS_ReelProxy_WetBlueBlack_Proto",
    "M_DS_ReelProxy_RedBrass_Proto",
    "M_DS_ReelProxy_ToxicGreen_Proto",
]

EXPECTED_ACTORS = [
    "DS_Reel_Proxy_Copilot3D_Static",
    "DS_Reel_Proxy_Review_Base",
    "DS_Reel_Proxy_Reel_Accent",
    "DS_Reel_Proxy_Toxic_Accent",
    "DS_Reel_Proxy_Toxic_Glow",
]

LEVEL_EDITOR = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
ACTOR_EDITOR = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)


def fail(message):
    raise RuntimeError(f"DockShield Reel proxy validation failed: {message}")


def repo_root():
    return os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))


def actor_by_label(label):
    return next((actor for actor in ACTOR_EDITOR.get_all_level_actors() if actor.get_actor_label() == label), None)


def static_mesh_component(actor):
    return actor.get_component_by_class(unreal.StaticMeshComponent) if actor else None


def find_imported_static_mesh():
    if not unreal.EditorAssetLibrary.does_directory_exist(DEST_DIR):
        return None
    for path in unreal.EditorAssetLibrary.list_assets(DEST_DIR, recursive=True, include_folder=False):
        asset = unreal.load_asset(path)
        if isinstance(asset, unreal.StaticMesh):
            return asset
    return None


def main():
    source_path = os.path.join(repo_root(), SOURCE_RELATIVE_PATH)
    if not os.path.exists(source_path):
        fail(f"missing private source GLB {source_path}")

    if not unreal.EditorAssetLibrary.does_asset_exist(MAP_PATH):
        fail(f"missing map asset {MAP_PATH}")

    imported_mesh = find_imported_static_mesh()
    if imported_mesh is None:
        fail(f"missing imported static mesh under {DEST_DIR}")

    for material_name in EXPECTED_MATERIALS:
        material_path = f"{MATERIAL_DIR}/{material_name}"
        if not unreal.EditorAssetLibrary.does_asset_exist(material_path):
            fail(f"missing Reel proxy material {material_path}")

    LEVEL_EDITOR.load_level(MAP_PATH)

    for label in EXPECTED_ACTORS:
        actor = actor_by_label(label)
        if actor is None:
            fail(f"missing Reel proxy actor {label}")

    proxy = actor_by_label("DS_Reel_Proxy_Copilot3D_Static")
    proxy_component = static_mesh_component(proxy)
    if proxy_component is None or proxy_component.static_mesh is None:
        fail("DS_Reel_Proxy_Copilot3D_Static has no static mesh component")

    if proxy_component.static_mesh.get_path_name() != imported_mesh.get_path_name():
        fail(
            "proxy actor is not using imported Reel mesh: "
            f"{proxy_component.static_mesh.get_path_name()} != {imported_mesh.get_path_name()}"
        )

    tags = {str(tag) for tag in proxy.tags}
    for required_tag in ["DockShieldVisualProxy", "ReelProxy", "PrivateReference"]:
        if required_tag not in tags:
            fail(f"proxy actor missing tag {required_tag}")

    if proxy.get_actor_scale3d().z < 1.0:
        fail("proxy actor scale is too small for human-height review")

    unreal.log(
        "DockShield Reel proxy validation passed: source GLB, imported static mesh, "
        "materials, proxy actors, tags, and map placement are present."
    )


main()
