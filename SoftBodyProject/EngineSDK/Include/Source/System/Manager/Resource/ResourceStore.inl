#pragma once

#include "ResourceStore.h"

template<class HANDLE>
bool ResourceStore<HANDLE>::Add(ResourceOwnerHandle _owner, std::filesystem::path _path, HANDLE _handle)
{
	const auto key{ _path.filename() };
	const auto it{ owners.find(_owner) };
	if (it != owners.end() && it->second.contains(key))
	{
		return true;
	}

	if (!masters.Retain(key, _handle))
	{
		return false;
	}

	owners.try_emplace(_owner).first->second.insert(key);
	return true;
}

template<class HANDLE>
HANDLE ResourceStore<HANDLE>::GetHandle(std::filesystem::path _path)
{
	return masters.GetHandle(_path);
}

template<class HANDLE>
HANDLE ResourceStore<HANDLE>::Erase(ResourceOwnerHandle _owner, std::filesystem::path _path)
{
	auto it{ owners.find(_owner) };

	if (it != owners.end())
	{
		const auto key{ _path.filename() };

		if (it->second.erase(key) == 1)
		{
			return masters.Release(key);
		}
	}

	return {};
}

template<class HANDLE>
std::vector<HANDLE> ResourceStore<HANDLE>::Clear()
{
	owners.clear();
	return masters.Clear();
}

template<class HANDLE>
std::vector<HANDLE> ResourceStore<HANDLE>::Clear(ResourceOwnerHandle _owner)
{
	auto it{ owners.find(_owner) };

	if (it == owners.end())
	{
		return {};
	}

	std::vector<HANDLE> eraseHandles;

	for (auto path : it->second)
	{
		HANDLE handle{ masters.Release(path) };
		if (handle.IsValid())
		{
			eraseHandles.push_back(handle);
		}
	}

	owners.erase(it);

	return eraseHandles;
}