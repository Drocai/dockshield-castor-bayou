# Toolchain Setup

Use official sources. Avoid third-party download mirrors.

## Required Installs

### 1. Git and Git LFS

Already detected locally:

- Git
- Git LFS

Useful links:

- Git: https://git-scm.com/downloads
- Git LFS: https://git-lfs.com/

After cloning this repo on any machine, run:

```powershell
git lfs install
git lfs track
```

### 2. Epic Games Launcher and Unreal Engine 5.7

Install through Epic's official launcher.

- Unreal download page: https://www.unrealengine.com/en-US/download
- Epic install docs: https://dev.epicgames.com/documentation/en-us/unreal-engine/install-unreal-engine
- Unreal Engine 5.7 announcement: https://www.unrealengine.com/news/unreal-engine-5-7-is-now-available

Install:

- Epic Games Launcher
- Unreal Engine 5.7.x
- Starter Content if available
- Templates and feature packs if prompted

Recommended project settings for the first slice:

- Template: Third Person
- Blueprint-first, C++ enabled only if needed
- Target platform: Desktop
- Quality preset: Scalable during prototyping
- Starter Content: yes if available
- Ray tracing: off for first prototype unless hardware is strong

### 3. Visual Studio C++ Tools

Install Visual Studio Community or Build Tools with Unreal support.

- Visual Studio game development: https://visualstudio.microsoft.com/vs/features/game-development/
- Microsoft Unreal setup docs: https://learn.microsoft.com/en-us/visualstudio/gamedev/unreal/get-started/vs-tools-unreal-install

Select workload:

- Game development with C++

Include:

- MSVC build tools
- Windows SDK
- Visual Studio Tools for Unreal Engine
- C++ profiling tools if available

### 4. Blender

Install Blender from the official source.

- Blender download: https://www.blender.org/download/

Use Blender for:

- dock props
- fishing gear props
- collision cleanup
- simple enemy kitbash
- asset export to FBX or glTF

### 5. OBS Studio

Install for capture, playtest evidence, and trailer clips.

- OBS download: https://obsproject.com/download

Use it to capture:

- first playable runs
- bug reproduction
- trailer footage

## Optional Later

### Perforce P4

Use only if the Unreal binary assets outgrow Git/Git LFS or a small team starts working with many `.uasset` and `.umap` files.

- P4 free version control: https://www.perforce.com/products/helix-core/free-version-control
- P4V install docs: https://help.perforce.com/helix-core/server-apps/p4v/current/Content/P4V/install-p4v.html

### MetaHuman

Use for placeholder human characters if it saves time.

- MetaHuman license: https://www.metahuman.com/license
- MetaHuman Creator docs: https://dev.epicgames.com/documentation/en-us/metahuman/metahuman-creator

### Free/Low-Cost Assets

Track licenses for every asset.

- Fab: https://www.fab.com/
- Poly Haven CC0 assets: https://polyhaven.com/
- Rokoko Vision for short mocap tests: https://www.rokoko.com/products/vision

## Local Hardware Check

Before serious Unreal work, confirm:

- Windows 10/11
- 32 GB RAM preferred
- modern dedicated GPU preferred
- 100 GB free disk space minimum
- SSD strongly preferred

## Current Local Validation

From the repo root, run:

```powershell
powershell -ExecutionPolicy Bypass -File scripts\unreal\validate-prototype.ps1
```

This wraps the current setup check, DockShield editor build, targeting map validation, visual style validation, Reel proxy validation, prototype runtime validation, Reel interaction smoke tests, water/boat smoke tests, Unreal DataValidation, Git status, and Git LFS status.

For a committed handoff checkpoint, run:

```powershell
powershell -ExecutionPolicy Bypass -File scripts\unreal\validate-prototype.ps1 -RequireCleanGit
```

## Known Local Warnings

The local Unreal build currently reports Visual Studio 2026 as a non-preferred compiler version and may warn that the Visual Studio SDK is missing for editor integration. These are tracked toolchain warnings, not current prototype blockers, because the DockShield editor target still builds and commandlet validation passes.

## First Unreal Project Creation

After installs:

1. Open Epic Games Launcher.
2. Install Unreal Engine 5.7.x.
3. Create a Third Person project.
4. Name it `DockShield`.
5. Put it in this repo only after confirming the folder layout.
6. Restart Codex and ask it to inspect the generated `.uproject` before editing anything.
