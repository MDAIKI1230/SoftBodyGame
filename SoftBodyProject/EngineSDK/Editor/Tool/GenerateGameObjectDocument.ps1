[CmdletBinding()]
param(
	[string]$projectRoot,
	[string]$cacheRoot,
	[string]$platform = 'x64',
	[string]$configuration = 'Debug'
)

$ErrorActionPreference = 'Stop'

# Editorプロジェクトディレクトリ
$editorDir = Split-Path -Parent $PSScriptRoot
# プロジェクトディレクトリ
$engineDir = Join-Path (Split-Path -Parent $editorDir) 'Include'
# ゲームオブジェクトフォルダ
$objectDir = Join-Path $engineDir 'Source\Objects'
# 追加で調べるプロジェクトディレクトリ
$projectDir = $engineDir
if (![string]::IsNullOrWhiteSpace($projectRoot))
{
	$projectDir = (Resolve-Path -LiteralPath $projectRoot -ErrorAction Stop).ProviderPath.TrimEnd([char[]]'\/')
}
# 追加プロジェクトのゲームオブジェクトフォルダ
$projectObjectDir = Join-Path $projectDir 'Source\Objects'
# Toolの一つ上を既定のキャッシュにする。指定されたルートがあれば優先する。
if ([string]::IsNullOrWhiteSpace($cacheRoot))
{
	$cacheRoot = Join-Path (Split-Path -Parent $PSScriptRoot) '.cache'
}
$cacheBase = $ExecutionContext.SessionState.Path.GetUnresolvedProviderPathFromPSPath($cacheRoot)
$cacheDir = Join-Path $cacheBase ($platform + '\' + $configuration)

# 出力先ファイル
$outputFile = Join-Path $cacheDir 'GameObjectDocumentRegistry.cpp'

# フォルダーがまだない場合も、登録対象は0件として扱う。
$headers = @()
if (Test-Path -LiteralPath $objectDir -PathType Container)
{
	$headers = @(
		Get-ChildItem -LiteralPath $objectDir -Filter '*.h' -File -Recurse |
		Where-Object {
			$_.FullName.Substring($objectDir.Length + 1) -notmatch '(^|[\\/])Base[\\/]'
		} |
		Sort-Object FullName
	)
}

# 追加プロジェクトのヘッダーを取得する。同じ場所は二重に調べない
$projectHeaders = @()
if ($projectDir -ne $engineDir -and (Test-Path -LiteralPath $projectObjectDir -PathType Container))
{
	$projectHeaders = @(
		Get-ChildItem -LiteralPath $projectObjectDir -Filter '*.h' -File -Recurse |
		Where-Object {
			$_.FullName.Substring($projectObjectDir.Length + 1) -notmatch '(^|[\\/])Base[\\/]'
		} |
		Sort-Object FullName)
}

# 同名の型があれば、両方のヘッダーを含められないので生成前に止める
$typePaths = [System.Collections.Generic.Dictionary[string, string]]::new([System.StringComparer]::Ordinal)
foreach ($header in ($headers + $projectHeaders))
{
	if ($typePaths.ContainsKey($header.BaseName))
	{
		throw "Duplicate factory type '$($header.BaseName)': $($typePaths[$header.BaseName]) and $($header.FullName)"
	}
	$typePaths.Add($header.BaseName, $header.FullName)
}

$lines = [System.Collections.Generic.List[string]]::new()
$lines.Add('// ツールにより自動生成されたものです。変更しないでください。' + "`n")
$lines.Add('#include <algorithm>' + "`n")
$lines.Add('#include "GameObjectDocumentRegistry.h"' + "`n")

foreach ($header in $headers)
{
	$relativePath = $header.FullName.Substring($engineDir.Length + 1)
	$relativePath = $relativePath.Replace('\', '/')

	$lines.Add('#include "' + $relativePath + '"')
}

# 追加プロジェクトのヘッダーは、キャッシュからも読める絶対パスにする
foreach ($header in $projectHeaders)
{
	$relativePath = $header.FullName.Replace('\', '/')
	$lines.Add('#include "' + $relativePath + '"')
}

# 既存コードの追加

$fixedCode = @'

ObjectDocument GameObjectDocumentRegistry::Create(const std::string& _name, std::uint64_t _id)
{
	const auto& funcs{ GetCreateFuncs() };
	const auto it{ funcs.find(_name) };

	if (it == funcs.end())
	{
		// ミスってるなら0で無効値
		ObjectDocument object{ 0 };
		return object;
	}

	return it->second(_id, false);
}

ObjectDocument GameObjectDocumentRegistry::CreateDefault(const std::string& _name, std::uint64_t _id)
{
	const auto& funcs{ GetCreateFuncs() };
	const auto it{ funcs.find(_name) };

	if (it == funcs.end())
	{
		// ミスってるなら0で無効値
		ObjectDocument object{ 0 };
		return object;
	}

	return it->second(_id, true);
}

std::vector<std::string> GameObjectDocumentRegistry::GetGameObjectNames()
{
	const auto& funcs{ GetCreateFuncs() };

	std::vector<std::string> names;
	names.reserve(funcs.size());

	for (const auto& entry : funcs)
	{
		names.push_back(entry.first);
	}

	std::sort(names.begin(), names.end());
	return names;
}
'@

$lines.Add($fixedCode)

# 実際の関数を作っていく所

$lines.Add('')
$lines.Add('const std::vector<std::filesystem::path>& GameObjectDocumentRegistry::GetGameObjectPaths()')
$lines.Add('{')
$lines.Add('	static std::vector<std::filesystem::path> paths{')

foreach ($header in $headers)
{
	$relativePath = '	"' + $header.FullName.Substring($objectDir.Length + 1) + '",'
	$relativePath = $relativePath.Replace('\', '/')

	$lines.Add($relativePath)
}

# 追加プロジェクトの一覧も、ゲームオブジェクトフォルダからの相対パスにする
foreach ($header in $projectHeaders)
{
	$relativePath = '	"' + $header.FullName.Substring($projectObjectDir.Length + 1) + '",'
	$relativePath = $relativePath.Replace('\', '/')
	$lines.Add($relativePath)
}

$lines.Add('	};')
$lines.Add('')
$lines.Add('	return paths;')
$lines.Add('}')

$lines.Add('')
# 既存の登録に、追加プロジェクトの型も加える
$headers += $projectHeaders

$lines.Add('bool GameObjectDocumentRegistry::RegisterAll()')
$lines.Add('{')

foreach ($header in $headers)
{
	$lines.Add('	if (!Register<' + $header.BaseName + ', "' + $header.BaseName + '">())')
	$lines.Add('	{')
	$lines.Add('		return false;')
	$lines.Add('	}')
}

$lines.Add('	return true;')
$lines.Add('}')

# 出力先フォルダを作る

$content = ($lines -join "`n").TrimEnd([char[]]"`r`n")
$outputPath = [System.IO.Path]::GetFullPath($outputFile)
$outputDir = [System.IO.Path]::GetDirectoryName($outputPath)

[System.IO.Directory]::CreateDirectory($outputDir) | Out-Null

# ファイルが存在しないまたは、既存の内容が生成内容と違うなら新しくする

if (!(Test-Path -LiteralPath $outputPath) -or
	[System.IO.File]::ReadAllText($outputPath) -cne $content)
{
	$encoding = [System.Text.UTF8Encoding]::new($true)
	[System.IO.File]::WriteAllText($outputPath, $content, $encoding)
}

Write-Verbose ('Documents: ' + $headers.Count + ' 件でした。')
Write-Output $outputFile