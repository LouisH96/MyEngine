#pragma once
#include <cstdint>

#include "FbxElement.h"

namespace MyEngine
{
	namespace Io
	{
		namespace Fbx
		{
			namespace Reading
			{
				class FbxFile
				{
				public:
					void SetVersion(uint32_t version);
					uint32_t GetVersion() const { return m_Version; }

					void SetIsBinary(bool isBinary) { m_IsBinary = isBinary; }
					bool IsBinary() const { return m_IsBinary; }

					const FbxElement& GetRoot() const { return m_Root; }
					FbxElement& GetRoot() { return m_Root; }

				private:
					uint32_t m_Version{};
					bool m_IsBinary{};
					FbxElement m_Root{};
				};
			}
		}
	}
}
