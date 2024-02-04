#pragma once
#include "Rendering/Buffers/Buffer.h"

namespace MyEngine
{
	namespace Rendering
	{
		class IdxBuffer;

		class ModelLoader
		{
		public:
			static void Load(const std::wstring& path, float scale, Buffer<V_PosColNorm>& vertices, const Float3& color);
		};
	}
}
