#pragma once

#include <type_traits>
#include <utility>

#include "ObjectDocument.h"

template<class T>
T* ObjectDocument::GetComponent()
{
	static_assert(std::is_base_of_v<ComponentDocumentBase, T>);
	static_assert(!std::is_same_v<T, TransformComponentDocument>, "Transform is fixed. Use GetTransform().");

	const auto typeID = ComponentDocumentBase::TypeIDOf<T>();

	for (const auto& component : components)
	{
		if (component && component->GetTypeID() == typeID)
		{
			return static_cast<T*>(component.get());
		}
	}

	return nullptr;
}

template<class T>
const T* ObjectDocument::GetComponent() const
{
	static_assert(std::is_base_of_v<ComponentDocumentBase, T>);
	static_assert(!std::is_same_v<T, TransformComponentDocument>, "Transform is fixed. Use GetTransform().");

	const auto typeID = ComponentDocumentBase::TypeIDOf<T>();

	for (const auto& component : components)
	{
		if (component && component->GetTypeID() == typeID)
		{
			return static_cast<const T*>(component.get());
		}
	}

	return nullptr;
}

template<class T, class... Args>
T* ObjectDocument::AddComponent(Args&&... _args)
{
	static_assert(std::is_base_of_v<ComponentDocumentBase, T>);
	static_assert(!std::is_same_v<T, TransformComponentDocument>, "Transform is fixed. Use GetTransform().");

	auto component{ std::make_unique<T>(id, nextComponentID, std::forward<Args>(_args)...) };

	// 複数付けれるか判定
	if (!component->AllowsMultiple() && GetComponent<T>())
	{
		return nullptr;
	}

	T* result{ component.get()};

	components.push_back(std::move(component));
	++nextComponentID;

	return result;
}