# GitHub Setup

## Recommended Repo

Created private GitHub repository:

```text
Drocai/dockshield-castor-bayou
```

URL: https://github.com/Drocai/dockshield-castor-bayou

## Push Commands

From this repo:

```powershell
git branch -M main
git remote add origin https://github.com/<your-account>/dockshield-castor-bayou.git
git push -u origin main
```

## Repository Settings

Recommended:

- Private repo
- Git LFS enabled by default through local `.gitattributes`
- Branch protection later, after Unreal project exists
- Issues enabled
- Wiki disabled unless needed
- Discussions disabled unless needed

## Collaborator Rules

Every collaborator should:

1. install Git LFS
2. run `git lfs install`
3. read `AGENTS.md`
4. read `docs/00-start-here.md`
5. avoid committing generated caches
6. avoid committing secrets

## First Labels

Create these labels if using GitHub Issues:

- `phase-0-setup`
- `phase-1-unreal`
- `gameplay`
- `art`
- `audio`
- `docs`
- `risk`
- `blocked`
- `playtest`
