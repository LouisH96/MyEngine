#pragma once
#include "List.h"
#include "Logger/Logger.h"

namespace MyEngine
{
	template<typename TKey, typename TValue>
	class Dictionary
	{
	public:
		struct Entry
		{
			TKey Key;
			TValue Value;
		};

		Dictionary() = default;

		void Add(TKey key, TValue value);
		const TValue* Get(TKey key) const;

	private:
		List<Entry> m_Entries{};
	};

	template <typename TKey, typename TValue>
	void Dictionary<TKey, TValue>::Add(TKey key, TValue value)
	{
		//todo: binary lookup
		for (unsigned i = 0; i < m_Entries.GetSize(); i++)
			if (key > m_Entries[i].Key)
			{
				m_Entries.Insert(i, { key, value });
				return;
			}
		m_Entries.Add({ key, value });
	}

	template <typename TKey, typename TValue>
	const TValue* Dictionary<TKey, TValue>::Get(TKey key) const
	{
		//todo: binary lookup
		for (unsigned i = 0; i < m_Entries.GetSize(); i++)
			if (key == m_Entries[i].Key)
				return &m_Entries[i].Value;
		return nullptr;
	}
}
