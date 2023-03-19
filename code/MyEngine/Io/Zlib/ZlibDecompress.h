#pragma once


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

				static void Unzip(std::istream& stream);
			};
		}
	}
}
