#pragma once

#include <DataStructures\Array.h>
#include <DataStructures\List.h>
#include "Drive.h"

namespace MyEngine
{
	class FileUtils
	{
		//---| Drives |---
	public:
		static Array<Drive> GetDrives();
		static Array<Drive> GetDrives(Drive::Type driveType);

	private:
		static constexpr unsigned QUERY_DOS_DEVICE_RESULT_LENGTH{ 128 };

		static Array<wchar_t> GetLogicalDriveLetters();
		static Drive::Type FindType(const std::wstring& queryResult);

		//---| Folders |---
	public:
		static List<std::wstring> GetFiles(const std::wstring directory);

		//---| Files |---
		static std::wstring GetFileName(const std::wstring fullFilePath);
	};
}
