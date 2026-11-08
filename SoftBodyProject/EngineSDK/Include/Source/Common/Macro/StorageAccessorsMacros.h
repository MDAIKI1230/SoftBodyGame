#pragma once

#include <cstdint>
#include <cassert>
#include <memory>
#include <ranges>
#include <unordered_map>
#include <utility>
#include <vector>



namespace MD::Storage::Detail
{
	// 取得用関数
	template<class Container>
	[[nodiscard("戻り値が無視されています")]] decltype(auto) Get(const Container& _container, uint32_t _index)
	{
		assert(_index < _container.size() && "範囲外アクセス");
		return (_container[_index]);
	}

	// 参照取得関数
	template<class Container>
	[[nodiscard("戻り値が無視されています")]] decltype(auto) Edit(Container& _container, uint32_t _index)
	{
		assert(_container.size() > _index && "範囲外アクセス");
		return _container[_index];
	}

	// 値代入関数
	template<class Container, class Type>
	void Set(Container& _container, uint32_t _index, const Type& _value)
	{
		assert(_container.size() > _index && "範囲外アクセス");
		_container[_index] = _value;
	}

	// 要素数取得関数
	template<class Container>
	[[nodiscard("戻り値が無視されています")]] uint32_t Count(const Container& _container) noexcept
	{
		return static_cast<uint32_t>(_container.size());
	}

	// 空チェック関数
	template<class Container>
	[[nodiscard("戻り値が無視されています")]] bool Empty(const Container& _container) noexcept
	{
		return _container.empty();
	}

	// 変化不可配列取得関数
	template<class Container>
	[[nodiscard("戻り値が無視されています")]] auto GetRange(const Container& _container) noexcept
	{
		return std::ranges::subrange{ _container.cbegin(), _container.cend() };
	}

	// 変化可能配列取得関数
	template<class Container>
	[[nodiscard("戻り値が無視されています")]] auto EditRange(Container& _container) noexcept
	{
		return std::ranges::subrange{ _container.begin(), _container.end() };
	}

	// 追加生成関数
	template<class Container, class... Args>
	void Emplace(Container& _container, Args&&... _args)
	{
		_container.emplace_back(std::forward<Args>(_args)...);
	}

	// 最後に追加関数
	template<class Container, class Type>
	void Add(Container& _container, const Type& _value)
	{
		_container.push_back(_value);
	}

	// メモリ確保関数
	template<class Container>
	void Reserve(Container& _container, uint32_t _size)
	{
		_container.reserve(_size);
	}

	// 全要素削除関数
	template<class Container>
	void Clear(Container& _container) noexcept
	{
		_container.clear();
	}
}

/// @def MD_STORAGE_READ_WRITE_COLUMN(Type, _tag, _member)
/// @brief 読み書き可能なStorage Columnを用意します。
///
/// 次の関数を生成します。
/// - GetName
/// - EditName
/// - SetName
/// - GetNameRange
/// - EditNameRange
/// - CountName
/// - EmptyName
///
/// @param Type 格納する要素型
/// @param _tag 変数が何か(関数名に使用される)
/// @param _member 内部メンバー変数名
///
/// @warning 取得した参照やRangeは再確保後に無効になります。
#define MD_STORAGE_READ_WRITE_COLUMN(Type, _tag, _member)\
public:\
	/* 値読み取り */\
	[[nodiscard("戻り値が無視されています")]] decltype(auto) Get##_tag(uint32_t _index) const\
	{\
		return MD::Storage::Detail::Get(_member, _index);\
	}\
	/* 値参照渡しで変更できる */\
	[[nodiscard("戻り値が無視されています")]] decltype(auto) Edit##_tag(uint32_t _index)\
	{\
		return MD::Storage::Detail::Edit(_member, _index);\
	}\
	/* 値代入 */\
	void Set##_tag(uint32_t _index, const Type& _value)\
	{\
		MD::Storage::Detail::Set(_member, _index, _value);\
	}\
	/* 配列の要素数 */\
	[[nodiscard("戻り値が無視されています")]] uint32_t Count##_tag() const noexcept\
	{\
		return MD::Storage::Detail::Count(_member);\
	}\
	/* 空チェック */\
	[[nodiscard("戻り値が無視されています")]] bool Empty##_tag() const noexcept\
	{\
		return MD::Storage::Detail::Empty(_member);\
	}\
	/* 変化不可配列取得(全探査用) */\
	[[nodiscard("戻り値が無視されています")]] auto Get##_tag##Range() const noexcept\
	{\
		return MD::Storage::Detail::GetRange(_member);\
	}\
	/* 変化可能配列取得(全探査用) */\
	[[nodiscard("戻り値が無視されています")]] auto Edit##_tag##Range() noexcept\
	{\
		return MD::Storage::Detail::EditRange(_member);\
	}\
