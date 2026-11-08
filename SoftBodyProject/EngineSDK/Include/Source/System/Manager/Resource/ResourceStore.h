#pragma once

#include <unordered_set>

#include "ResourceMasters.h"

#include "ResourceOwnerHandle.h"

template<class HANDLE>
class ResourceStore
{
	using OwnerResources = std::unordered_set<std::filesystem::path>;
public:
	bool Add(ResourceOwnerHandle _owner, std::filesystem::path _path, HANDLE _handle);
	[[nodiscard]] HANDLE GetHandle(std::filesystem::path _path);
	[[nodiscard]] HANDLE Erase(ResourceOwnerHandle _owner, std::filesystem::path _path);
	std::vector<HANDLE> Clear();
	[[nodiscard]] std::vector<HANDLE> Clear(ResourceOwnerHandle _owner);
private:
	std::unordered_map<ResourceOwnerHandle, OwnerResources> owners;
	ResourceMasters<HANDLE> masters;
};

#include "ResourceStore.inl"