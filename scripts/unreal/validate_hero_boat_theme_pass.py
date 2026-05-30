import unreal


MAP_PATH = "/Game/DockShield/Maps/M_Test_Targeting"
MATERIAL_DIR = "/Game/DockShield/Environment/Bayou/Materials"

EXPECTED_MATERIALS = [
    "M_DS_ReelTheme_Proto",
    "M_DS_FlyTheme_Proto",
    "M_DS_LillyTheme_Proto",
]

EXPECTED_BOATS = {
    "DS_Boat_Prototype_Tow": {
        "label": "REEL",
        "emblem": "R // THE REEL",
        "loadout": "REEL-GAUNTLET",
        "tag": "ReelBoat",
    },
    "DS_Boat_Fly_Blackwater": {
        "label": "FLY",
        "emblem": "FLY // COVERT",
        "loadout": "STEALTH BLACK/TEAL",
        "tag": "FlyBoat",
    },
    "DS_Boat_Lilly_Swamp": {
        "label": "LILLY",
        "emblem": "LILLY // SWAMP QUEEN",
        "loadout": "PINK/LIME/MUD CAMO",
        "tag": "LillyBoat",
    },
}

EXPECTED_GADGETS = {
    "DS_Gadget_Reel_Gauntlet_Debug": ["DockShieldGadget", "ReelGear", "ReelGauntlet"],
    "DS_Gadget_Fly_SonarProbe_Debug": ["DockShieldGadget", "FlyGear", "SonarProbe"],
    "DS_Gadget_Lilly_VineReel_Debug": ["DockShieldGadget", "LillyGear", "VineReel"],
}

LEVEL_EDITOR = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
ACTOR_EDITOR = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)


def fail(message):
    raise RuntimeError(f"DockShield hero boat theme validation failed: {message}")


def actor_by_label(label):
    return next((actor for actor in ACTOR_EDITOR.get_all_level_actors() if actor.get_actor_label() == label), None)


def require_method(actor, method_name):
    if not hasattr(actor, method_name):
        fail(f"{actor.get_actor_label()} missing Python method {method_name}")


def tag_set(actor):
    return {str(tag) for tag in actor.tags}


def main():
    if not unreal.EditorAssetLibrary.does_asset_exist(MAP_PATH):
        fail(f"missing map asset {MAP_PATH}")

    for material_name in EXPECTED_MATERIALS:
        material_path = f"{MATERIAL_DIR}/{material_name}"
        if not unreal.EditorAssetLibrary.does_asset_exist(material_path):
            fail(f"missing theme material {material_path}")

    LEVEL_EDITOR.load_level(MAP_PATH)

    for label, expected in EXPECTED_BOATS.items():
        boat = actor_by_label(label)
        if boat is None:
            fail(f"missing themed boat {label}")

        for method_name in [
            "get_boat_theme_label",
            "get_boat_emblem_text",
            "get_boat_loadout_text",
            "get_boat_primary_color",
            "get_boat_accent_color",
        ]:
            require_method(boat, method_name)

        if expected["label"] not in boat.get_boat_theme_label():
            fail(f"{label} theme label mismatch: {boat.get_boat_theme_label()}")
        if expected["emblem"] != boat.get_boat_emblem_text():
            fail(f"{label} emblem mismatch: {boat.get_boat_emblem_text()}")
        if expected["loadout"] not in boat.get_boat_loadout_text():
            fail(f"{label} loadout mismatch: {boat.get_boat_loadout_text()}")
        if expected["tag"] not in tag_set(boat):
            fail(f"{label} missing tag {expected['tag']}")

        primary = boat.get_boat_primary_color()
        accent = boat.get_boat_accent_color()
        color_delta = abs(primary.r - accent.r) + abs(primary.g - accent.g) + abs(primary.b - accent.b)
        if color_delta < 0.05:
            fail(f"{label} primary/accent colors are not distinct enough")

    for label, expected_tags in EXPECTED_GADGETS.items():
        gadget = actor_by_label(label)
        if gadget is None:
            fail(f"missing themed gadget marker {label}")
        tags = tag_set(gadget)
        missing = [tag for tag in expected_tags if tag not in tags]
        if missing:
            fail(f"{label} missing tags: {', '.join(missing)}")
        component = gadget.get_component_by_class(unreal.StaticMeshComponent)
        if component is None or component.static_mesh is None:
            fail(f"{label} missing static mesh")

    unreal.log("DockShield hero boat theme validation passed: boats, emblems, loadouts, colors, and gadget markers are present.")


main()
