@echo off
cd /d "%~dp0"

ShaderCompiler.exe /Tps_5_0 /Emain "Sky\SkyPS.hlsl"
ShaderCompiler.exe /Tvs_5_0 /Emain "Sky\SkyVS.hlsl"

pause
