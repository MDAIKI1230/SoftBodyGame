#pragma once

#include <cstdint>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "CollisionConstants.h"

#include "CollisionTypeDocument.h"

struct CollisionSettingDocument
{
public:
	CollisionSettingDocument();
	std::vector<std::uint8_t> GetSortedCategoryBits() const;
	bool AddType(std::uint8_t _bit, std::string _codeName);
private:
	std::uint8_t GetCategoryBit(CollisionTag::Type _type) const;
public:
	// 独自カテゴリ一覧(ビットに対しての名前)
	std::unordered_map<std::uint8_t, CollisionTypeDocument> types;
	// カテゴリー事の衝突無効対象
	std::unordered_map<std::uint8_t, std::unordered_set<std::uint8_t>> ignoreMaps;
};