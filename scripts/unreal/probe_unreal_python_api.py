import os

import unreal


PROJECT_SAVED_DIR = unreal.Paths.project_saved_dir()
OUTPUT_PATH = os.path.join(PROJECT_SAVED_DIR, "python_api_probe.txt")


def write_line(handle, text):
    handle.write(text + "\n")


with open(OUTPUT_PATH, "w", encoding="utf-8") as handle:
    for name in [
        "EditorAssetLibrary",
        "EditorLevelLibrary",
        "EditorLoadingAndSavingUtils",
        "LevelEditorSubsystem",
        "EditorActorSubsystem",
        "AssetToolsHelpers",
        "StaticMeshActor",
        "PointLight",
        "PlayerStart",
    ]:
        value = getattr(unreal, name, None)
        write_line(handle, f"{name}={value is not None}")
        if value is not None:
            public_methods = [item for item in dir(value) if not item.startswith("_")]
            write_line(handle, f"{name}.methods={','.join(public_methods[:80])}")

unreal.log(f"DockShield Python API probe written to {OUTPUT_PATH}")
