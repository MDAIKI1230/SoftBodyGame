#pragma once

#include <Windows.h>
#include <filesystem>

class FileDialog
{
public:
	static HRESULT OpenJson(HWND _owner, std::filesystem::path _openFilePath, std::filesystem::path& _outputFilePath);
	static HRESULT SaveJson(HWND _owner, std::filesystem::path _openFilePath, std::filesystem::path& _outputFilePath, std::filesystem::path _fileName = L"NewScene.json");
};