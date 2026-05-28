# Character Asset Pipeline Research - 2026-05-27

## Question

How should DockShield produce the first playable-quality hero asset, starting with The Reel, using Unreal Engine, Blender, AI 3D generation, shading, lighting, and texture tools without assuming paid subscriptions?

## Current Repo Reality

- Current playable characters are working prototype pawns, not final character art.
- The current Copilot3D files are useful, but they are static meshes with no armatures or animation actions.
- The best current Reel candidate is `Copilot3D-92f74995-2e02-44ad-ad7c-b5ea100bce10.glb`, but it is a static visual proxy or sculpt reference, not a playable mesh.
- The current visual pass already gives the test map storm fog, toxic glow, wet dock dressing, and prototype bayou materials.

## Research Sources

- Unreal FBX Content Pipeline: https://dev.epicgames.com/documentation/en-us/unreal-engine/fbx-content-pipeline
- Unreal glTF File Format Support: https://dev.epicgames.com/documentation/en-us/unreal-engine/gltf-file-format-support-in-unreal-engine
- Unreal Lumen Global Illumination and Reflections: https://dev.epicgames.com/documentation/en-us/unreal-engine/lumen-global-illumination-and-reflections-in-unreal-engine
- Unreal Physically Based Materials: https://dev.epicgames.com/documentation/en-us/unreal-engine/physically-based-materials-in-unreal-engine
- MetaHuman 5.7 overview: https://www.metahuman.com/en-US
- MetaHuman Creator in Unreal Engine 5.7: https://dev.epicgames.com/documentation/ru-ru/metahuman/metahuman-creator?application_version=5.7
- Mesh to MetaHuman: https://dev.epicgames.com/documentation/en-us/metahuman/mesh-to-metahuman
- Meshy Text to 3D API: https://docs.meshy.ai/api/text-to-3d
- Adobe Mixamo custom character rigging: https://helpx.adobe.com/creative-cloud/help/mixamo-rigging-animation.html
- Reallusion AccuRIG: https://www.reallusion.com/auto-rig/accurig/
- Blender glTF 2.0 import/export manual: https://docs.blender.org/manual/en/5.0/addons/import_export/scene_gltf2.html
- Blender render baking manual: https://docs.blender.org/manual/en/dev/render/cycles/baking.html

## Findings

### 1. Final playable humans should be built around a skeletal mesh path

Unreal's FBX pipeline remains the safest route for animated characters because it supports skeletal meshes, animations, morph targets, materials, and textures in one pipeline. GLB/glTF is useful for material-rich static/source inspection and can work for import/export, but FBX is still the better target for rigged gameplay characters.

Decision: use GLB for source/proxy review, FBX for rigged playable asset import.

### 2. MetaHuman is the highest-quality route for a final human base

MetaHuman 5.7 is now integrated into Unreal Editor and can create rigged high-fidelity humans with hair, clothing, body controls, material controls, and assembly workflows. Mesh to MetaHuman can create an identity from a mesh or footage, but it is mostly a head/identity path, not a one-click conversion of a messy AI full-body sculpt into a fully dressed game character.

Decision: use MetaHuman for The Reel's human base if we want believable face/body/rig quickly. Use generated Copilot3D/Meshy assets for outfit, gear, silhouette, and texture direction.

### 3. AI 3D generation is useful, but it is not the finish line

Meshy supports text-to-3D preview/refine workflow, requested target formats including GLB/OBJ/FBX/STL/USDZ/3MF, and optional PBR map generation. This makes it useful for rapid source assets, props, and outfit concepts. The current Copilot3D files prove the same pattern: fast useful shapes, but no rig and high triangle counts.

Decision: ask generators for A-pose/T-pose, separated gear, GLB/FBX, PBR textures, and no action pose. Treat outputs as source candidates until inspected.

### 4. Auto-rigging can help, but input posture matters

Mixamo can upload and rig custom 3D characters through its online service. AccuRIG can rig biped characters and export FBX/USD for Unreal/Blender-type pipelines. Both need a humanoid mesh that is clean enough to rig. Current Reel/Fly assets have props, heavy gear, and non-neutral poses, so they are risky direct auto-rig inputs.

Decision: if using auto-rigging, generate The Reel in a neutral A-pose with separated or removable hard-surface props. Keep fishing rod, reel gauntlet, bobbers, ropes, and cloak as separate meshes where possible.

### 5. Shading and lighting should be authored in Unreal, not baked into prompts

The reference look depends on storm lighting, wet materials, fog, toxic green accents, red/amber warning lights, and reflective water. Unreal Lumen, PBR materials, post-process, fog, and material instances are the correct place to tune that. AI prompts should request physically plausible materials and texture maps, not final scene lighting baked into the character texture.

Decision: prompt for neutral studio asset output; do the cinematic bayou lighting in Unreal.

## Recommended Reel-First Pipeline

1. Generate or select **The Reel A-pose source body**.
2. Generate **separate Reel gear props**:
   - tactical reel gauntlet
   - fishing rod / line launcher
   - bobber bomb
   - life ring launcher
   - rescue raft pack
   - belt pouches, hooks, lures, rope, rescue floats
3. Inspect every output with Blender and record:
   - SHA256
   - triangle count
   - material count
   - texture count
   - armature/action count
   - dimensions
   - verdict
4. Bring the best source into Blender for cleanup:
   - scale/origin
   - separate props if possible
   - material slot cleanup
   - UV/texture check
   - retopo decision
5. Rig path:
   - Best quality: MetaHuman body/face plus custom outfit and gear.
   - Fast prototype: AccuRIG/Mixamo auto-rig on a clean A-pose mesh.
   - Do not rig action-pose static sculpts unless they are only for statues/proxies.
6. Unreal path:
   - import as static proxy first for scale/lighting
   - import rigged FBX later as skeletal mesh
   - retarget to existing Manny/MetaHuman animation path only after rig quality is acceptable
7. Validate:
   - open in `M_Test_Targeting`
   - inspect silhouette at gameplay camera distance
   - inspect storm/fog/toxic lighting readability
   - confirm no gameplay regression with `scripts\unreal\validate-prototype.ps1`

## What To Generate Next

Generate more assets only if they are specific pipeline inputs:

- The Reel neutral full-body A-pose, no scene, no action pose.
- The Reel front/side/back turnaround sheet.
- Reel tactical gauntlet prop.
- Reel rod/line launcher prop.
- Reel rescue equipment prop pack.

Do not generate more cinematic posed full scenes for playable asset production. Those are good for reference, but bad for rigging.

## Decision

The next concrete build pass should not chase final graphics immediately. It should produce a clean Reel source package:

```text
The Reel A-pose character source
The Reel gear prop source
Blender inspection manifest
Static Unreal proxy review
Rigging route decision
```

That is the shortest path from cool images to a real playable character.
