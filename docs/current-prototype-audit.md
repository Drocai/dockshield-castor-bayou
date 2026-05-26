# Current Prototype Audit

Date: 2026-05-26

## Bottom Line

Do not expect the current build to look like the canon gameplay images yet. The project is currently a graybox Unreal prototype proving targeting, Reel Pull, and HUD readability. The canon images define the intended movie/game look, but the production art pipeline has not started.

## Ten Problems Found

| # | Problem | Current Fix / Disposition |
| ---: | --- | --- |
| 1 | The current map is graybox geometry, not Castor Bayou production art. | Logged as the biggest expectation risk; next visual pass must build a bayou dock graybox art direction lane. |
| 2 | The player is still Unreal Manny, not The Reel character from the canon images. | Keep as placeholder for mechanics; next character task is a Reel silhouette/gear proxy before final model work. |
| 3 | The canon images were labeled too weakly as references. | Reclassify them as the private Canon Visual Target Pack while still not treating raw PNGs as shipping assets. |
| 4 | There was no dedicated "how to play this exact prototype" doc. | Add a current play guide so expectations are clear before opening Unreal. |
| 5 | Existing commandlet validation checked map actors but not gameplay/HUD wiring. | Add a runtime validation script for game mode, pawn, HUD, target components, and prompts. |
| 6 | The HUD is functional but not cinematic. | Keep the Canvas HUD as v0 only; do not call it final UI. |
| 7 | Targeting range ignored each target component's `InteractionRange`. | Update target selection to respect per-target range. |
| 8 | HUD prompt text could overflow if prompts get longer. | Clamp prototype prompt text before drawing. |
| 9 | The Reel Pull is built but still lacks interactive PIE proof. | Keep GitHub issue `#7` open until in-editor smoke test confirms repeated Pull/rescue. |
| 10 | Playwright cannot playtest Unreal directly. | Use Unreal build, Python commandlets, DataValidation, and later packaged build/OBS capture instead. |

## Graphics Reality

Current graphics are prototype quality:

- Third Person template character
- test-map graybox floor/cubes
- debug target line/spheres
- simple Canvas HUD
- first test-map lighting/fog pass only
- no final storm/rain/water visuals
- no custom Reel/Fly/Lilly models
- no cinematic props or VFX

Target graphics are defined by `docs/reference/gameplay-ui/` and should drive the art direction from here.

## Do Not Show Publicly Yet

This is not trailer, pitch, store-page, or audience-ready footage. It is a mechanics test.
