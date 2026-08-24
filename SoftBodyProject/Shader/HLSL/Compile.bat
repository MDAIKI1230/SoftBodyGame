@echo off
cd /d "%~dp0"

ShaderCompiler.exe /Tps_5_0 /Emain "Sky\SkySolidPS.hlsl"
ShaderCompiler.exe /Tvs_5_0 /Emain "Sky\SkyVS.hlsl"

pause
