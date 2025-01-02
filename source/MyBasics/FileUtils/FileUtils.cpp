#include "FileUtils.h"

#include <Windows.h>

#include <Logger\Logger.h>
#include <String\StringUtils.h>

using namespace MyEngine;

Array<Drive> FileUtils::GetDrives()
{
	const Array<wchar_t> letters{ GetLogicalDriveLetters() };

	wchar_t queryResult[QUERY_DOS_DEVICE_RESULT_LENGTH];
	wchar_t query[3];
	query[1] = L':';
	query[2] = L'\0';

	Array<Drive> drives{ letters.GetSize() };
	for (unsigned i = 0; i < letters.GetSize(); ++i)
	{
		query[0] = letters[i];
		const DWORD resultLength{ QueryDosDevice(query, queryResult, QUERY_DOS_DEVICE_RESULT_LENGTH) };
		const Drive::Type type{ FindType(queryResult) };
		drives[i] = Drive{ letters[i], type };
	}

	return drives;
}

Array<Drive> FileUtils::GetDrives(Drive::Type driveType)
{
	static constexpr wchar_t emptyLetter{ L'\0' };

	Array<wchar_t> letters{ GetLogicalDriveLetters() };

	wchar_t queryResult[QUERY_DOS_DEVICE_RESULT_LENGTH];
	wchar_t query[3];
	query[1] = L':';
	query[2] = L'\0';

	//count
	unsigned count{ 0 };
	for (unsigned i = 0; i < letters.GetSize(); i++)
	{
		query[0] = letters[i];
		const DWORD resultLength{ QueryDosDevice(query, queryResult, QUERY_DOS_DEVICE_RESULT_LENGTH) };
		const Drive::Type type{ FindType(queryResult) };

		if (type == driveType)
			count++;
		else
			letters[i] = emptyLetter;
	}

	//make array
	Array<Drive> typeDrives{ count };

	for (unsigned iLetter = 0, iTypeDrive = 0; iLetter < letters.GetSize(); iLetter++)
		if (letters[iLetter] != emptyLetter)
			typeDrives[iTypeDrive++] = Drive{ letters[iLetter], driveType };

	return typeDrives;
}

Array<wchar_t> FileUtils::GetLogicalDriveLetters()
{
	static constexpr unsigned maxDrives{ 16 };
	static constexpr unsigned charPerDrive{ 4 };
	static constexpr unsigned bufferSize{ maxDrives * charPerDrive };

	wchar_t* pBuffer{ new wchar_t[bufferSize] };
	const DWORD resultLength{ GetLogicalDriveStrings(bufferSize, pBuffer) };

	Array<wchar_t> letters{ static_cast<unsigned>(resultLength / 4) };
	for (unsigned i = 0; i < letters.GetSize(); i++)
		letters[i] = pBuffer[i * 4];

	delete[] pBuffer;
	return letters;
}

Drive::Type FileUtils::FindType(const std::wstring& queryResult)
{
	static const std::wstring deviceString{ LR"(\Device)" };
	static const std::wstring hardDiskString{ LR"(\Harddisk)" };
	static const std::wstring cdRomString{ LR"(\CdRom)" };

	const wchar_t* pChar{ &queryResult[0] };

	if (!StringUtils::BeginsWith<wchar_t>(pChar, deviceString))
		return Drive::Type::Other;

	pChar += deviceString.size();

	if (StringUtils::BeginsWith<wchar_t>(pChar, hardDiskString))
		return Drive::Type::Storage;

	if (StringUtils::BeginsWith<wchar_t>(pChar, cdRomString))
		return Drive::Type::CdRom;

	return Drive::Type::Other;
}

List<std::wstring> FileUtils::GetFiles(const std::wstring directory)
{
	const std::wstring pattern{ directory + LR"(\*)" };

	List < std::wstring> files{};
	WIN32_FIND_DATA data;
	HANDLE findHandle;

	if ((findHandle = FindFirstFile(pattern.c_str(), &data)) == INVALID_HANDLE_VALUE)
	{
		Logger::PrintError("[FileUtils::ListFiles] Invalid handle value");
		return files;
	}

	do
	{
		if (data.cFileName[0] != '.')
			files.Add(data.cFileName);
	} while (FindNextFile(findHandle, &data) != 0);

	FindClose(findHandle);

	return files;
}

std::wstring FileUtils::GetFileName(const std::wstring fullFilePath)
{
	int begin = 0, end = 0;

	int i = static_cast<int>(fullFilePath.size());

	for (; i >= 0; i--)
	{
		if (fullFilePath[i] == '.')
		{
			end = i;
			break;
		}
	}

	for (; i >= 0; i--)
	{
		if (fullFilePath[i] == '\\' || fullFilePath[i] == '/')
		{
			begin = i;
			break;
		}
	}

	return fullFilePath.substr(begin, end - begin);
}

std::wstring FileUtils::GetFolder(const std::wstring fullFilePath)
{
	size_t end{ fullFilePath.size() - 1 };

	while (end > 0)
	{
		const wchar_t endChar{ fullFilePath[end] };
		if (endChar == '\\' || endChar == '/')
			return fullFilePath.substr(0, end + 1);
		--end;
	}
	return fullFilePath;
}
