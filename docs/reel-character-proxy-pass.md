# Reel Character Proxy Pass - 2026-05-29

## Purpose

This pass brings The Reel's current best 3D visual candidate into the Unreal test map as a private static scale and lighting proxy.

It is **not** a playable character replacement. The current C++ playable Reel pawn, controls, targeting, boat tow, and rescue gameplay remain the source of truth until a rigged, animation-ready character source exists.

## Source Asset

| Field | Value |
| --- | --- |
| Controlled source path | `SourceAssets/Private/Copilot3D/SM_Reel_Copilot3D_Proxy.glb` |
| Original local source | `C:\Users\djmc1\Downloads\Copilot3D-92f74995-2e02-44ad-ad7c-b5ea100bce10.glb` |
| SHA256 | `3DD8BCC8EDBC8D2EA769E74B4F5C414CC71BB29F933268CF0B0DD2E5B6D4691F` |
| Size | `8,387,188` bytes |
| Blender inspection | 1 mesh, 0 armatures, 0 actions, about 142,970 triangles |
| Usage status | Private prototype/reference proxy only; public/commercial use TBD |

## Unreal Assets

Imported static proxy:

```text
/Game/DockShield/Characters/Reel/Proxy/Copilot3D/
  SM_Reel_Copilot3D_Proxy/
    StaticMeshes/SM_Reel_Copilot3D_Proxy
    Materials/SM_Reel_Copilot3D_Proxy_material_0
    Textures/SM_Reel_Copilot3D_Proxy_texture_0
    Textures/SM_Reel_Copilot3D_Proxy_texture_1
```

Prototype review materials:

```text
/Game/DockShield/Characters/Reel/Materials/M_DS_ReelProxy_WetBlueBlack_Proto
/Game/DockShield/Characters/Reel/Materials/M_DS_ReelProxy_RedBrass_Proto
/Game/DockShield/Characters/Reel/Materials/M_DS_ReelProxy_ToxicGreen_Proto
```

Placed in `/Game/DockShield/Maps/M_Test_Targeting`:

```text
DS_Reel_Proxy_Copilot3D_Static
DS_Reel_Proxy_Review_Base
DS_Reel_Proxy_Reel_Accent
DS_Reel_Proxy_Toxic_Accent
DS_Reel_Proxy_Toxic_Glow
```

## Validation

Automation scripts:

```text
scripts/unreal/apply_reel_proxy_pass.py
scripts/unreal/validate_reel_proxy_pass.py
```

The apply script is resumable: if Unreal has already imported the static mesh, it reuses the existing asset instead of deleting and reimporting the folder.

Validated checks:

- controlled source GLB exists
- controlled source GLB size is `8,387,188` bytes
- controlled source GLB SHA256 matches `3DD8BCC8EDBC8D2EA769E74B4F5C414CC71BB29F933268CF0B0DD2E5B6D4691F`
- imported Unreal static mesh exists
- prototype Reel materials exist
- proxy review actors exist in `M_Test_Targeting`
- static proxy actor has the imported mesh and private reference tags

## Review Checklist

- The proxy reads as The Reel at third-person camera distance.
- The hat, reel hardware, belt, and wet tactical silhouette are legible.
- Toxic-green accent lighting is visible but not dominant.
- The proxy scale is close enough to the playable pawn for future replacement planning.
- The proxy remains labeled as private static reference, not final playable art.

## Next Required Asset Step

Generate or source a riggable The Reel A-pose/T-pose character with separated gear where possible. Use:

```text
docs/prompts/the-reel-asset-generation-prompts.md
```

The prompt labeled `The Reel - Main Riggable Character` is the next source target. The resulting asset should be FBX or GLB with PBR textures, clear front/back/side silhouette, and a rigging route through MetaHuman, AccuRIG/Mixamo, or a custom Blender rig.
