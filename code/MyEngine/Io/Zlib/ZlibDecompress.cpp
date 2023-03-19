#include "pch.h"
#include "ZlibDecompress.h"

#include <sstream>

#include "Io/Binary/Deflate.h"

void Io::Zlib::ZlibDecompress::Unzip(std::istream& stream)
{
	//https://www.rfc-editor.org/rfc/rfc1950
	//CMF
	const unsigned char cmf = stream.get();
	const char compressionMethod = cmf & 0xF;
	const char compressionInfo = cmf >> 4;
	if (compressionMethod != 8)
	{
		Logger::PrintError("CompressionMethod is not DEFLATE");
		return;
	}
	if (compressionInfo != 7)
	{
		Logger::PrintError("CompressionInfo does not indicate a 32K window");
		return;
	}
	//FLG
	const unsigned char flag = stream.get();
	const uint8_t fCheck = flag & 0xF;
	const bool fDict = flag & 0x1'0000;
	const uint8_t fLevel = flag & 0x110'0000;
	if ((cmf * 256 + flag) % 31 != 0)
	{
		Logger::PrintError("FlagCheck (/31) failed");
		return;
	}

	//DEFLATE
	Binary::Deflate::Decompress(stream);
}
