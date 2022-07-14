#include "CppUnitTest.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <regex>
#include "../source/Logging/Logger.h"

namespace TestProject_MyEngine
{
	using Logger = MyEngine::Logging::Logger;

	TEST_CLASS(TestClass_MyEngine)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			const std::string logLine = Logger::GetLogLine("Log Line");
			const std::regex regex(R"(\[\d\d:\d\d:\d\d\] Log Line\n)");

			const bool isMatch = std::regex_match(logLine, regex);
			Assert::IsTrue(isMatch);
		}
	};
}