private:\
	std::vector<Type> _member;

/// @def MD_STORAGE_READ_ONLY_COLUMN(Type, _tag, _member)
/// @brief 読み込み可能なStorage Columnを用意します。
///
/// 次の関数を生成します。
/// - GetName
/// - GetNameRange
/// - CountName
/// - EmptyName
///
/// @param Type 格納する要素型
/// @param _tag 変数が何か(関数名に使用される)
/// @param _member 内部メンバー変数名
///
/// @warning 取得した参照やRangeは再確保後に無効になります。
#define MD_STORAGE_READ_ONLY_COLUMN(Type, _tag, _member)\
public:\
	/* 値読み取り */\
	[[nodiscard("戻り値が無視されています")]] decltype(auto) Get##_tag(uint32_t _index) const\
	{\
		return MD::Storage::Detail::Get(_member, _index);\
	}\
	/* 配列の要素数 */\
	[[nodiscard("戻り値が無視されています")]] uint32_t Count##_tag() const\
	{\
		return MD::Storage::Detail::Count(_member);\
	}\
	/* 空チェック */\
	[[nodiscard("戻り値が無視されています")]] bool Empty##_tag() const noexcept\
	{\
		return MD::Storage::Detail::Empty(_member);\
	}\
	/* 変化不可配列取得(全探査用) */\
	[[nodiscard("戻り値が無視されています")]] auto Get##_tag##Range() const noexcept\
	{\
		return MD::Storage::Detail::GetRange(_member);\
	}\
private:\
	std::vector<Type> _member;

/// @def MD_STORAGE_WRITE_ORIGINAL_COLUMN(Type, _tag, _member)
/// @brief 独自にSetterを作成可能なStorage Columnを用意します。
///
/// 次の関数を生成します。
/// - GetName
/// - GetNameRange
/// - CountName
/// - EmptyName
///
/// @param Type 格納する要素型
/// @param _tag 変数が何か(関数名に使用される)
/// @param _member 内部メンバー変数名
///
/// @warning 取得した参照やRangeは再確保後に無効になります。
#define MD_STORAGE_WRITE_ORIGINAL_COLUMN(Type, _tag, _member)\
	MD_STORAGE_READ_ONLY_COLUMN(Type, _tag, _member);

