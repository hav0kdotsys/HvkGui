param(
    [Alias('d')]
    [switch]$Debug,

    [Alias('r')]
    [switch]$Release,

    [Alias('e')]
    [switch]$Dev,

    [Alias('v')]
    [switch]$Verbose,

    [string[]]$Configurations,

    [string]$Platform,

    [string]$MsbuildPath = 'D:\Program Files\Microsoft Visual Studio\18\Insiders\MSBuild\Current\Bin\MSBuild.exe',

    [string]$Solution = 'HvkGui.slnx'
)

$ErrorActionPreference = 'Stop'

function Get-ProjectConfigurations {
    param([string]$ProjectPath)

    if (-not (Test-Path $ProjectPath)) {
        return @()
    }

    [xml]$projectXml = Get-Content -Path $ProjectPath
    $configs = $projectXml.Project.ProjectConfigurations.ProjectConfiguration |
        ForEach-Object { $_.Configuration } |
        Sort-Object -Unique

    return $configs
}

function Get-ProjectPlatforms {
    param([string]$ProjectPath)

    if (-not (Test-Path $ProjectPath)) {
        return @()
    }

    [xml]$projectXml = Get-Content -Path $ProjectPath
    $platforms = $projectXml.Project.ProjectConfigurations.ProjectConfiguration |
        ForEach-Object { $_.Platform } |
        Sort-Object -Unique

    return $platforms
}

function Resolve-Selection {
    param(
        [string[]]$Configured,
        [string[]]$Defaults
    )

    if ($Configured -and $Configured.Count -gt 0) {
        return $Configured
    }

    return $Defaults
}

$projectPath = 'HvkGui.vcxproj'
$availableConfigs = Get-ProjectConfigurations -ProjectPath $projectPath
$availablePlatforms = Get-ProjectPlatforms -ProjectPath $projectPath

if (-not $Platform) {
    $Platform = Resolve-Selection -Configured $availablePlatforms -Defaults @('x64') | Select-Object -First 1
}

$selectedConfigs = @()
if ($Debug) {
    $selectedConfigs += 'Debug'
}
if ($Release) {
    $selectedConfigs += 'Release'
}
if ($Dev) {
    $selectedConfigs += 'Dev'
}
if ($Configurations) {
    $selectedConfigs += $Configurations
}

if (-not $selectedConfigs -or $selectedConfigs.Count -eq 0) {
    $promptConfigs = if ($availableConfigs.Count -gt 0) { $availableConfigs -join ', ' } else { 'Debug, Release' }
    Write-Host "No configuration flags supplied." -ForegroundColor Yellow
    Write-Host "Available configurations: $promptConfigs" -ForegroundColor Yellow
    $inputConfigs = Read-Host 'Enter configurations to build (comma-separated)'
    $selectedConfigs = $inputConfigs -split ',' | ForEach-Object { $_.Trim() } | Where-Object { $_ }
}

if (-not $selectedConfigs -or $selectedConfigs.Count -eq 0) {
    throw 'No configurations selected. Provide -debug/-release or -Configurations.'
}

if ($availableConfigs.Count -gt 0) {
    $unknownConfigs = $selectedConfigs | Where-Object { $availableConfigs -notcontains $_ }
    if ($unknownConfigs.Count -gt 0) {
        Write-Host "Warning: unknown configurations: $($unknownConfigs -join ', ')" -ForegroundColor Yellow
    }
}

if ($availablePlatforms.Count -gt 0 -and ($availablePlatforms -notcontains $Platform)) {
    Write-Host "Warning: unknown platform '$Platform'. Available: $($availablePlatforms -join ', ')" -ForegroundColor Yellow
}

if (-not (Test-Path $MsbuildPath)) {
    if ($MsbuildPath -eq 'D:\Program Files\Microsoft Visual Studio\18\Insiders\MSBuild\Current\Bin\MSBuild.exe') {
        $fallbackPaths = @(
            'C:\Program Files\Microsoft Visual Studio\18\Insiders\MSBuild\Current\Bin\MSBuild.exe',
            'C:\Program Files\Microsoft Visual Studio\2022\Enterprise\MSBuild\Current\Bin\MSBuild.exe',
            'C:\Program Files\Microsoft Visual Studio\2022\Professional\MSBuild\Current\Bin\MSBuild.exe',
            'C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe',
            'C:\Program Files\Microsoft Visual Studio\2019\Enterprise\MSBuild\Current\Bin\MSBuild.exe',
            'C:\Program Files\Microsoft Visual Studio\2019\Professional\MSBuild\Current\Bin\MSBuild.exe',
            'C:\Program Files\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\MSBuild.exe'
        )
        foreach ($path in $fallbackPaths) {
            if (Test-Path $path) {
                $MsbuildPath = $path
                break
            }
        }
    }
    if (-not (Test-Path $MsbuildPath)) {
        throw "MSBuild not found at '$MsbuildPath'. Provide -MsbuildPath to override."
    }
}

if (-not (Test-Path $Solution)) {
    throw "Solution file '$Solution' not found. Provide -Solution to override."
}

$verbosity = if ($Verbose) { 'detailed' } else { 'minimal' }

foreach ($config in $selectedConfigs) {
    $buildTarget = $Solution
    if ($Solution -like '*.slnx' -and $config -eq 'Dev') {
        $buildTarget = $projectPath
    }
    Write-Host "Building $buildTarget ($config|$Platform)" -ForegroundColor Cyan
    & $MsbuildPath $buildTarget /m /t:Build "/p:Configuration=$config;Platform=$Platform" "/v:$verbosity"
    if ($LASTEXITCODE -ne 0) {
        throw "Build failed for configuration '$config' (exit code $LASTEXITCODE)."
    }
}

Write-Host 'Build completed successfully.' -ForegroundColor Green
