# Copilot3D Candidate Asset Audit - 2026-05-27

These files are user-provided Copilot3D candidate assets for DockShield hero visual development.

They are **not cleared production assets yet**. Keep them in the private reference/source-candidate lane until the license, usage rights, optimization path, and Unreal import quality are confirmed.

## Audit Result

- All nine named files import successfully in Blender 5.1.
- The nine files reduce to five unique meshes by SHA256.
- The GLB files are more useful than the STL files because they include embedded textures.
- None of the files include armatures or animation actions, so they are not drop-in playable characters.
- Triangle counts are high for direct gameplay use: roughly 140k-148k triangles per mesh.
- Best immediate use is visual proxy, sculpt/reference, or kitbash source for The Reel and The Fly.

## Contact Sheet

![Copilot3D candidate contact sheet](contact-sheet.png)

## Highest-Value Candidates

| Candidate | Files | Use | Notes |
| --- | --- | --- | --- |
| The Reel side textured proxy | `Copilot3D-92f74995-2e02-44ad-ad7c-b5ea100bce10.glb` or duplicate `Copilot3D-ee3787ee-edbe-46a6-ae7a-b42ada533958.glb` | Best immediate Reel visual reference/proxy | Textured and readable; no rig; needs cleanup/retopo before gameplay character use |
| Front staff textured proxy | `Copilot3D-86d88bdd-1eb9-4934-832a-2183614db136.glb` or duplicate `Copilot3D-c429a291-f263-49d9-9b46-22d55aa72653.glb` | The Fly or gadget-heavy hero proxy | Textured, front-readable silhouette; no rig |
| Back staff textured proxy | `Copilot3D-913e8f57-8b9e-490d-ba31-711d67f2766c.glb` or duplicate `Copilot3D-2610fb6a-c345-4409-a798-1ea8050a06f8.glb` | Back silhouette and gear reference | Good for cloak/gear read; no rig |
| Winged staff STL | `Copilot3D-4753f91f-69c3-4616-a805-9d2f1c0b9218.stl` | Fly/creature sculpt reference | Interesting form but untextured STL |
| Reel side STL | `Copilot3D-cae7c791-4abd-4957-b3bc-7ee23f88724d.stl` or duplicate `Copilot3D-6ac8f255-f124-4fa1-8ed8-b43797038a5c.stl` | Sculpt backup only | Lower priority because the GLB duplicate family is textured |

## Recommended Next Step

Do not replace the playable Manny/Quinn-derived pawns with these yet.

Next production-safe pass:

1. Copy only the best unique GLB candidate into a controlled `SourceAssets/Private/Copilot3D/` folder if the license is acceptable.
2. Import it into Unreal as a **static visual proxy** under `/Game/DockShield/Characters/Reel/Proxy/`.
3. Place it next to the current playable Reel pawn in `M_Test_Targeting` for scale and lighting review.
4. Use Blender to extract usable accessories or silhouette guidance for the animated playable character.
5. Retopologize, rig, and texture a proper playable character later; these files are not rigged for direct animation.

## Supporting Files

- `manifest.csv`: source file, checksum, duplicate group, mesh metrics, and candidate verdict.
- `previews/`: generated neutral preview renders for the nine user-provided files.
