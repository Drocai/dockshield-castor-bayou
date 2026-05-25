$ErrorActionPreference = "SilentlyContinue"

function Show-Check {
    param(
        [string]$Name,
        [bool]$Found,
        [string]$Detail,
        [string]$Install
    )

    if ($Found) {
        Write-Host "[OK]   $Name - $Detail"
    }
    else {
        Write-Host "[MISS] $Name - $Install"
    }
}

$git = Get-Command git
Show-Check "Git" ($null -ne $git) ($git.Source) "Install: https://git-scm.com/downloads"

$gitLfs = Get-Command git-lfs
Show-Check "Git LFS" ($null -ne $gitLfs) ($gitLfs.Source) "Install: https://git-lfs.com/"

$blender = Get-Command blender
Show-Check "Blender" ($null -ne $blender) ($blender.Source) "Install: https://www.blender.org/download/"

$obs = Get-Command obs64
if ($null -eq $obs) {
    $obs = Get-Command obs
}
Show-Check "OBS Studio" ($null -ne $obs) ($obs.Source) "Install: https://obsproject.com/download"

$epicDirs = Get-ChildItem "C:\Program Files\Epic Games" -Directory
$ueDirs = $epicDirs | Where-Object { $_.Name -like "UE_*" }
Show-Check "Unreal Engine" (($ueDirs | Measure-Object).Count -gt 0) (($ueDirs | Select-Object -ExpandProperty FullName) -join ", ") "Install UE 5.7.x via https://www.unrealengine.com/en-US/download"

$vsWhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
$vsFound = Test-Path $vsWhere
if ($vsFound) {
    $vsInstall = & $vsWhere -latest -products * -property installationPath
    Show-Check "Visual Studio" ($null -ne $vsInstall -and $vsInstall.Length -gt 0) $vsInstall "Install Visual Studio Build Tools"

    $vcInstall = & $vsWhere -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
    Show-Check "Visual Studio C++ Tools" ($null -ne $vcInstall -and $vcInstall.Length -gt 0) $vcInstall "Install Game development with C++ workload"
}
else {
    Show-Check "Visual Studio" $false "" "Install Visual Studio Build Tools: https://visualstudio.microsoft.com/vs/features/game-development/"
    Show-Check "Visual Studio C++ Tools" $false "" "Install Game development with C++ workload"
}

$windowsSdkRoot = "${env:ProgramFiles(x86)}\Windows Kits\10\Include"
$windowsSdkVersions = @()
if (Test-Path $windowsSdkRoot) {
    $windowsSdkVersions = Get-ChildItem $windowsSdkRoot -Directory | Select-Object -ExpandProperty Name
}
$validWindowsSdk = $windowsSdkVersions | Where-Object { $_ -ge "10.0.19041.0" }
Show-Check "Windows SDK" (($validWindowsSdk | Measure-Object).Count -gt 0) (($validWindowsSdk | Sort-Object -Descending | Select-Object -First 1)) "Install Windows 10/11 SDK through Visual Studio Installer"

Write-Host ""
$repoRoot = Split-Path -Parent $PSScriptRoot
$projectPath = Join-Path $repoRoot "DockShield\DockShield.uproject"
if (Test-Path $projectPath) {
    Write-Host "Next: install missing tools, then open DockShield\DockShield.uproject and verify M_Test_Targeting in editor."
}
else {
    Write-Host "Next: install missing tools, then create the Unreal 5.7 Third Person project named DockShield."
}
