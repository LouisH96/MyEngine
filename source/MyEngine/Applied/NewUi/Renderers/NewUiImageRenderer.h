#pragma once
#include <Geometry\Shapes\Rects.h>

namespace MyEngine
{
namespace NewUi
{
class NewUiImageRenderer
{
public:
	NewUiImageRenderer();

	void Clear();
	void CreateBuffer();
	void OnCanvasResized(const App::ResizedEvent& event);

	void Render();

	unsigned AddTexture(Rendering::Texture&& texture);
	void ReplaceTexture(unsigned textureId, Rendering::Texture&& texture);

	void Add(const RectFloat& screenSpace, const Float2& uvLeftTop, const Float2& uvRightBot, unsigned textureId);

private:
	using Vertex = Rendering::V_Pos2Uv;
	static constexpr ModelTopology TOPOLOGY{ ModelTopology::TriangleList };
	struct TextureGroup {
		Rendering::Texture Texture;
		List<Vertex> Vertices{};
	};
	List<TextureGroup> m_Groups;

	Float2 m_2DivScreenSize;
	Rendering::InputLayout m_InputLayout;
	Rendering::Shader m_Shader;
	Rendering::VertexArray<Vertex> m_Vertices; //in ndc

	static Float2 Calc2InvScreenSize();
};
}
}

