#pragma once

#include <vector>

#include "PropertyChangeRecord.h"
struct SceneDocument;

/*
	実際にUndoとRedoを実装するクラス。
*/
class CommandHistory
{
public:
	CommandHistory(SceneDocument& _document);

	// 変数変更記録
	bool Record(PropertyChangeRecord& _record);

	// 巻き戻し
	bool Undo();

	// やり直し
	bool Redo();

	// 操作履歴をすべて消す
	void Clear();

	// 削除したComponentの履歴と編集中の記録だけを取り除く。
	void RemoveComponentRecords(std::uint64_t _objectID, std::uint64_t _componentID);

	// 操作前の値を1回だけ保存
	template<class T>
	bool BeginEdit(const PropertyTarget& _target, const T& _before);
	// 現在値と違っていれば、履歴を1件追加
	template<class T>
	void CommitEdit(const PropertyTarget& _target, const T& _current);
	// 操作前の値へ戻して終了
	void CancelEdit(const PropertyTarget& _target);

private:
	// テンプレートからSceneDocumentの完全な定義を要求しないための確認処理。
	bool HasProperty(const PropertyTarget& _target);
private:
	PropertyChangeRecord editingRecord;
	bool editing{ false };

	SceneDocument& document;

	std::vector<PropertyChangeRecord> records;
	std::size_t cursor{ 0 };
};

#include "CommandHistory.inl"