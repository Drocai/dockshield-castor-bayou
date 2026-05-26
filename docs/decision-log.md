# Decision Log

Use this file for decisions that should not be reopened casually.

| Date | Decision | Reason | Revisit Trigger |
| --- | --- | --- | --- |
| 2026-05-25 | DockShield is the platform/game title | It supports missions, systems, and future waterway operations better than a one-off mission title | Trademark conflict or stronger verified title |
| 2026-05-25 | The Depth is the squad | Keeps the hero team separate from the operation system | Major story restructure |
| 2026-05-25 | Castor Bayou Incident is the first vertical slice | Gives a concrete first mission with weather, water, docks, and contamination | If another existing prototype is far more advanced |
| 2026-05-25 | Start with three verbs: Pull, Mark, Bind | This proves the unified targeting hook without building every ability | If one verb fails in playtest |
| 2026-05-25 | Use Unreal Engine as target engine | Best fit for cinematic third-person action, Blueprints, water/weather visuals, and marketplace support | If existing prototypes are already much stronger in another engine |
| 2026-05-25 | Do not generate Unreal files before Unreal is installed | Avoids fake project state and broken assets | Unreal project is created intentionally |
| 2026-05-25 | Do not silently disable Smart App Control | It is a system security setting and may require a Windows reset or reinstall to turn back on | D RoC explicitly accepts that risk or chooses a separate build machine |
| 2026-05-25 | Smart App Control was turned off by D RoC for Unreal C++ builds | UnrealBuildTool must load generated unsigned build-rules DLLs for local C++ builds | If a separate build machine or signing path is introduced |
