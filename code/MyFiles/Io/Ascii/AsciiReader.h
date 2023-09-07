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
			static std::string ReadUntil(std::istream& stream, char delim);
			static std::string ReadUntil(std::istream& stream, char delim1, char orDelim2);
			static bool ReadLine(std::istream& stream, std::string& string);

			std::string ReadFrom(const std::streampos& pos) const { return ReadFrom(m_Stream, pos); }
			std::string ReadUntil(char delim) const;
			std::string ReadUntil(char delim1, char orDelim2) const;
			bool ReadLine(std::string& string) const;
			std::string ReadUntilWhiteSpace() const;

			//---| Other |---
			static void Move(std::istream& stream, int amount);
			static void MoveBack(std::istream& stream, unsigned amount = 1);
			static void Reset(std::istream& stream);

			void Move(int amount) const { Move(m_Stream, amount); }
			void MoveForward(unsigned amount = 1) const { Move(static_cast<int>(amount)); }
			void MoveBack(unsigned amount = 1) const;
			void Reset() const;

			//---| Ignore |---
			static void Ignore(std::istream& stream, unsigned amount = 1);
			static void Ignore(std::istream& stream, char c, unsigned amount);
			static void IgnoreUntil(std::istream& stream, char c);
			static void IgnoreLine(std::istream& stream);
			static void IgnoreLines(std::istream& stream, unsigned amount);

			void Ignore(unsigned amount);
			void Ignore(char c, unsigned amount);
			void IgnoreLine();
			void IgnoreLines(unsigned amount);
			void IgnoreUntil(char c);

			//---| Class |---
			explicit AsciiReader(std::istream& stream);
			std::istream& GetStream() { return m_Stream; }
			bool GetChar(char& c);
			char GetChar();
			char PeekChar();
			std::streampos GetPos() const;
			std::string GetUntil(char delim);
			double GetDouble(char separator = '.');
			int GetInteger();
			std::string GetString();

		private:
			std::istream& m_Stream;
		};
	}
}
