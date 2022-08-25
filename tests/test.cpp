#include "pch.h"
#include "../source/Logging/Logger.h"


TEST(TestCaseName, TestName) {
	using Logger = MyEngine::Logging::Logger;

	constexpr int expected{ 12 + 5 };
	const int result = static_cast<int>(Logger::GetLogLine("Hallo").length());

	EXPECT_EQ(expected, result) << "Length of GetLogLine result is not expected";
}