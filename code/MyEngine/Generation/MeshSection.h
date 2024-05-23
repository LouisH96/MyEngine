#pragma once
#include <Geometry\ModelTopology.h>
#include <Rendering\Mesh\MeshData.h>

namespace MyEngine
{
	namespace Generation
	{
		//---| Main Classes |---
		template<typename Vertex, ModelTopology Topology>
		class MeshSectionWithIndices
		{
		public:
			using VertexType = Vertex;

			MeshSectionWithIndices(MeshData<Vertex, Topology>& meshData);

			MeshSectionWithIndices(MeshData<Vertex, Topology>& meshData,
				unsigned vertexStart, unsigned vertexEnd,
				unsigned indexStart, unsigned indexEnd);

			void SetVertexEnd(unsigned vertexEnd) { m_VertexEnd = vertexEnd; }
			void SetIndexEnd(unsigned indexEnd) { m_IndexEnd = indexEnd; }

			template<typename Adaptor>
			void Adapt(Adaptor adaptor);

			unsigned GetVertexStart() const { return m_VertexStart; }
			unsigned GetIndexStart() const { return m_IndexStart; }
			unsigned GetNrVertices() const { return m_VertexEnd - m_VertexStart; }
			unsigned GetNrIndices() const { return m_IndexEnd - m_IndexStart; }

		protected:
			MeshData<Vertex, Topology>& m_MeshData;
			unsigned m_VertexStart;
			unsigned m_VertexEnd;
			unsigned m_IndexStart;
			unsigned m_IndexEnd;
		};

		template<typename Vertex, ModelTopology Topology>
		class MeshSection
		{
		};

		template<typename Vertex>
		class MeshSection<Vertex, ModelTopology::TriangleListIdx>
			: public MeshSectionWithIndices<Vertex, ModelTopology::TriangleListIdx>
		{
		public:
			MeshSection(MeshData<Vertex, ModelTopology::TriangleListIdx>& meshData);

			MeshSection(MeshData<Vertex, ModelTopology::TriangleListIdx>& meshData,
				unsigned vertexStart, unsigned vertexEnd,
				unsigned indexStart, unsigned indexEnd);

			MeshSection<Vertex, ModelTopology::TriangleListIdx> Copy();
		};

		template<typename Vertex, ModelTopology Topology>
		inline MeshSectionWithIndices<Vertex, Topology>::MeshSectionWithIndices(MeshData<Vertex, Topology>& meshData)
			: m_MeshData{ meshData }
			, m_VertexStart{ meshData.Vertices.GetSize() }
			, m_VertexEnd{ Uint::MAX }
			, m_IndexStart{ meshData.Indices.GetSize() }
			, m_IndexEnd{ Uint::MAX }
		{
		}

		template<typename Vertex, ModelTopology Topology>
		inline MeshSectionWithIndices<Vertex, Topology>::MeshSectionWithIndices(MeshData<Vertex, Topology>& meshData, unsigned vertexStart, unsigned vertexEnd, unsigned indexStart, unsigned indexEnd)
			: m_MeshData{ meshData }
			, m_VertexStart{ vertexStart }, m_VertexEnd{ vertexEnd }
			, m_IndexStart{ indexStart }, m_IndexEnd{ indexEnd }
		{
		}

		template<typename Vertex, ModelTopology Topology>
		template<typename Adaptor>
		inline void MeshSectionWithIndices<Vertex, Topology>::Adapt(Adaptor adaptor)
		{
			for (unsigned i = m_VertexStart; i < m_VertexEnd; i++)
				adaptor(m_MeshData.Vertices[i]);
		}

		template<typename Vertex>
		inline MeshSection<Vertex, ModelTopology::TriangleListIdx>::MeshSection(MeshData<Vertex, ModelTopology::TriangleListIdx>& meshData)
			: MeshSectionWithIndices<Vertex, ModelTopology::TriangleListIdx>{ meshData }
		{
		}

		template<typename Vertex>
		inline MeshSection<Vertex, ModelTopology::TriangleListIdx>::MeshSection(MeshData<Vertex, ModelTopology::TriangleListIdx>& meshData, unsigned vertexStart, unsigned vertexEnd, unsigned indexStart, unsigned indexEnd)
			: MeshSectionWithIndices<Vertex, ModelTopology::TriangleListIdx>{ meshData, vertexStart, vertexEnd, indexStart, indexEnd }
		{
		}

		template<typename Vertex>
		inline MeshSection<Vertex, ModelTopology::TriangleListIdx> MeshSection<Vertex, ModelTopology::TriangleListIdx>::Copy()
		{
			using Base = MeshSectionWithIndices<Vertex, ModelTopology::TriangleListIdx>;

			//Vertices
			const unsigned nrVertices{ Base::GetNrVertices() };
			Base::m_MeshData.Vertices.EnsureIncrease(nrVertices);

			for (unsigned i = Base::m_VertexStart; i < Base::m_VertexEnd; i++)
				Base::m_MeshData.Vertices.Add(Base::m_MeshData.Vertices[i]);

			//Indices
			Base::m_MeshData.Indices.EnsureIncrease(Base::GetNrIndices());

			for (unsigned i = Base::m_IndexStart; i < Base::m_IndexEnd; i++)
				Base::m_MeshData.Indices.Add(Base::m_MeshData.Indices[i] + nrVertices);

			return MeshSection<Vertex, ModelTopology::TriangleListIdx>{ Base::m_MeshData,
				Base::m_VertexEnd, Base::m_MeshData.Vertices.GetSize(),
				Base::m_IndexEnd, Base::m_MeshData.Indices.GetSize() };
		}
	}
}