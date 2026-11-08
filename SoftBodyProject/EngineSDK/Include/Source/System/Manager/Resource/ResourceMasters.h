#pragma once

#include <unordered_map>
#include <filesystem>
#include <vector>

#include "ResourceRow.h"

template<class HANDLE>
class ResourceMasters
{
public:
	[[nodiscard]] HANDLE GetHandle(std::filesystem::path _path);
	bool Retain(std::filesystem::path _path, HANDLE _handle);
	[[nodiscard]] HANDLE Release(std::filesystem::path _path);
	[[nodiscard]] std::vector<HANDLE> Clear();
private:
	bool Add(std::filesystem::path _path, HANDLE _handle);
private:
	std::unordered_map<std::filesystem::path, ResourceRow<HANDLE>> masters;
};

#include "ResourceMasters.inl"