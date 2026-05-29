# Asset Sourcing and Licensing

## Rule

Every asset must have a known source and license before it enters production.

## Approved Source Types

- self-created assets
- official Unreal template/starter assets
- Fab assets with license recorded
- CC0 assets from sources like Poly Haven
- commissioned assets with written rights

## Asset Intake Checklist

For each asset, record:

- asset name
- source URL
- creator/vendor
- license type
- commercial use allowed: yes/no
- modification allowed: yes/no
- attribution required: yes/no
- date acquired
- notes

## Asset Log Template

```text
Asset:
Source:
Creator/Vendor:
License:
Commercial Use:
Modification:
Attribution:
Date Acquired:
Used In:
Notes:
```

## Do Not Use

- random Google Images
- unlicensed ripped game assets
- unclear Sketchfab assets
- AI-generated assets with unclear commercial rights
- assets from marketplace accounts that are not controlled by the project owner

## Private Canon Visual Target Exception

AI-generated or user-provided canon visual target images may be stored in `docs/reference/` only when they are clearly marked as private internal visual targets.

Rules:

- Do not import raw target images into Unreal `Content/`.
- Do not use raw target images in public marketing, trailers, store pages, contractor packets, or shipped builds.
- Record each file in `docs/asset-register.csv`.
- Include provenance and checksum details in the relevant reference README.
- Rebuild target images as licensed, self-created, commissioned, or otherwise cleared production assets before public use.

## Private 3D Candidate Asset Exception

User-provided GLB, glTF, FBX, STL, OBJ, or Blender files may be inspected as private source candidates before production use.

Rules:

- Do not import candidate 3D files into Unreal `Content/` for playable, public, trailer, or shipping use until the license and technical fitness are recorded.
- A candidate may be imported into Unreal `Content/` as a **private static proxy only** when provenance, checksum, file size, technical verdict, and usage limits are documented before commit.
- Do not use candidate 3D files in public marketing, trailers, store pages, contractor packets, or shipped builds until rights are cleared.
- Record each candidate or candidate family in `docs/asset-register.csv`.
- Include source path, checksum, duplicate status, mesh metrics, and import verdict in the relevant reference README or manifest.
- Prefer GLB/glTF/FBX with materials and textures over STL for game prototypes.
- Treat unrigged character meshes as static visual proxies or sculpt references only until retopology and rigging are complete.

## Useful Sources

- Fab: https://www.fab.com/
- Poly Haven: https://polyhaven.com/
- MetaHuman: https://www.metahuman.com/
- Unreal Marketplace/Fab free monthly content, only after license check
