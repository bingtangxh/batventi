param(
  [string]$SourceDir = "batventi",

  [string]$OutDir = "build",

  [string]$Output = "batventi.exe"
)

$ErrorActionPreference = "Stop"

function Find-Command {
  param([string]$Name)
  return Get-Command $Name -ErrorAction SilentlyContinue
}

function Invoke-Checked {
  param(
    [string]$FilePath,
    [string[]]$Arguments
  )

  Write-Host "> $FilePath $($Arguments -join ' ')"
  & $FilePath @Arguments
  if ($LASTEXITCODE -ne 0) {
    throw "$FilePath failed with exit code $LASTEXITCODE"
  }
}

function Get-ObjectName {
  param(
    [string]$Root,
    [string]$Path,
    [string]$Extension
  )

  $rootFull = [IO.Path]::GetFullPath($Root).TrimEnd('\', '/')
  $pathFull = [IO.Path]::GetFullPath($Path)

  if ($pathFull.StartsWith($rootFull, [StringComparison]::OrdinalIgnoreCase)) {
    $relative = $pathFull.Substring($rootFull.Length).TrimStart('\', '/')
  } else {
    $relative = [IO.Path]::GetFileName($pathFull)
  }

  $flatName = $relative -replace "[\\/]", "_"
  return [IO.Path]::ChangeExtension($flatName, $Extension)
}

$repoRoot = $PSScriptRoot
if (-not $repoRoot) {
  $repoRoot = (Get-Location).Path
}

$sourceRoot = Join-Path $repoRoot $SourceDir
$outRoot = Join-Path $repoRoot $OutDir
$objRoot = Join-Path $outRoot "obj"
$exePath = Join-Path $outRoot $Output
$resourceScript = Join-Path $sourceRoot "batventi.rc"

if (-not (Test-Path $sourceRoot)) {
  throw "Source directory not found: $sourceRoot"
}

$cSources = Get-ChildItem -Path $sourceRoot -Recurse -File -Filter "*.c" | Sort-Object FullName
$cppSources = Get-ChildItem -Path $sourceRoot -Recurse -File -Include "*.cpp", "*.cxx", "*.cc" | Sort-Object FullName
$sources = @($cSources) + @($cppSources)
if (-not $sources) {
  throw "No .c/.cpp files found under $sourceRoot"
}

New-Item -ItemType Directory -Force -Path $outRoot, $objRoot | Out-Null

Write-Host "Compiler: msvc"
Write-Host "C files : $($cSources.Count)"
Write-Host "C++ files: $($cppSources.Count)"
Write-Host "Output  : $exePath"

$commonLibs = @(
  "kernel32",
  "user32",
  "gdi32",
  "winspool",
  "comdlg32",
  "advapi32",
  "shell32",
  "ole32",
  "oleaut32",
  "uuid",
  "odbc32",
  "odbccp32",
  "Propsys"
)

foreach ($tool in @("cl.exe", "link.exe", "rc.exe")) {
  if (-not (Find-Command $tool)) {
    throw "$tool not found. Run this from a Visual Studio Developer PowerShell/Command Prompt."
  }
}

$resourceObj = Join-Path $objRoot "batventi.res"
if (-not (Test-Path $resourceScript)) {
  throw "Resource script not found: $resourceScript"
}
Invoke-Checked "rc.exe" @("/nologo", "/fo", $resourceObj, $resourceScript)

$commonCompileFlags = @(
  "/nologo",
  "/O1",
  "/W3",
  "/Z7",
  "/GF",
  "/FS",
  "/EHsc",
  "/utf-8",
  "/MT",
  "/DUNICODE",
  "/D_UNICODE",
  "/I$sourceRoot",
  "/c"
)

$objects = @()
foreach ($source in $cSources) {
  $objPath = Join-Path $objRoot (Get-ObjectName $sourceRoot $source.FullName ".obj")
  Invoke-Checked "cl.exe" ($commonCompileFlags + @("/TC", "/Fo$objPath", $source.FullName))
  $objects += $objPath
}

foreach ($source in $cppSources) {
  $objPath = Join-Path $objRoot (Get-ObjectName $sourceRoot $source.FullName ".obj")
  Invoke-Checked "cl.exe" ($commonCompileFlags + @("/TP", "/Fo$objPath", $source.FullName))
  $objects += $objPath
}

$linkArgs = @("/NOLOGO") + $objects + $resourceObj
$linkArgs += @("/MANIFEST", "/MANIFEST:EMBED", "/OUT:$exePath")
$linkArgs += ($commonLibs | ForEach-Object { "$_.lib" })

Invoke-Checked "link.exe" $linkArgs

Write-Host "Built: $exePath"