/// @def MD_STORAGE_BUFFER_COLUMN(Type, _tag, _member)
/// @brief Buffer用変数の追加
///
/// 次の関数を生成します。
/// - GetName
/// - EditName
/// - SetName
/// - GetNameRange
/// - EditNameRange
/// - CountName
/// - EmptyName
///
/// @param Type 格納する要素型
/// @param _tag 変数が何か(関数名に使用される)
/// @param _member 内部メンバー変数名
///
/// @warning 取得した参照やRangeは再確保後に無効になります。
#define MD_STORAGE_BUFFER_COLUMN(Type, _tag, _member)\
public:\
	/* 値読み取り */\
	[[nodiscard("戻り値が無視されています")]] decltype(auto) Get##_tag(uint32_t _index) const\
	{\
		return MD::Storage::Detail::Get(_member, _index);\
	}\
	/* 値参照渡しで変更できる */\
	[[nodiscard("戻り値が無視されています")]] decltype(auto) Edit##_tag(uint32_t _index)\
	{\
		return MD::Storage::Detail::Edit(_member, _index);\
	}\
	/* 値代入 */\
	void Set##_tag(uint32_t _index, const Type& _value)\
	{\
		MD::Storage::Detail::Set(_member, _index, _value);\
	}\
	/* 配列の要素数 */\
	[[nodiscard("戻り値が無視されています")]] uint32_t Count##_tag() const noexcept\
	{\
		return MD::Storage::Detail::Count(_member);\
	}\
	/* 空チェック */\
	[[nodiscard("戻り値が無視されています")]] bool Empty##_tag() const noexcept\
	{\
		return MD::Storage::Detail::Empty(_member);\
	}\
	/* 変化不可配列取得(全探査用) */\
	[[nodiscard("戻り値が無視されています")]] auto Get##_tag##Range() const noexcept\
	{\
		return MD::Storage::Detail::GetRange(_member);\
	}\
	/* 変化可能配列取得(全探査用) */\
	[[nodiscard("戻り値が無視されています")]] auto Edit##_tag##Range() noexcept\
	{\
		return MD::Storage::Detail::EditRange(_member);\
	}\
	/* 追加生成 */\
	template<class... Args>\
	void Emplace##_tag(Args&&... _args)\
	{\
		MD::Storage::Detail::Emplace(_member, std::forward<Args>(_args)...);\
	}\
	/* 追加 */\
	void Add##_tag(const Type& _value)\
	{\
		MD::Storage::Detail::Add(_member, _value);\
	}\
	/* メモリ確保 */\
	void Reserve##_tag(uint32_t _size)\
	{\
		MD::Storage::Detail::Reserve(_member, _size);\
	}\
	/* 全要素削除 */\
	void Clear##_tag() noexcept\
	{\
		MD::Storage::Detail::Clear(_member);\
	}\
private:\
	std::vector<Type> _member;

/// @def MD_OWNED_STORAGE_READ_WRITE_ACCESSORS(IDType, _tag, _storageName, _member, _memberTag)
/// @brief 読み書き可能な子ストレージのラッパー関数を用意します。
///
/// 次の関数を生成します。
/// - GetName
/// - EditName
/// - SetName
/// - GetNameRange
/// - EditNameRange
/// - CountName
/// - EmptyName
///
/// @param IDType 対応するIDの型
/// @param MemberType 変数の型
/// @param _tag 変数が何か(関数名に使用される)
/// @param _storageName ストレージの変数名
/// @param _memberTag ストレージ内の公開したい変数のタグ
///
/// @warning 取得した参照やRangeは再確保後に無効になります。
/// @warning 対応したStorageじゃない場合エラーになるときがあります。
#define MD_OWNED_STORAGE_READ_WRITE_ACCESSORS(IDType, MemberType, _tag, _storageName, _memberTag)\
public:\
	/* 値読み取り */\
	[[nodiscard("戻り値が無視されています")]] decltype(auto) Get##_tag(IDType _id) const\
	{\
		return _storageName->Get##_memberTag(GetDenseIndex(_id));\
	}\
	/* 値参照渡しで変更できる */\
	[[nodiscard("戻り値が無視されています")]] decltype(auto) Edit##_tag(IDType _id)\
	{\
		return _storageName->Edit##_memberTag(GetDenseIndex(_id));\
	}\
	/* 値代入 */\
	void Set##_tag(IDType _id, const MemberType& _value)\
	{\
		_storageName->Set##_memberTag(GetDenseIndex(_id), _value);\
	}\
	/* 配列の要素数 */\
	[[nodiscard("戻り値が無視されています")]] uint32_t Count##_tag() const noexcept\
	{\
		return _storageName->Count##_memberTag();\
	}\
	/* 空チェック */\
	[[nodiscard("戻り値が無視されています")]] bool Empty##_tag() const noexcept\
	{\
		return _storageName->Empty##_memberTag();\
	}\
	/* 変化不可配列取得(全探査用) */\
	[[nodiscard("戻り値が無視されています")]] auto Get##_tag##Range() const noexcept\
	{\
		return _storageName->Get##_memberTag##Range();\
	}\
	/* 変化可能配列取得(全探査用) */\
	[[nodiscard("戻り値が無視されています")]] auto Edit##_tag##Range() noexcept\
	{\
		return _storageName->Edit##_memberTag##Range();\
	}

