#pragma once

#include <Rendering\Structs\VertexTypes.h>
#include <Rendering\State\Shader.h>
#include <Rendering\DrawData\VertexList.h>

namespace MyEngine::Debug
{
class DebugLinesScreenSpace
{
public:
	DebugLinesScreenSpace();

	void Init();
	void Render();
	void OnCanvasResized(const App::ResizedEvent& resizedEvent);

	void DrawLine(const Float2& begin, const Float2& end, const Float3& color);
	void DrawRay(const Float2& begin, const Float2& enddisplacement, const Float3& color);

private:
	using Vertex = Rendering::V_Pos2Col;
	struct CameraBuffer
	{
		Float2 Multiplier;
		float Padding[2];
	};

	Rendering::InputLayout m_InputLayout;
	Rendering::Shader m_Shader;
	Rendering::VertexList<Vertex> m_Vertices{};
	Rendering::ConstantBuffer<CameraBuffer> m_CameraBuffer;

	void SetCameraBuffer(const Float2& screenSize);
};
}