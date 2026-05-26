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

## Private Reference Exception

AI-generated or user-provided concept images may be stored in `docs/reference/` only when they are clearly marked as private reference material.

Rules:

- Do not import reference-only images into Unreal `Content/`.
- Do not use reference-only images in public marketing, trailers, store pages, contractor packets, or shipped builds.
- Record each file in `docs/asset-register.csv`.
- Include provenance and checksum details in the relevant reference README.
- Replace reference-only images with licensed, self-created, commissioned, or otherwise cleared production assets before public use.

## Useful Sources

- Fab: https://www.fab.com/
- Poly Haven: https://polyhaven.com/
- MetaHuman: https://www.metahuman.com/
- Unreal Marketplace/Fab free monthly content, only after license check
