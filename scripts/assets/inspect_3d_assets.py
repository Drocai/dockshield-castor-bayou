"""Inspect candidate GLB/STL assets with Blender and print JSON results.

Usage from repo root:
  blender --background --python scripts/assets/inspect_3d_assets.py -- <paths...>
"""

from __future__ import annotations

import hashlib
import json
import os
import sys
from pathlib import Path

import bpy
from mathutils import Vector


def _clear_scene() -> None:
    bpy.ops.object.select_all(action="SELECT")
    bpy.ops.object.delete()


def _sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as handle:
        for chunk in iter(lambda: handle.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


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


def _bounds_for_objects(objects: list[bpy.types.Object]) -> tuple[list[float], list[float], list[float]]:
    points: list[Vector] = []
    for obj in objects:
        if obj.type != "MESH":
            continue
        points.extend(obj.matrix_world @ Vector(corner) for corner in obj.bound_box)

    if not points:
        return [0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0]

    min_v = Vector((min(p.x for p in points), min(p.y for p in points), min(p.z for p in points)))
    max_v = Vector((max(p.x for p in points), max(p.y for p in points), max(p.z for p in points)))
    dims = max_v - min_v
    return [round(v, 4) for v in min_v], [round(v, 4) for v in max_v], [round(v, 4) for v in dims]


def _inspect(path_text: str) -> dict:
    path = Path(path_text)
    result: dict = {
        "path": str(path),
        "name": path.name,
        "extension": path.suffix.lower(),
        "exists": path.exists(),
    }
    if not path.exists():
        result["status"] = "missing"
        return result

    result["size_bytes"] = path.stat().st_size
    result["sha256"] = _sha256(path)

    _clear_scene()
    try:
        _import_asset(path)
    except Exception as exc:  # noqa: BLE001 - diagnostic script
        result["status"] = "import_failed"
        result["error"] = str(exc)
        return result

    objects = list(bpy.context.scene.objects)
    mesh_objects = [obj for obj in objects if obj.type == "MESH"]
    armatures = [obj for obj in objects if obj.type == "ARMATURE"]
    cameras = [obj for obj in objects if obj.type == "CAMERA"]
    lights = [obj for obj in objects if obj.type == "LIGHT"]

    vertex_count = 0
    triangle_count = 0
    material_names: set[str] = set()
    texture_images: set[str] = set()
    mesh_names: list[str] = []

    for obj in mesh_objects:
        mesh_names.append(obj.name)
        mesh = obj.data
        vertex_count += len(mesh.vertices)
        mesh.calc_loop_triangles()
        triangle_count += len(mesh.loop_triangles)
        for slot in obj.material_slots:
            if not slot.material:
                continue
            material_names.add(slot.material.name)
            if slot.material.use_nodes and slot.material.node_tree:
                for node in slot.material.node_tree.nodes:
                    if node.bl_idname == "ShaderNodeTexImage" and getattr(node, "image", None):
                        texture_images.add(node.image.name)

    bounds_min, bounds_max, dimensions = _bounds_for_objects(mesh_objects)

    result.update(
        {
            "status": "ok",
            "object_count": len(objects),
            "mesh_count": len(mesh_objects),
            "armature_count": len(armatures),
            "camera_count": len(cameras),
            "light_count": len(lights),
            "action_count": len(bpy.data.actions),
            "vertex_count": vertex_count,
            "triangle_count": triangle_count,
            "material_count": len(material_names),
            "texture_image_count": len(texture_images),
            "bounds_min": bounds_min,
            "bounds_max": bounds_max,
            "dimensions": dimensions,
            "mesh_names": mesh_names[:20],
            "material_names": sorted(material_names)[:20],
            "texture_images": sorted(texture_images)[:20],
        }
    )
    return result


def _argv_after_double_dash() -> list[str]:
    if "--" not in sys.argv:
        return []
    return sys.argv[sys.argv.index("--") + 1 :]


def main() -> int:
    paths = _argv_after_double_dash()
    if not paths:
        print(json.dumps({"status": "error", "message": "No asset paths supplied."}))
        return 1

    results = [_inspect(path) for path in paths]
    print("DS_ASSET_INSPECTION_JSON_BEGIN")
    print(json.dumps(results, indent=2))
    print("DS_ASSET_INSPECTION_JSON_END")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
