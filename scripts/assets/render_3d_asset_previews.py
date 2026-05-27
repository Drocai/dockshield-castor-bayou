"""Render neutral preview PNGs for candidate GLB/STL assets with Blender.

Usage from repo root:
  blender --background --python scripts/assets/render_3d_asset_previews.py -- <output_dir> <paths...>
"""

from __future__ import annotations

import math
import os
import re
import sys
from pathlib import Path

import bpy
from mathutils import Vector


def _slug(name: str) -> str:
    value = re.sub(r"[^A-Za-z0-9_.-]+", "-", name).strip("-")
    return value[:120] or "asset"


def _clear_scene() -> None:
    bpy.ops.object.select_all(action="SELECT")
    bpy.ops.object.delete()


def _import_asset(path: Path) -> None:
    suffix = path.suffix.lower()
    if suffix in {".glb", ".gltf"}:
        bpy.ops.import_scene.gltf(filepath=str(path))
        return
    if suffix == ".stl":
        if hasattr(bpy.ops.wm, "stl_import"):
            bpy.ops.wm.stl_import(filepath=str(path))
        else:
            bpy.ops.import_mesh.stl(filepath=str(path))
        return
    raise ValueError(f"Unsupported asset extension: {suffix}")


def _bounds(mesh_objects: list[bpy.types.Object]) -> tuple[Vector, Vector, Vector]:
    points: list[Vector] = []
    for obj in mesh_objects:
        points.extend(obj.matrix_world @ Vector(corner) for corner in obj.bound_box)
    if not points:
        return Vector((0, 0, 0)), Vector((0, 0, 0)), Vector((1, 1, 1))
    min_v = Vector((min(p.x for p in points), min(p.y for p in points), min(p.z for p in points)))
    max_v = Vector((max(p.x for p in points), max(p.y for p in points), max(p.z for p in points)))
    return min_v, max_v, max_v - min_v


def _normalize_scene() -> None:
    mesh_objects = [obj for obj in bpy.context.scene.objects if obj.type == "MESH"]
    if not mesh_objects:
        return
    min_v, max_v, dims = _bounds(mesh_objects)
    center = (min_v + max_v) / 2
    max_dim = max(dims.x, dims.y, dims.z, 0.0001)
    scale = 3.0 / max_dim
    for obj in mesh_objects:
        obj.location = (Vector(obj.location) - center) * scale
        obj.scale = Vector(obj.scale) * scale


def _ensure_stl_materials() -> None:
    mat = bpy.data.materials.new("DS_Neutral_Clay")
    mat.use_nodes = True
    bsdf = mat.node_tree.nodes.get("Principled BSDF")
    if bsdf:
        bsdf.inputs["Base Color"].default_value = (0.42, 0.38, 0.32, 1.0)
        bsdf.inputs["Roughness"].default_value = 0.78
        bsdf.inputs["Metallic"].default_value = 0.05
    for obj in bpy.context.scene.objects:
        if obj.type == "MESH" and len(obj.material_slots) == 0:
            obj.data.materials.append(mat)


def _setup_render(output_path: Path) -> None:
    scene = bpy.context.scene
    available_engines = {item.identifier for item in scene.render.bl_rna.properties["engine"].enum_items}
    scene.render.engine = "BLENDER_EEVEE_NEXT" if "BLENDER_EEVEE_NEXT" in available_engines else "BLENDER_EEVEE"
    if hasattr(scene, "eevee"):
        scene.eevee.taa_render_samples = 32
    scene.render.resolution_x = 900
    scene.render.resolution_y = 900
    scene.view_settings.view_transform = "Filmic"
    scene.view_settings.look = "Medium High Contrast"
    scene.world = bpy.data.worlds.new("DS_PreviewWorld")
    scene.world.color = (0.015, 0.018, 0.018)

    light_data = bpy.data.lights.new("DS_KeyLight", "AREA")
    light_data.energy = 550
    light_data.size = 5
    light = bpy.data.objects.new("DS_KeyLight", light_data)
    bpy.context.collection.objects.link(light)
    light.location = (3.5, -5.0, 4.5)

    fill_data = bpy.data.lights.new("DS_ToxicFill", "POINT")
    fill_data.energy = 85
    fill_data.color = (0.45, 0.9, 0.62)
    fill = bpy.data.objects.new("DS_ToxicFill", fill_data)
    bpy.context.collection.objects.link(fill)
    fill.location = (-3.0, 3.0, 1.0)

    camera_data = bpy.data.cameras.new("DS_PreviewCamera")
    camera = bpy.data.objects.new("DS_PreviewCamera", camera_data)
    bpy.context.collection.objects.link(camera)
    camera.location = (3.2, -5.0, 2.4)
    direction = Vector((0, 0, 0.55)) - Vector(camera.location)
    camera.rotation_euler = direction.to_track_quat("-Z", "Y").to_euler()
    camera_data.lens = 58
    scene.camera = camera
    scene.render.filepath = str(output_path)


def _render_one(output_dir: Path, path: Path) -> str:
    _clear_scene()
    _import_asset(path)
    _ensure_stl_materials()
    _normalize_scene()
    output_path = output_dir / f"{_slug(path.stem)}-preview.png"
    _setup_render(output_path)
    bpy.ops.render.render(write_still=True)
    return str(output_path)


def _argv_after_double_dash() -> list[str]:
    if "--" not in sys.argv:
        return []
    return sys.argv[sys.argv.index("--") + 1 :]


def main() -> int:
    args = _argv_after_double_dash()
    if len(args) < 2:
        print("Usage: <output_dir> <asset paths...>")
        return 1
    output_dir = Path(args[0])
    if not output_dir.is_absolute():
        output_dir = Path(os.environ.get("DS_REPO_ROOT", Path.cwd())) / output_dir
    output_dir.mkdir(parents=True, exist_ok=True)
    for path_text in args[1:]:
        path = Path(path_text)
        try:
            rendered = _render_one(output_dir, path)
            print(f"RENDERED {rendered}")
        except Exception as exc:  # noqa: BLE001 - diagnostic script
            print(f"FAILED {path}: {exc}")
            return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
