#pragma once
#include "DataStructures/InvalidateList.h"
#include "Generation/DiskGenerator.h"
#include "Rendering/Buffers/InvalidateBuffer.h"

namespace MyEngine
{
namespace Ui
{
class UiShapeRenderer
{
public:
	static constexpr ModelTopology TOPOLOGY{ ModelTopology::TriangleList };
	UiShapeRenderer();

	void OnCanvasResized(const App::ResizedEvent& event);
	void Render();

	//should be in a Drawer object
	void Remove(unsigned id);
	unsigned Rect(Float2 leftBot, Float2 size, const Float3& color);
	unsigned Line(Float2 begin, Float2 end, float thickness, const Float3& color);
	unsigned Circle(Float2 center, Generation::DiskGenerator<TOPOLOGY>::Options options, const Float3& color);

private:
	using Vertex = Rendering::V_Pos2Col;
	struct Shape
	{
		List<unsigned> Triangles{};

		bool IsValid() const { return !Triangles.IsEmpty(); }
		void Invalidate() { Triangles.Clear(); }
	};

	Rendering::Shader m_Shader;
	Rendering::InputLayout m_InputLayout;

	InvalidateList<Shape> m_Shapes;
	InvalidateBuffer<Vertex> m_Vertices;

	Float2 m_ToNdcMultiplier;

	class Adder
	{
	public:
		explicit Adder(Shape& shape, InvalidateBuffer<Vertex>& vertices);

		void Add(const Vertex& vertex);

	private:
		Shape& m_Shape;
		InvalidateBuffer<Vertex>& m_Vertices;
		unsigned m_NrVertices;
	};
};
}
}
