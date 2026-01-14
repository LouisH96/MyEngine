#pragma once

#include <App\ResizedEvent.h>
#include <Geometry\ModelTopology.h>
#include <Geometry\Shapes\Rects.h>
#include <Math\Vectors.h>
#include <Rendering\Buffers\Buffer.h>
#include <Rendering\Buffers\InvalidateBuffer.h>
#include <Rendering\State\InputLayout.h>
#include <Rendering\State\Shader.h>
#include <Rendering\Structs\InstanceTypes.h>
#include <Rendering\Structs\VertexTypes.h>

namespace MyEngine
{
namespace Ui
{
class UiRectRenderer
{
public:
	using Vertex = Rendering::V_Pos2;
	using Instance = Rendering::I_Pos3Size2Col;

public:
	UiRectRenderer();

	void Render();

	void Remove(unsigned id);
	unsigned Add(RectFloat rect, const Float3& color);

	void SetColor(unsigned id, const Float3& color);
	void MoveTo(unsigned id, Float2 leftBot);

	Float3 GetColor(unsigned id) const;

private:
	static constexpr ModelTopology TOPOLOGY{ ModelTopology::TriangleStrip };

	Rendering::Buffer<Vertex> m_Vertices;
	InvalidateBuffer<Instance> m_Instances;

	Rendering::Shader m_Shader;
	Rendering::InputLayout m_InputLayout;
};
}
}