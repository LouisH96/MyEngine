#include "pch.h"
#include "../MyEngine/Debug/Logger.h"


TEST(TestCaseName, TestName) {
	using Logger = MyEngine::Debug::Logger;

	constexpr int expected{ 12 + 5 };
	const int result = static_cast<int>(Logger::GetLogLine("Hallo").length());

	EXPECT_EQ(expected, result) << "Length of GetLogLine result is not expected";
}