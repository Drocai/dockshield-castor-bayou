# Risk Register

| Risk | Severity | Likelihood | Mitigation | Owner |
| --- | --- | --- | --- | --- |
| Scope expands beyond a playable vertical slice | High | High | Keep first build limited to Pull, Mark, Bind and one mission path | Producer |
| Unreal project becomes binary-heavy before source control discipline exists | High | Medium | Use Git LFS immediately and consider Perforce later | Technical lead |
| AI agents create fake or unusable Unreal files | High | Medium | Require repo truth pass and forbid hand-created `.uasset`/`.umap` files | Codex |
| Asset licensing becomes unclear | High | Medium | Record every asset in asset register before production use | Art lead |
| Weather system becomes cosmetic instead of gameplay | Medium | Medium | First weather state must alter target visibility, range, or hazard behavior | Design lead |
| Three heroes feel like reskins | High | Medium | Enforce separate verbs: Pull, Mark, Bind | Design lead |
| Command mode becomes too large too early | High | High | First command mode is slow-time one-action queue only | Gameplay lead |
| $10,000 is spent on polish before feel is proven | High | Medium | Spend first on targeting and graybox gameplay | Producer |
| Hardware cannot handle Unreal comfortably | Medium | Unknown | Run local hardware/toolchain checks before heavy content | D RoC |
| Trademark/title conflict | Medium | Unknown | Perform title search before public launch or store page | Producer |
| Smart App Control blocks Unreal C++ builds if re-enabled | High | Medium | Keep the current setting documented; rerun `scripts/check-toolchain.ps1` and a C++ build after Windows security changes | D RoC |
| Local Visual Studio version/SDK warning causes future Unreal editor integration trouble | Low | Medium | Keep using commandlet/build validation now; install Epic-preferred VS toolchain/SDK if packaging or editor integration starts failing | Technical lead |
| Static proxy character gets mistaken for playable/final character art | High | High | Label Copilot3D imports as private static proxies, keep playable pawn unchanged, and require rig/retopo route before replacement | Art lead |
| Private GLB source drifts after import | Medium | Medium | Validate source file size and SHA256 before applying or validating the Reel proxy pass | Technical lead |
| Packaged build is delayed because validation stays editor-only | Medium | Medium | Add packaged Windows build only after enemy and mission success path exist; keep commandlet validation green first | Technical lead |
| Visual polish outruns asset rights review | High | Medium | Keep reference/proxy assets private until commercial/public rights are cleared or assets are rebuilt as owned production art | Producer |
