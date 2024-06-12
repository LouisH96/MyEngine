#pragma once
#include <Geometry\ModelTopology.h>
#include "MeshData.h"
#include <Rendering\Buffers\Buffer.h>
#include <Rendering\Buffers\IdxBuffer.h>
#include <Rendering\State\PrimitiveTopology.h>

namespace MyEngine
{
	//---| Main Classes |---
	template<ModelTopology Topology>
	class MeshBuffersCommon
	{
	public:
		void ActivateTopology();
	};

	template<typename Vertex, ModelTopology Topology>
	class MeshBuffersWithoutIndices
		: public MeshBuffersCommon<Topology>
	{
	public:
		void ActivateAndDraw();
		void CreateBuffers(MeshData<Vertex, Topology>& data);

		Rendering::Buffer<Vertex>VertexBuffer;
	};

	template<typename Vertex, ModelTopology Topology>
	class MeshBuffersWithIndices
		: public MeshBuffersCommon<Topology>
	{
	public:
		void Activate();
		void ActivateAndDraw();
		void CreateBuffers(MeshData<Vertex, Topology>& data);

		Rendering::Buffer<Vertex> VertexBuffer;
		Rendering::IdxBuffer IndexBuffer;
	};

	template<typename Vertex, ModelTopology Topology>
	class MeshBuffers
	{};

	//---| Specialized Classes |---
	template<typename Vertex>
	class MeshBuffers<Vertex, ModelTopology::LineStrip>
		: public MeshBuffersWithoutIndices<Vertex, ModelTopology::LineStrip>
	{};
	template<typename Vertex>
	class MeshBuffers<Vertex, ModelTopology::TriangleList>
		: public MeshBuffersWithoutIndices<Vertex, ModelTopology::TriangleList>
	{};
	template<typename Vertex>
	class MeshBuffers<Vertex, ModelTopology::TriangleStrip>
		: public MeshBuffersWithoutIndices<Vertex, ModelTopology::TriangleStrip>
	{};
	template<typename Vertex>
	class MeshBuffers<Vertex, ModelTopology::TriangleListIdx>
		: public MeshBuffersWithIndices<Vertex, ModelTopology::TriangleListIdx>
	{};
	template<typename Vertex>
	class MeshBuffers<Vertex, ModelTopology::TriangleStripIdx>
		: public MeshBuffersWithIndices<Vertex, ModelTopology::TriangleStripIdx>
	{};

	//---| Main Implementations |---
	template<ModelTopology Topology>
	inline void MeshBuffersCommon<Topology>::ActivateTopology()
	{
		Rendering::PrimitiveTopology::Activate(Topology);
	}

	//---| Specialized Implemantions |---
	template<typename Vertex, ModelTopology Topology>
	inline void MeshBuffersWithoutIndices<Vertex, Topology>::ActivateAndDraw()
	{
		MeshBuffersCommon<Topology>::ActivateTopology();
		VertexBuffer.Activate(0);
		VertexBuffer.Draw();
	}
	template<typename Vertex, ModelTopology Topology>
	inline void MeshBuffersWithoutIndices<Vertex, Topology>::CreateBuffers(MeshData<Vertex, Topology>& data)
	{
		VertexBuffer = Rendering::Buffer<Vertex>(data.Vertices, false);
	}
	template<typename Vertex, ModelTopology Topology>
	inline void MeshBuffersWithIndices<Vertex, Topology>::Activate()
	{
		MeshBuffersCommon<Topology>::ActivateTopology();
		VertexBuffer.Activate(0);
		IndexBuffer.Activate();
	}
	template<typename Vertex, ModelTopology Topology>
	inline void MeshBuffersWithIndices<Vertex, Topology>::ActivateAndDraw()
	{
		MeshBuffersCommon<Topology>::ActivateTopology();
		VertexBuffer.Activate(0);
		IndexBuffer.Activate();
		IndexBuffer.Draw();
	}
	template<typename Vertex, ModelTopology Topology>
	inline void MeshBuffersWithIndices<Vertex, Topology>::CreateBuffers(MeshData<Vertex, Topology>& data)
	{
		VertexBuffer = Rendering::Buffer<Vertex>(data.Vertices, false);
		IndexBuffer = Rendering::IdxBuffer{ data.Indices };
	}
}