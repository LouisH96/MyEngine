#pragma once
#include <Rendering\Buffers\Buffer.h>
#include <Rendering\Mesh\MeshData.h>

namespace MyEngine
{
namespace Rendering
{
class IdxBuffer;

class ModelLoader
{
public:
	static void Load(const std::wstring& path, float scale, Buffer<V_PosColNorm>& vertices, const Float3& color);
	static void Load(const std::wstring& path, float scale, MeshData<V_PosNorUv, ModelTopology::TriangleList>& vertices);
};
}
}
