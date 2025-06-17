#include "pch.h"
#include "DebugLinesScreenSpace.h"

#include <Rendering\Structs\VertexTypes.h>
#include <App\Win32\Window.h>

using namespace Debug;
using namespace Rendering;

DebugLinesScreenSpace::DebugLinesScreenSpace()
	: m_Shader{ Resources::GlobalShader(L"ScreenPixelSpace.hlsl") }
	, m_InputLayout{ InputLayout::FromType<V_Pos2Col>() }
	, m_Vertices{ 8, ModelTopology::LineList }
{
}

void DebugLinesScreenSpace::Init()
{
	SetCameraBuffer(WINDOW.GetClientSizeF());
}

void DebugLinesScreenSpace::Render()
{
	m_Shader.Activate();
	m_InputLayout.Activate();

	m_CameraBuffer.Activate();

	m_Vertices.Draw();
	m_Vertices.Clear();
}

void DebugLinesScreenSpace::OnCanvasResized(const App::ResizedEvent& canvasResized)
{
	SetCameraBuffer(Float2{ canvasResized.NewSize });
}

void DebugLinesScreenSpace::DrawLine(
	const Float2& begin, const Float2& end,
	const Float3& color)
{
	m_Vertices.Add(Vertex{ begin, color });
	m_Vertices.Add(Vertex{ end, color });
}

void DebugLinesScreenSpace::DrawRay(
	const Float2& begin, const Float2& displacement,
	const Float3& color)
{
	m_Vertices.Add(Vertex{ begin, color });
	m_Vertices.Add(Vertex{ begin + displacement, color });
}

void DebugLinesScreenSpace::SetCameraBuffer(const Float2& screenSize)
{
	//Operation in shader:
	// pixel.x = vertex.x * cameraBuffer.multiplier.x - 1;
	// Result: (x/screenWidth) * 2 - 1 

	CameraBuffer buffer{};
	buffer.Multiplier.x = 2.f / screenSize.x;
	buffer.Multiplier.y = 2.f / screenSize.y;
	m_CameraBuffer.Update(buffer);
}
