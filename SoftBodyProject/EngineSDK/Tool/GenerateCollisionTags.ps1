[CmdletBinding()]
param(
	[Parameter(Mandatory = $true)]
	[string]$ProjectRoot,
	[string]$OutputDirectory,
	[switch]$AllowMissingSettings
)

$ErrorActionPreference = 'Stop'

$projectDir = (Resolve-Path -LiteralPath $ProjectRoot).ProviderPath
$settingPath = Join-Path $projectDir 'Res\Data\Setting\CollisionSetting.json'
if ([string]::IsNullOrWhiteSpace($OutputDirectory))
{
	$OutputDirectory = Join-Path $projectDir '.cache'
}
$outputDir = $ExecutionContext.SessionState.Path.GetUnresolvedProviderPathFromPSPath($OutputDirectory)
$outputPath = Join-Path $outputDir 'CollisionTags.generated.h'

$customCategories = @()
if (!$AllowMissingSettings -or (Test-Path -LiteralPath $settingPath))
{
	$setting = Get-Content -LiteralPath $settingPath -Raw -Encoding UTF8 | ConvertFrom-Json
	if ($setting.CustomCategories -isnot [array])
	{
		throw "CustomCategories must be an array: $settingPath"
	}
	$customCategories = $setting.CustomCategories
}

$bits = [System.Collections.Generic.HashSet[int]]::new()
$names = [System.Collections.Generic.HashSet[string]]::new([System.StringComparer]::Ordinal)
$names.UnionWith([string[]]@('NONE', 'RAGDOLL'))

$lines = [System.Collections.Generic.List[string]]::new()
$lines.Add('// 自動生成ファイル')
$lines.Add('#pragma once')
$lines.Add('')
$lines.Add('#include <cstdint>')
$lines.Add('')
$lines.Add('#include "CollisionTag.h"')
$lines.Add('')
$lines.Add('namespace CollisionTag')
$lines.Add('{')

foreach ($category in ($customCategories | Sort-Object -Property Bit))
{
	$bit = $category.Bit
	$name = $category.CodeName

	if (($bit -isnot [int] -and $bit -isnot [long]) -or $bit -lt 2 -or $bit -gt 31)
	{
		throw "Custom category bit must be an integer from 2 to 31: '$bit' in $settingPath"
	}
	if ($name -isnot [string] -or $name -cnotmatch '\A[A-Z][A-Z0-9]*(?:_[A-Z0-9]+)*\z')
	{
		throw "Invalid category CodeName '$name' in $settingPath"
	}
	if (!$bits.Add([int]$bit))
	{
		throw "Duplicate category bit '$bit' in $settingPath"
	}
	if (!$names.Add($name))
	{
		throw "Duplicate category CodeName '$name' in $settingPath"
	}

	$lines.Add("`tinline constexpr Type ${name}{ static_cast<Type>(std::uint32_t{ 1 } << $bit) };")
}

$lines.Add('}')
$content = $lines -join "`n"
if (!(Test-Path -LiteralPath $outputPath) -or [System.IO.File]::ReadAllText($outputPath) -cne $content)
{
	[System.IO.Directory]::CreateDirectory($outputDir) | Out-Null
	[System.IO.File]::WriteAllText($outputPath, $content, [System.Text.UTF8Encoding]::new($true))
}

Write-Output $outputPath