/// @def MD_OWNED_STORAGE_READ_WRITE_ACCESSORS(IDType, _tag, _storageName, _member, _memberTag)
/// @brief 読み込み可能な子ストレージのラッパー関数を用意します。
///
/// 次の関数を生成します。
/// - GetName
/// - CountName
/// - EmptyName
/// - GetNameRange
///
/// @param IDType 対応するIDの型
/// @param MemberType 変数の型
/// @param _tag 変数が何か(関数名に使用される)
/// @param _storageName ストレージの変数名
/// @param _memberTag ストレージ内の公開したい変数のタグ
///
/// @warning 取得した参照やRangeは再確保後に無効になります。
/// @warning 対応したStorageじゃない場合エラーになるときがあります。
#define MD_OWNED_STORAGE_READ_ONLY_ACCESSORS(IDType, MemberType, _tag, _storageName, _memberTag)\
public:\
	/* 値読み取り */\
	[[nodiscard("戻り値が無視されています")]] decltype(auto) Get##_tag(IDType _id) const\
	{\
		return _storageName->Get##_memberTag(GetDenseIndex(_id));\
	}\
	/* 配列の要素数 */\
	[[nodiscard("戻り値が無視されています")]] uint32_t Count##_tag() const noexcept\
	{\
		return _storageName->Count##_memberTag();\
	}\
	/* 空チェック */\
	[[nodiscard("戻り値が無視されています")]] bool Empty##_tag() const noexcept\
	{\
		return _storageName->Empty##_memberTag();\
	}\
	/* 変化不可配列取得(全探査用) */\
	[[nodiscard("戻り値が無視されています")]] auto Get##_tag##Range() const noexcept\
	{\
		return _storageName->Get##_memberTag##Range();\
	}\

/// @def MD_OWNED_STORAGE_WRITE_ORIGINAL_ACCESSORS(IDType, MemberType, _tag, _storageName, _memberTag)
/// @brief 独自にSetterを作成可能な子ストレージのラッパー関数を用意します。
///
/// 次の関数を生成します。
/// - GetName
/// - CountName
/// - EmptyName
/// - GetNameRange
///
/// @param IDType 対応するIDの型
/// @param MemberType 変数の型
/// @param _tag 変数が何か(関数名に使用される)
/// @param _storageName ストレージの変数名
/// @param _memberTag ストレージ内の公開したい変数のタグ
///
/// @warning 取得した参照やRangeは再確保後に無効になります。
/// @warning 対応したStorageじゃない場合エラーになるときがあります。
#define MD_OWNED_STORAGE_WRITE_ORIGINAL_ACCESSORS(IDType, MemberType, _tag, _storageName, _memberTag)\
	MD_OWNED_STORAGE_READ_ONLY_ACCESSORS(IDType, MemberType, _tag, _storageName, _memberTag);


