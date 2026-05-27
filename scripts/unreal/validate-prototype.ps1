param(
    [string]$UnrealRoot = "C:\Program Files\Epic Games\UE_5.7",
    [switch]$SkipBuild,
    [switch]$SkipToolchain,
    [switch]$SkipDataValidation
)

$ErrorActionPreference = "Stop"

$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$RepoRoot = (Resolve-Path (Join-Path $ScriptDir "..\..")).Path
$ProjectPath = Join-Path $RepoRoot "DockShield\DockShield.uproject"
$BuildBat = Join-Path $UnrealRoot "Engine\Build\BatchFiles\Build.bat"
$EditorCmd = Join-Path $UnrealRoot "Engine\Binaries\Win64\UnrealEditor-Cmd.exe"
$ToolchainScript = Join-Path $RepoRoot "scripts\check-toolchain.ps1"

$PythonValidations = @(
    "validate_targeting_test_map.py",
    "validate_prototype_runtime.py",
    "validate_reel_interaction_smoke.py",
    "validate_water_boat_reel_smoke.py",
    "validate_fly_recon_smoke.py"
)

function Invoke-Step {
    param(
        [string]$Name,
        [scriptblock]$Command
    )

    Write-Host ""
    Write-Host "== $Name ==" -ForegroundColor Cyan
    & $Command
    Write-Host "[OK] $Name" -ForegroundColor Green
}

function Invoke-Native {
    param(
        [string]$FilePath,
        [string[]]$Arguments
    )

    & $FilePath @Arguments
    if ($LASTEXITCODE -ne 0) {
        throw "$FilePath failed with exit code $LASTEXITCODE"
    }
}

if (-not (Test-Path $ProjectPath)) {
    throw "Missing Unreal project: $ProjectPath"
}

if (-not (Test-Path $EditorCmd)) {
    throw "Missing UnrealEditor-Cmd.exe: $EditorCmd"
}

if (-not $SkipBuild -and -not (Test-Path $BuildBat)) {
    throw "Missing Unreal Build.bat: $BuildBat"
}

if (-not $SkipToolchain) {
    Invoke-Step "Toolchain check" {
        Invoke-Native "powershell" @("-ExecutionPolicy", "Bypass", "-File", $ToolchainScript)
    }
}

if (-not $SkipBuild) {
    $OpenEditor = Get-Process UnrealEditor -ErrorAction SilentlyContinue
    if ($OpenEditor) {
        throw "UnrealEditor is open. Close it before running the C++ build, or rerun with -SkipBuild after compiling in the editor."
    }

    Invoke-Step "DockShieldEditor C++ build" {
        Invoke-Native $BuildBat @("DockShieldEditor", "Win64", "Development", "-Project=$ProjectPath", "-WaitMutex")
    }
}

foreach ($Validation in $PythonValidations) {
    $ValidationPath = Join-Path $ScriptDir $Validation
    if (-not (Test-Path $ValidationPath)) {
        throw "Missing validation script: $ValidationPath"
    }

    Invoke-Step $Validation {
        Invoke-Native $EditorCmd @($ProjectPath, "-run=pythonscript", "-script=$ValidationPath", "-unattended", "-nop4", "-nosplash")
    }
}

if (-not $SkipDataValidation) {
    Invoke-Step "Unreal DataValidation" {
        Invoke-Native $EditorCmd @($ProjectPath, "-run=DataValidation", "-unattended", "-nop4", "-nosplash")
    }
}

Invoke-Step "Git status" {
    Invoke-Native "git" @("-C", $RepoRoot, "status", "--short", "--branch", "--ignored")
}

Invoke-Step "Git LFS status" {
    Invoke-Native "git" @("-C", $RepoRoot, "lfs", "status")
}

Write-Host ""
Write-Host "DockShield prototype validation complete." -ForegroundColor Green
