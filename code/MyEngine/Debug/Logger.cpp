#include "pch.h"
#include "Logger.h"

#include <Windows.h>
#include <iostream>
#include <locale>
#include <iomanip>
#include <sstream>

std::string Logger::GetLogLine(const std::string& logMessage)
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

void Logger::Print(const std::string& logMessage)
{
	std::cout << GetLogLine(logMessage);
}

void Logger::Print(const std::string& prefix, float value)
{
	std::stringstream ss;
	ss << prefix << value;
	Print(ss.str());
}

void Logger::Print(float x, float y, float z)
{
	Print(ToString(x, y, z));
}

void Logger::Print(const DirectX::XMFLOAT3& vector)
{
	Print(ToString(vector));
}

void Logger::PrintXYZ(const DirectX::XMVECTOR& vector)
{
	Print(ToStringXYZ(vector));
}

void Logger::PrintXYZ(const float* x)
{
	Print(ToStringXYZ(x));
}

void Logger::PrintXYZ(const Float3& vector)
{
	Print(ToStringXYZ(vector));
}

void Logger::PrintXYZ(const std::string& prefix, const Float3& vector)
{
	std::stringstream ss;
	ss << prefix;
	ss << ToStringXYZ(vector);
	Print(ss.str());
}

void Logger::PrintXY(const Int2& vector)
{
	Print(ToStringXY(vector));
}

void Logger::PrintXY(const std::string& prefix, const Int2& vector)
{
	std::stringstream ss;
	ss << prefix;
	ss << ToStringXY(vector);
	Print(ss.str());
}

void Logger::PrintError(const std::string& message)
{
	std::stringstream ss;
	ss << "[Error] ";
	ss << message;
	SetColorRed();
	Print(ss.str());
	SetColorWhite();
}

void Logger::PrintWarning(const std::string& message)
{
	std::stringstream ss;
	ss << "[Warning] ";
	ss << message;
	SetColorYellow();
	Print(ss.str());
	SetColorWhite();
}

std::string Logger::ToStringXY(const Int2& vector)
{
	std::stringstream ss{};
	ss << "(";
	ss << std::to_string(vector.x);
	ss << ",";
	ss << std::to_string(vector.y);
	ss << ")";
	return ss.str();
}

void Logger::SetColorWhite()
{
	SetColor(7);
}

void Logger::SetColorYellow()
{
	SetColor(14);
}

void Logger::SetColorRed()
{
	SetColor(12);
}

void Logger::SetColor(short attribute)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), attribute);
}

std::string Logger::ToStringXYZ(const float* x)
{
	return ToString(x[0], x[1], x[2]);
}

std::string Logger::ToStringXYZ(const Float3& vector)
{
	return ToString(vector.x, vector.y, vector.z);
}

std::string Logger::ToString(float x, float y, float z)
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

std::string Logger::ToString(const DirectX::XMFLOAT3& vector)
{
	return ToString(vector.x, vector.y, vector.z);
}

std::string Logger::ToStringXYZ(const DirectX::XMVECTOR& vector)
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
