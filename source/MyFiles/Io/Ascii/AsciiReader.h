#pragma once
#include <string>
#include <fstream>

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
			static bool IsWhiteSpace(char c);

			ValueType DetectedValueType() { return DetectValueType(m_Stream); }

			//---| Read |---
			static std::string ReadFrom(std::istream& stream, const std::streampos& pos);
			static std::string ReadUntil(std::istream& stream, char delim);
			static std::string ReadUntil(std::istream& stream, char delim1, char orDelim2);
			static bool ReadLine(std::istream& stream, std::string& string);

			std::string ReadFrom(const std::streampos& pos) { return ReadFrom(m_Stream, pos); }
			std::string ReadUntil(char delim);
			std::string ReadUntil(char delim1, char orDelim2);
			std::string ReadUntil(const std::streampos& end);
			bool ReadLine(std::string& string);
			std::string ReadLine();
			std::string ReadUntilWhiteSpace();
			
			std::string Read(const std::streampos& begin, const std::streampos& end);
			std::string ReadLineUntilAndSkip(const std::string& end, bool* pFound = nullptr);

			bool CheckAndSkip(const std::string& target);
			bool CheckEndOfLineAndSkip();
			bool CheckLineAndSkip(const std::string& target);
			bool CheckReadUntil(char delim, std::string& out);

			//---| Other |---
			static void Move(std::istream& stream, int amount);
			static void MoveBack(std::istream& stream, unsigned amount = 1);
			static void Reset(std::istream& stream);

			void Move(int amount) { Move(m_Stream, amount); }
			void MoveForward(unsigned amount = 1) { Move(static_cast<int>(amount)); }
			void MoveBack(unsigned amount = 1);
			void MoveTo(std::streampos pos);
			void Reset();

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
			explicit AsciiReader(const std::wstring& path);
			explicit AsciiReader(std::ifstream&& stream);
			std::istream& GetStream() { return m_Stream; }
			bool GetChar(char& c);
			char GetChar();
			char PeekChar();
			std::streampos GetPos();
			std::string GetUntil(char delim);
			double GetDouble(char separator = '.');
			int GetInteger();
			std::string GetString();
			bool Good();
			bool IsOpen();
			bool Eof();

		private:
			std::ifstream m_Stream;
		};
	}
}
