#pragma once
#include <string>

namespace MyEngine
{
	namespace Io
	{
		class AsciiReader
		{
		public:
			//---| ValueType |---
			enum ValueType
			{
				Number, String, Other
			};
			static ValueType DetectValueType(std::istream& stream);
			static bool IsNumber(char c);

			ValueType DetectedValueType() const { return DetectValueType(m_Stream); }

			//---| Read |---
			static std::string ReadFrom(std::istream& stream, const std::streampos& pos);

			std::string ReadFrom(const std::streampos& pos) { return ReadFrom(m_Stream, pos); }

			//---| Other |---
			static void Move(std::istream& stream, int amount);

			void Move(int amount) { Move(m_Stream, amount); }

			//---| Class |---
			explicit AsciiReader(std::istream& stream);
			std::istream& GetStream() { return m_Stream; }
			bool GetChar(char& c);
			char GetChar();
			char PeekChar();
			std::streampos GetPos() const;
			void Ignore(unsigned amount);
			void IgnoreLine();
			void IgnoreUntil(char c);
			std::string GetUntil(char delim);
			double GetDouble(char separator = '.');
			int GetInteger();
			std::string GetString();

		private:
			std::istream& m_Stream;
		};
	}
}