/// @def MD_STORAGE_ID_READ_WRITE_COLUMN(IDType, Type, _tag, _member)
/// @brief 読み書き可能なStorage Columnを用意します。
///
/// 次の関数を生成します。
/// - GetName
/// - EditName
/// - SetName
/// - GetNameRange
/// - EditNameRange
/// - CountName
/// - EmptyName
///
/// @param Type 格納する要素型
/// @param _tag 変数が何か(関数名に使用される)
/// @param _member 内部メンバー変数名
///
/// @warning 取得した参照やRangeは再確保後に無効になります。
#define MD_STORAGE_ID_READ_WRITE_COLUMN(IDType, Type, _tag, _member)\
public:\
	/* 値読み取り */\
	[[nodiscard("戻り値が無視されています")]] decltype(auto) Get##_tag(IDType _id) const\
	{\
		return MD::Storage::Detail::Get(_member, GetDenseIndex(_id));\
	}\
	/* 値参照渡しで変更できる */\
	[[nodiscard("戻り値が無視されています")]] decltype(auto) Edit##_tag(IDType _id)\
	{\
		return MD::Storage::Detail::Edit(_member, GetDenseIndex(_id));\
	}\
	/* 値代入 */\
	void Set##_tag(IDType _id, const Type& _value)\
	{\
		MD::Storage::Detail::Set(_member, GetDenseIndex(_id), _value);\
	}\
	/* 配列の要素数 */\
	[[nodiscard("戻り値が無視されています")]] uint32_t Count##_tag() const noexcept\
	{\
		return MD::Storage::Detail::Count(_member);\
	}\
	/* 空チェック */\
	[[nodiscard("戻り値が無視されています")]] bool Empty##_tag() const noexcept\
	{\
		return MD::Storage::Detail::Empty(_member);\
	}\
	/* 変化不可配列取得(全探査用) */\
	[[nodiscard("戻り値が無視されています")]] auto Get##_tag##Range() const noexcept\
	{\
		return MD::Storage::Detail::GetRange(_member);\
	}\
	/* 変化可能配列取得(全探査用) */\
	[[nodiscard("戻り値が無視されています")]] auto Edit##_tag##Range() noexcept\
	{\
		return MD::Storage::Detail::EditRange(_member);\
	}\
private:\
	std::vector<Type> _member;

/// @def MD_STORAGE_ID_READ_ONLY_COLUMN(IDType, Type, _tag, _member)
/// @brief 読み込み可能なStorage Columnを用意します。
///
/// 次の関数を生成します。
/// - GetName
/// - GetNameRange
/// - CountName
/// - EmptyName
///
/// @param Type 格納する要素型
/// @param _tag 変数が何か(関数名に使用される)
/// @param _member 内部メンバー変数名
///
/// @warning 取得した参照やRangeは再確保後に無効になります。
#define MD_STORAGE_ID_READ_ONLY_COLUMN(IDType, Type, _tag, _member)\
public:\
	/* 値読み取り */\
	[[nodiscard("戻り値が無視されています")]] decltype(auto) Get##_tag(IDType _id) const\
	{\
		return MD::Storage::Detail::Get(_member, GetDenseIndex(_id));\
	}\
	/* 配列の要素数 */\
	[[nodiscard("戻り値が無視されています")]] uint32_t Count##_tag() const\
	{\
		return MD::Storage::Detail::Count(_member);\
	}\
	/* 空チェック */\
	[[nodiscard("戻り値が無視されています")]] bool Empty##_tag() const noexcept\
	{\
		return MD::Storage::Detail::Empty(_member);\
	}\
	/* 変化不可配列取得(全探査用) */\
	[[nodiscard("戻り値が無視されています")]] auto Get##_tag##Range() const noexcept\
	{\
		return MD::Storage::Detail::GetRange(_member);\
	}\
private:\
	std::vector<Type> _member;

/// @def MD_STORAGE_ID_WRITE_ORIGINAL_COLUMN(IDType, Type, _tag, _member)
/// @brief 独自にSetterを作成可能なStorage Columnを用意します。
///
/// 次の関数を生成します。
/// - GetName
/// - GetNameRange
/// - CountName
/// - EmptyName
///
/// @param Type 格納する要素型
/// @param _tag 変数が何か(関数名に使用される)
/// @param _member 内部メンバー変数名
///
/// @warning 取得した参照やRangeは再確保後に無効になります。
#define MD_STORAGE_ID_WRITE_ORIGINAL_COLUMN(IDType, Type, _tag, _member)\
	MD_STORAGE_ID_READ_ONLY_COLUMN(IDType, Type, _tag, _member);
