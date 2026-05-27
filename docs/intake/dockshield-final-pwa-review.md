# DockShield Final PWA Intake Review

Date: 2026-05-27

## Source Artifact

`C:\Users\djmc1\Downloads\dockshield_final_pwa.zip`

Archive SHA256:

```text
C8E4F9EB12CF2616DC31B9847C71EEB0C97D2109E2D0673C1D342F18F4FD34FE
```

Review extraction path:

```text
C:\Users\djmc1\AppData\Local\Temp\dockshield-pwa-review-20260527-091611
```

This extraction is for review only. Nothing from the PWA has been imported into the production Unreal project.

## Contents

| File | Notes |
| --- | --- |
| `index.html` | Single-file mobile/desktop Three.js prototype, 102 KB, 1309 lines |
| `README.txt` | Control summary and launch note |
| `manifest.webmanifest` | PWA metadata for `DockShield: Swamp Reel` |
| `sw.js` | Small service worker cache |
| `start_bg.jpg` | 1536x864 title/background image |
| `portrait_reel.png` | 96x64 Reel portrait |
| `portrait_fly.png` | 96x64 Fly portrait |
| `portrait_lilly.png` | 96x64 Lilly portrait |
| `icon-192.png`, `icon-512.png` | PWA icons |

## What Is Useful

The PWA is valuable as a mechanics sketch and design reference. It should not be ported directly, but several systems should be translated into Unreal:

- Reeling model: cast, hold reel, line tension, tension color states, stamina drain, snap risk, and swing-release impulse.
- Hero differentiation: The Reel uses winch/shockwave, The Fly uses sonar/marking/stealth, Lilly uses vine/root/heal/swamp bloom.
- Gadget inventory: grapple, rescue hook, cutting line, bobber bomb, medfoam, flare, reinforced line, lure, precision reel, heavy line, fast reel.
- Flood/weather model: water level rises over time, storm intensity increases, rain density changes, lightning spikes pressure, water has animated wave motion.
- Airboat model: board/dismount, rider follows boat, boat uses movement input, objective/HUD reports boat state.
- Rescue/evidence loop: civilians, witnesses, toxin samples, boss defeat, mission grade, salvage/upgrades.
- HUD design concepts: hero badge/portrait, HP/stamina/flood/boss bars, objective stack, radio log, gadget dock, target label, minimap, line-tension reticle color.
- Save-loop concept: local run snapshot with hero state, inventory, rescued targets, evidence, boat, boss, and active hero.

## What Should Not Be Copied Directly

- The HTML/JavaScript architecture is not suitable for the Unreal repo. Most gameplay is in one file and later features are layered through prototype monkey patches.
- The raw images need provenance and license confirmation before they are treated as shippable assets, even though they are valid canon direction for this project.
- The PWA depends on CDN Tailwind, CDN Three.js, and Google Fonts. That is fine for a web sketch but not relevant to the UE 5.7 prototype.
- The service worker does not cache the background or portrait images, so its offline install behavior is incomplete.
- The UI look is useful for tactical readability, but the cinematic Castor Bayou target remains the private canon reference pack in `docs/reference/gameplay-ui/`.

## Review Checks Run

```powershell
Zip path safety check passed
node --check inline-script-for-review.js
rg secret/API-key scan
chrome --headless screenshot of start screen
```

Findings:

- No zip path traversal found.
- Inline JavaScript passed `node --check`.
- No obvious secrets or API keys found by pattern scan.
- Chrome produced a 1366x768 start-screen screenshot successfully.

## Recommended Unreal Translation Order

1. Extend the current Reel prototype with a real line-tension state machine: safe, strain, danger, snap.
2. Add cast/reel separation to input: cast attaches line, hold reel applies pull over time, release stops pull.
3. Add water current and flood pressure to `ADSWaterZoneActor` / boat logic.
4. Promote current boat boarding into a clearer rescue raft/airboat prototype with acceleration, drift, and exit safety.
5. Add a lightweight objective manager for rescue count, evidence count, boat state, and boss placeholder state.
6. Add The Fly placeholder next using the PWA sonar/mark/stun behavior as the first translated non-Reel hero mechanic.

Current translation note: items 1 and 2 are now represented in C++ as The Reel continuous line v0: LMB/`E` casts or detaches, `R` reels while held, release eases tension, and sustained strain can snap the line. Item 3 has started with water-zone current, flood pressure, flood-surge rise, Reel tension pressure, and boat drift/drag hooks.

## Bottom Line

Use this PWA as a playable design notebook. It contains strong gameplay DNA for DockShield, especially reeling, hero roles, storm pressure, airboat behavior, HUD density, and mission-loop structure. Do not import it wholesale into Unreal. Translate its best ideas into small, validated UE systems.
