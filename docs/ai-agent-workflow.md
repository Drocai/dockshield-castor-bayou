# AI Agent Workflow

## Agent Roles

Use one agent as the active editor at a time.

Recommended default:

- Codex: repo operator, implementation, verification, docs updates
- Claude Code: second-pass review or isolated branch work
- Gemini/ChatGPT: research, critique, design alternatives, long-context synthesis

## Rules

- Do not let two agents edit the same branch at the same time.
- Every agent must read `AGENTS.md`, `README.md`, and `docs/00-start-here.md`.
- Every implementation task must start with repo truth.
- Every code task must end with verification.
- No agent may claim Unreal build success unless the editor/package command actually ran.

## Good Codex Task Shape

Use small, concrete prompts:

```text
Inspect the DockShield repo and summarize the current Unreal project structure. Do not edit files yet.
```

```text
Create the Unreal Content folder plan as documentation only. Do not create fake .uasset files.
```

```text
Implement the shared targeting interface for grapple points, enemies, civilians, hazards, and weak points. Keep the change scoped and verify in editor.
```

## Bad Task Shape

Avoid:

```text
Build the whole DockShield game.
```

```text
Make it AAA.
```

```text
Add procedural maps, weather, three heroes, boss fights, and cinematics all at once.
```

## First Codex Prompts

### Prompt 1 - Repo Truth

```text
Read AGENTS.md, README.md, and docs/00-start-here.md. Summarize current repo reality, missing toolchain pieces, and the next safest setup step. Do not edit files.
```

### Prompt 2 - Unreal Project Intake

```text
An Unreal project has now been created. Inspect the .uproject, Config folder, Content folder, and any Source folder. Summarize the actual generated structure and recommend the smallest next setup change. Do not edit files.
```

### Prompt 3 - Shared Targeting Design

```text
Design the first implementation pass for DockShield's shared targeting system inside the existing Unreal project. It must support Grapple Point, Enemy, Civilian, Object, Weak Point, Hazard, and Rescue Target. Keep the design implementable in one focused pass.
```

### Prompt 4 - Implement One Playable Verb

```text
Implement The Reel's prototype Pull ability using the existing shared targeting system. It only needs to work on one grapple point and one civilian rescue target. Verify in editor or report the exact blocker.
```

