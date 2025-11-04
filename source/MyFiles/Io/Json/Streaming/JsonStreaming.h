#pragma once
#include <fstream>
#include <stack>

#include "Io/Json/JsonElement.h"

//need lots of cleaning
//performance can improve a lot
namespace MyEngine
{
	namespace Json
	{
		enum class StreamState
		{
			Root,
			InObject,
			Unknown
		};

		class JsonStreaming
		{
		public:
			JsonStreaming(const std::wstring& path);
			~JsonStreaming();


			std::string GetNextProperty();
			void GetInNextObject();
			void GetInArray();
			Io::Json::ElementType FindNextElementType();

			std::string GetNextStringValue();
			double GetNextNumberValue();
			void SkipNextScope();
			void SkipNextNull();
			void SkipNextBool();
			void SkipNextNumber();
			void SkipNextPropertyValue();
			void SkipNextProperty();
			void GetOutOfScope();
			void FindProperty(const std::string& propertyName);

			bool IsEndOfObject();
			bool IsEndOfArray();

		private:
			std::ifstream m_Stream;
			StreamState m_State;
			std::stack<char> m_Scopes{};

			static char FindNextSign(std::ifstream& stream);
			static std::string GetNextString(std::ifstream& stream);
			static void SkipWhiteSpace(std::ifstream& stream);
		};
	}
}
