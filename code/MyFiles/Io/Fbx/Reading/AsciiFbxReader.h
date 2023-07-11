#pragma once
#include <istream>

#include "Io/Fbx/Reading/FbxFile.h"

namespace MyEngine
{
	namespace Io
	{
		class AsciiReader;

		namespace Fbx
		{
			namespace Reading
			{
				class AsciiFbxReader
				{
				public:
					static FbxFile Read(std::istream& stream);

				private:
					static void SkipUseless(AsciiReader& reader);

					static void ReadElement(AsciiReader& reader, FbxElement& element);
					static void ReadChildren(AsciiReader& reader, FbxElement& parent);

					static void ReadProperties(AsciiReader& reader, FbxElement& element);
					static FbxProperty* ReadNextProperty(AsciiReader& reader);

					static FbxProperty* ReadNumberProperty(AsciiReader& reader);
					static FbxProperty* ReadStringProperty(AsciiReader& reader);
					static FbxProperty* ReadArrayProperty(AsciiReader& reader);
				};
			}
		}
	}
}
