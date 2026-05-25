# AGENTS.md

## Project Operating Rules

This repo belongs to D RoC's creative/technical ecosystem. Treat the repo as the source of truth. Do not invent architecture before reading files.

## First Step Every Session

Before editing:
- Read `README.md`
- Read this `AGENTS.md`
- Inspect `docs/00-start-here.md`
- Inspect `docs/toolchain-setup.md`
- Identify the current project phase
- Identify available scripts, if any
- Summarize repo reality before changing code

## Current Phase

This is a planning/control repo for **DockShield: Castor Bayou Incident**.

There is no Unreal `.uproject` yet. Do not fabricate Unreal source, Blueprints, assets, maps, or generated project files until Unreal Engine is installed and the project has been created intentionally.

## Coding Standards

- Use the existing style.
- Prefer minimal, targeted diffs.
- Do not rewrite working systems unless the task requires it.
- Preserve existing routes, names, content, assets, prompts, and brand language.
- Do not add new packages unless necessary.
- Keep code typed and readable when code exists.
- Avoid speculative abstractions.

## Security

- Never commit secrets.
- Never expose server keys to client code.
- Use environment variables when services are introduced.
- Update `.env.example` with placeholders only.
- Flag exposed credentials immediately.
- Recommend key rotation when secrets appear in repo history or source.

## Validation

For this planning repo, validation means:
- `git status --short`
- confirm Git LFS patterns with `.gitattributes`
- review changed docs for broken links, placeholders, and contradictions

When the Unreal project exists, prefer:

```bash
git status --short
git lfs track
```

Then use Unreal's editor/package/build validation as documented in `docs/toolchain-setup.md`.

## Final Response Format

```txt
Changed:
- path: change

Verified:
- command: result

Risks:
- risk or None found

Next:
- exact next command/action
```

## Project Notes

Public-facing artist name: D RoC.

Do not use private/internal names in public-facing copy unless the task is internal infrastructure and the repo already uses that language.

Working title hierarchy:

- Game/platform title: DockShield
- First campaign: DockShield: The Depth
- First vertical slice: DockShield: Castor Bayou Incident
- First mission arc/signature phrase: Hook, Line & Sinker

