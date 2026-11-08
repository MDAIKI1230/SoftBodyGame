# シーンファクトリのScene特殊化をフォルダ名から作っちゃうよ

[CmdletBinding()]
param(
	[string]$projectRoot,
	[string]$cacheRoot,
	[string]$platform = 'x64',
	[string]$configuration = 'Debug'
)

$ErrorActionPreference = 'Stop'

# プロジェクトディレクトリ
$engineDir = Join-Path (Split-Path -Parent $PSScriptRoot) 'Include'
# ゲームオブジェクトフォルダ
$objectDir = Join-Path $engineDir 'Source\System\Scenes'
# 追加で調べるプロジェクトディレクトリ
$projectDir = $engineDir
if (![string]::IsNullOrWhiteSpace($projectRoot))
{
	$projectDir = (Resolve-Path -LiteralPath $projectRoot -ErrorAction Stop).ProviderPath.TrimEnd([char[]]'\/')
}
# 追加プロジェクトのゲームオブジェクトフォルダ
$projectObjectDir = Join-Path $projectDir 'Source\System\Scenes'
# Toolの一つ上を既定のキャッシュにする。指定されたルートがあれば優先する。
if ([string]::IsNullOrWhiteSpace($cacheRoot))
{
	$cacheRoot = Join-Path (Split-Path -Parent $PSScriptRoot) '.cache'
}
$cacheBase = $ExecutionContext.SessionState.Path.GetUnresolvedProviderPathFromPSPath($cacheRoot)
$cacheDir = Join-Path $cacheBase ($platform + '\' + $configuration)

# 出力先ファイル
$outputFile = Join-Path $cacheDir 'SceneFactory.inl'
$outputCppFile = Join-Path $cacheDir 'SceneFactory.cpp'

# フォルダーがまだない場合も、登録対象は0件として扱う。
$headers = @()
if (Test-Path -LiteralPath $objectDir -PathType Container)
{
	$headers = @(
		Get-ChildItem -LiteralPath $objectDir -Filter '*.h' -File -Recurse)
}

# 追加プロジェクトのヘッダーを取得する。同じ場所は二重に調べない
$projectHeaders = @()
if ($projectDir -ne $engineDir -and (Test-Path -LiteralPath $projectObjectDir -PathType Container))
{
	$projectHeaders = @(
		Get-ChildItem -LiteralPath $projectObjectDir -Filter '*.h' -File -Recurse)
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

# 初期Sceneの設定は、ビルドするプロジェクト側のJSONから読む。
$settingPath = Join-Path $projectDir 'Res\Data\Setting\GameSetting.json'
$initialScene = ''
if (Test-Path -LiteralPath $settingPath -PathType Leaf)
{
	$setting = [System.IO.File]::ReadAllText($settingPath) | ConvertFrom-Json
	$initialScene = [string]$setting.InitialScene
}
# 0件の生成は許可し、Sceneがある場合の指定間違いはここで知らせる。
if ($typePaths.Count -gt 0 -and ![string]::IsNullOrWhiteSpace($initialScene) -and !$typePaths.ContainsKey($initialScene))
{
	throw "初期Scene '$initialScene' が見つかりません。設定ファイル: $settingPath"
}

$lines = [System.Collections.Generic.List[string]]::new()
$lines.Add('// ツールにより自動生成されたものです。変更しないでください。' + "`n")
$lines.Add('#pragma once' + "`n")
$lines.Add('#include "SceneFactory.h"' + "`n")

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

# 既存の登録に、追加プロジェクトの型も加える
$headers += $projectHeaders

foreach ($header in $headers)
{
    $lines.Add('')
	$lines.Add('template<>')
	$lines.Add('inline std::unique_ptr<SceneBase> SceneFactory::Create<"'+ $header.BaseName +'">()')
	$lines.Add('{')
	$lines.Add('	return std::make_unique<'+ $header.BaseName +'>();')
	$lines.Add('}')
}

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

# 初期Sceneの作成関数も、同じキャッシュへ通常のcppとして生成する。
# SceneManagerはこの関数を呼び、具体的なSceneはゲーム側のビルドで確定する。
$lines = [System.Collections.Generic.List[string]]::new()
$lines.Add('// ツールにより自動生成されたものです。変更しないでください。')
$lines.Add('')
$lines.Add('#include "SceneFactory.h"')
if ($typePaths.Count -eq 0 -or [string]::IsNullOrWhiteSpace($initialScene))
{
	$lines.Add('#include <stdexcept>')
}
$lines.Add('')
$lines.Add('std::unique_ptr<SceneBase> SceneFactory::CreateInitialScene()')
$lines.Add('{')
if ($typePaths.Count -gt 0 -and ![string]::IsNullOrWhiteSpace($initialScene))
{
	$lines.Add("`treturn SceneFactory::Create<`"" + $initialScene + '">();')
}
else
{
	# 0件でもビルドできる定義を出し、起動された場合だけ設定不足を知らせる。
	$lines.Add("`tthrow std::runtime_error(" + '"初期Sceneが未設定、または生成できるSceneがありません。"' + ');')
}
$lines.Add('}')

$content = ($lines -join "`n").TrimEnd([char[]]"`r`n")
$outputPath = [System.IO.Path]::GetFullPath($outputCppFile)
if (!(Test-Path -LiteralPath $outputPath) -or
	[System.IO.File]::ReadAllText($outputPath) -cne $content)
{
	$encoding = [System.Text.UTF8Encoding]::new($true)
	[System.IO.File]::WriteAllText($outputPath, $content, $encoding)
}

Write-Verbose ('Scenes: ' + $headers.Count + ' 件でした。')
Write-Output $outputFile