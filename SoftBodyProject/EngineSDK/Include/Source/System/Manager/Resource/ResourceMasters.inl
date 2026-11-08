#pragma once

#include "ResourceMasters.h"

template<class HANDLE>
HANDLE ResourceMasters<HANDLE>::GetHandle(std::filesystem::path _path)
{
	const auto key{ _path.filename() };
	const auto it{ masters.find(key) };
	if (it != masters.end())
	{
		return it->second.handle;
	}

	return {};
}

template<class HANDLE>
bool ResourceMasters<HANDLE>::Retain(std::filesystem::path _path, HANDLE _handle)
{
	if (!Add(_path, _handle))
	{
		return false;
	}

	const auto key{ _path.filename() };
	const auto it{ masters.find(key) };
	if (it != masters.end())
	{
		it->second.ownerCount++;
	}

	return true;
}

template<class HANDLE>
HANDLE ResourceMasters<HANDLE>::Release(std::filesystem::path _path)
{
	const auto key{ _path.filename() };
	auto it{ masters.find(key) };

	if (it == masters.end())
	{
		return {};
	}

	const HANDLE handle{ it->second.handle };
	it->second.ownerCount--;

	if (it->second.ownerCount == 0)
	{
		masters.erase(it);

		return handle;
	}

	return {};
}

template<class HANDLE>
std::vector<HANDLE> ResourceMasters<HANDLE>::Clear()
{
	std::vector<HANDLE> eraseHandles;

	for (const auto& [path, row] : masters)
	{
		eraseHandles.push_back(row.handle);
	}

	masters.clear();

	return eraseHandles;
}

template<class HANDLE>
bool ResourceMasters<HANDLE>::Add(std::filesystem::path _path, HANDLE _handle)
{
	if (!_handle.IsValid())
	{
		return false;
	}

	// ファイル名だけをキーとして扱う
	const auto key{ _path.filename() };

	masters.try_emplace(key, _handle, 0);

	return masters[key].handle.GetGeneration() != 0;
}