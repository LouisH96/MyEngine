#include "pch.h"
#include "Logger.h"

#include <iostream>
#include <locale>
#include <iomanip>
#include <sstream>
#include "Math/Int2.h"

std::string MyEngine::Logging::Logger::GetLogLine(const std::string& logMessage)
{
	tm tm{};
	const time_t now = time(nullptr);
	localtime_s(&tm, &now);

	constexpr unsigned timeFormatLength = 11;
	std::string datetime(timeFormatLength + logMessage.size() + 1, 0);
	datetime[datetime.size() - 1] = '\n';
	std::strftime(&datetime[0], timeFormatLength + 1, "[%T] ", &tm);

	datetime.replace(timeFormatLength, logMessage.size(), logMessage);
	return datetime;
}

void MyEngine::Logging::Logger::Print(const std::string& logMessage)
{
	std::cout << GetLogLine(logMessage);
}

void MyEngine::Logging::Logger::Print(float x, float y, float z)
{
	Print(ToString(x, y, z));
}

void MyEngine::Logging::Logger::Print(const DirectX::XMFLOAT3& vector)
{
	Print(ToString(vector));
}

void MyEngine::Logging::Logger::PrintXYZ(const DirectX::XMVECTOR& vector)
{
	Print(ToStringXYZ(vector));
}

void MyEngine::Logging::Logger::PrintXYZ(const float* x)
{
	Print(ToStringXYZ(x));
}

void Logger::PrintXY(const Math::Int2& vector)
{
	Print(ToStringXY(vector));
}

void Logger::PrintXY(const std::string& prefix, const Math::Int2& vector)
{
	std::stringstream ss;
	ss << prefix;
	ss << ToStringXY(vector);
	Print(ss.str());
}

std::string Logger::ToStringXY(const Math::Int2& vector)
{
	std::stringstream ss{};
	ss << "(";
	ss << std::to_string(vector.x);
	ss << ",";
	ss << std::to_string(vector.y);
	ss << ")";
	return ss.str();
}

std::string MyEngine::Logging::Logger::ToStringXYZ(const float* x)
{
	return ToString(x[0], x[1], x[2]);
}

std::string MyEngine::Logging::Logger::ToString(float x, float y, float z)
{
	std::stringstream ss{};
	ss << "(";
	ss << std::to_string(x);
	ss << ",";
	ss << std::to_string(y);
	ss << ",";
	ss << std::to_string(z);
	ss << ")";
	return ss.str();
}

std::string MyEngine::Logging::Logger::ToString(const DirectX::XMFLOAT3& vector)
{
	return ToString(vector.x, vector.y, vector.z);
}

std::string MyEngine::Logging::Logger::ToStringXYZ(const DirectX::XMVECTOR& vector)
{
	std::stringstream ss{};
	ss << "(";
	ss << std::to_string(DirectX::XMVectorGetX(vector));
	ss << ",";
	ss << std::to_string(DirectX::XMVectorGetY(vector));
	ss << ",";
	ss << std::to_string(DirectX::XMVectorGetZ(vector));
	ss << ")";
	return ss.str();
}
