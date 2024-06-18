#pragma once

#include <string>

namespace MyEngine
{
	class Drive
	{
	public:
		enum class Type
		{
			CdRom, Storage, Other
		};

		Drive() = default;
		Drive(wchar_t letter, Type type);

		wchar_t GetLetter() const { return m_Letter; }
		Type GetType() const { return m_Type; }

		std::wstring GetPath() const;

	private:
		wchar_t m_Letter{ '\0' };
		Type m_Type{ Type::Other };
	};
}

