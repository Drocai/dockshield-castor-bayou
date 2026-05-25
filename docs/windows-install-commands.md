# Windows Install Commands

Use official GUI installers when in doubt. These commands are convenience shortcuts and should be verified on the machine before running.

## Official Links

- Unreal Engine / Epic Games Launcher: https://www.unrealengine.com/en-US/download
- Unreal install docs: https://dev.epicgames.com/documentation/en-us/unreal-engine/install-unreal-engine
- Visual Studio Unreal setup: https://learn.microsoft.com/en-us/visualstudio/gamedev/unreal/get-started/vs-tools-unreal-install
- Blender: https://www.blender.org/download/
- Git: https://git-scm.com/downloads
- Git LFS: https://git-lfs.com/
- OBS Studio: https://obsproject.com/download
- GitHub Desktop: https://desktop.github.com/download/

## Recommended Manual Install Order

1. Install Epic Games Launcher.
2. Open Epic Games Launcher and install Unreal Engine 5.7.x.
3. Install Visual Studio Community or Build Tools with `Game development with C++`.
4. Install Blender.
5. Install OBS Studio.
6. Install GitHub Desktop if you want a GUI for commits/remotes.
7. Open PowerShell in this repo and run `scripts/check-toolchain.ps1`.

## Winget Options

Run these one at a time from PowerShell:

```powershell
winget install --id Git.Git -e
winget install --id GitHub.GitLFS -e
winget install --id BlenderFoundation.Blender -e
winget install --id OBSProject.OBSStudio -e
winget install --id GitHub.GitHubDesktop -e
winget install --id Microsoft.VisualStudio.2022.Community -e
```

After installing Visual Studio, open **Visual Studio Installer** and confirm the **Game development with C++** workload is selected.

Epic Games Launcher package availability can vary in `winget`. Prefer the official Unreal download page for Unreal.

## Post-Install Verification

From this repo:

```powershell
scripts/check-toolchain.ps1
git lfs install
git status --short
```

