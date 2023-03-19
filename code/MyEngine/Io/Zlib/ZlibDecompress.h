#pragma once
#include <vector>


namespace MyEngine
{
	namespace Io
	{
		namespace Zlib
		{
			class ZlibDecompress
			{
			public:
				ZlibDecompress() = delete;

				static std::vector<uint8_t> Unzip(std::istream& stream);
			};
		}
	}
}
