#pragma once
#include <Geometry\ModelTopology.h>
#include <DataStructures\List.h>

namespace MyEngine
{
	namespace Generation
	{
		/*
			VertexLayout: 0 ---- 1   Bottom Vertices
							| /|     are the oldest
							|/ |
						  2 ---- 3
		*/
		//note: non-indexed topologies not supported (yet).
		//Because it's annoying to write in the current form.
		//The generator should work with a fully pre-determined size of vertices (& indices), then it would be easy.

		//---| Main Class |---
		class ClosedStripGeneratorBase
		{
		public:
			struct Options {
				unsigned NrCorners{};
			};

			ClosedStripGeneratorBase(unsigned firstIndex, const Options& options);

		protected:
			const Options& m_Options;
			unsigned m_FirstVertex;
			unsigned m_CurrentVertex;
			bool m_HasStartCap{ false };
		};

		template<typename Vertex, ModelTopology Topology>
		class ClosedStripGenerator
		{ };

		//---| Specialized Classes |---
		template<typename Vertex>
		class ClosedStripGenerator<Vertex, ModelTopology::TriangleListIdx> : public ClosedStripGeneratorBase
		{
		public:
			using MeshData = MeshListData<Vertex, ModelTopology::TriangleListIdx>;

			ClosedStripGenerator(MeshData& meshData, unsigned firstIndex, const Options& options);

			void AddVertex(const Vertex& vertex);
			void SetStartCap(const Vertex& vertex);
			void SetEndCap(const Vertex& vertex);

		private:
			MeshData* m_pMesh;

			void MakeStartCap();
		};

		template<typename Vertex>
		class ClosedStripGenerator<Vertex, ModelTopology::TriangleStripIdx> : public ClosedStripGeneratorBase
		{
		public:
			using MeshData = MeshListData<Vertex, ModelTopology::TriangleStripIdx>;

			ClosedStripGenerator(MeshData& meshData, unsigned firstIndex, const Options& options);

			void AddVertex(const Vertex& vertex);
			void SetStartCap(const Vertex& vertex);
			void SetEndCap(const Vertex& vertex);

		private:
			MeshData* m_pMesh;

			void MakeStartCap();

			void MakeCapClockwise(unsigned first, unsigned center);
			void MakeCapCounterClockwise(unsigned first, unsigned center);
		};

		//---| Main Implementation |---
		inline ClosedStripGeneratorBase::ClosedStripGeneratorBase(unsigned firstVertex, const Options& options)
			: m_Options{ options }
			, m_FirstVertex{ firstVertex }
			, m_CurrentVertex{ firstVertex }
		{
		}

		//---| Specialized Implemenations |---

		//---| TRIANGLE LIST IDX |---
		template<typename Vertex>
		inline ClosedStripGenerator<Vertex, ModelTopology::TriangleListIdx>::ClosedStripGenerator(MeshData& meshData, unsigned firstIndex, const Options& options)
			: ClosedStripGeneratorBase{ firstIndex, options }
			, m_pMesh{ &meshData }
		{
		}

		template<typename Vertex>
		inline void ClosedStripGenerator<Vertex, ModelTopology::TriangleListIdx>::SetStartCap(const Vertex& vertex)
		{
			m_pMesh->Vertices.Add(vertex);
			m_HasStartCap = true;
			m_FirstVertex++;
			m_CurrentVertex++;
		}

		template<typename Vertex>
		inline void ClosedStripGenerator<Vertex, ModelTopology::TriangleListIdx>::SetEndCap(const Vertex& vertex)
		{
			m_pMesh->Vertices.Add(vertex);

			const unsigned center{ m_CurrentVertex };
			const unsigned beforeLast{ m_CurrentVertex - 1 };
			const unsigned first{ m_CurrentVertex - m_Options.NrCorners };

			m_pMesh->Indices.Add(center, first, beforeLast);

			for (unsigned iCorner = beforeLast; iCorner > first; iCorner--)
				m_pMesh->Indices.Add(center, iCorner, iCorner - 1);
		}

		template<typename Vertex>
		inline void ClosedStripGenerator<Vertex, ModelTopology::TriangleListIdx>::AddVertex(const Vertex& vertex)
		{
			m_pMesh->Vertices.Add(vertex);

			const unsigned iCorner{ (m_CurrentVertex - m_FirstVertex) % m_Options.NrCorners };
			m_CurrentVertex++;

			if (iCorner + 1 != m_Options.NrCorners)
				return;
			//last vertex of row, thus assemble vertices

			if (m_CurrentVertex - m_FirstVertex == m_Options.NrCorners)
			{
				//end of first row
				if (m_HasStartCap)
					MakeStartCap();
				return;
			}

			const unsigned firstTop{ m_CurrentVertex - m_Options.NrCorners };
			const unsigned firstBot{ firstTop - m_Options.NrCorners };

			for (unsigned iBot = firstBot; iBot + 1 < firstTop; iBot++)
			{
				const unsigned botLeft{ iBot };
				const unsigned botRight{ botLeft + 1 };
				const unsigned topLeft{ botLeft + m_Options.NrCorners };
				const unsigned topRight{ topLeft + 1 };

				m_pMesh->Indices.Add(botLeft, topLeft, botRight);
				m_pMesh->Indices.Add(botRight, topLeft, topRight);
			}

			//last
			const unsigned botRight{ firstBot };
			const unsigned botLeft{ botRight + m_Options.NrCorners - 1 };
			const unsigned topRight{ firstTop };
			const unsigned topLeft{ topRight + m_Options.NrCorners - 1 };

			m_pMesh->Indices.Add(botLeft, topLeft, botRight);
			m_pMesh->Indices.Add(botRight, topLeft, topRight);
		}

		template<typename Vertex>
		inline void ClosedStripGenerator<Vertex, ModelTopology::TriangleListIdx>::MakeStartCap()
		{
			const unsigned center{ m_FirstVertex - 1 };
			const unsigned last{ m_CurrentVertex - 1 };

			for (unsigned iCorner = m_FirstVertex; iCorner < last; iCorner++)
				m_pMesh->Indices.Add(center, iCorner, iCorner + 1);

			m_pMesh->Indices.Add(center, last, m_FirstVertex);
		}

		//---| TRIANGLE STRIP IDX |---
		template<typename Vertex>
		inline ClosedStripGenerator<Vertex, ModelTopology::TriangleStripIdx>::ClosedStripGenerator(MeshData& meshData, unsigned firstIndex, const Options& options)
			: ClosedStripGeneratorBase{ firstIndex, options }
			, m_pMesh{ &meshData }
		{
		}

		template<typename Vertex>
		inline void ClosedStripGenerator<Vertex, ModelTopology::TriangleStripIdx>::SetStartCap(const Vertex& vertex)
		{
			m_pMesh->Vertices.Add(vertex);
			m_HasStartCap = true;
			m_FirstVertex++;
			m_CurrentVertex++;
		}

		template<typename Vertex>
		inline void ClosedStripGenerator<Vertex, ModelTopology::TriangleStripIdx>::SetEndCap(const Vertex& vertex)
		{
			m_pMesh->Vertices.Add(vertex);

			const unsigned center{ m_CurrentVertex };
			const unsigned first{ center - m_Options.NrCorners };

			if (m_pMesh->Indices.GetSize() % 2 == 0)
				MakeCapClockwise(first, center);
			else
				MakeCapCounterClockwise(first, center);
		}

		template<typename Vertex>
		inline void ClosedStripGenerator<Vertex, ModelTopology::TriangleStripIdx>::AddVertex(const Vertex& vertex)
		{
			m_pMesh->Vertices.Add(vertex);

			const unsigned iCorner{ (m_CurrentVertex - m_FirstVertex) % m_Options.NrCorners };
			m_CurrentVertex++;

			if (iCorner + 1 != m_Options.NrCorners)
				return;
			//last vertex of row, thus assemble vertices

			if (m_CurrentVertex - m_FirstVertex == m_Options.NrCorners)
			{
				//end of first row
				if (m_HasStartCap)
					MakeStartCap();
				return;
			}

			const unsigned firstTop{ m_CurrentVertex - m_Options.NrCorners };
			const unsigned firstBot{ firstTop - m_Options.NrCorners };

			if (m_pMesh->Indices.GetSize() % 2 == 0)
			{
				for (unsigned iBot = firstBot; iBot < firstTop; iBot++)
				{
					m_pMesh->Indices.Add(iBot);
					m_pMesh->Indices.Add(iBot + m_Options.NrCorners);
				}
				m_pMesh->Indices.Add(firstBot);
				m_pMesh->Indices.Add(firstTop);
			}
			else
			{
				m_pMesh->Indices.Add(firstBot);
				m_pMesh->Indices.Add(firstTop);
				for (unsigned iBot = firstTop - 1; iBot >= firstBot; iBot--)
				{
					m_pMesh->Indices.Add(iBot);
					m_pMesh->Indices.Add(iBot + m_Options.NrCorners);
				}
			}
		}

		template<typename Vertex>
		inline void ClosedStripGenerator<Vertex, ModelTopology::TriangleStripIdx>::MakeStartCap()
		{
			const unsigned center{ m_FirstVertex - 1 };
			const unsigned first{ center + 1 };
			unsigned iCorner{ first };

			if (m_pMesh->Indices.GetSize() % 2 == 0)
				MakeCapCounterClockwise(first, center);
			else
				MakeCapClockwise(first, center);
		}
		template<typename Vertex>
		inline void ClosedStripGenerator<Vertex, ModelTopology::TriangleStripIdx>::MakeCapClockwise(unsigned first, unsigned center)
		{
			const unsigned beforeLast{ first + m_Options.NrCorners - 1 };
			unsigned iCorner{ first };

			//first
			m_pMesh->Indices.Add(iCorner, beforeLast, center);
			iCorner = beforeLast;

			for (; iCorner > first + 2; iCorner -= 2)
			{
				m_pMesh->Indices.Add(iCorner - 1, iCorner - 2);
				m_pMesh->Indices.Add(iCorner - 2, center); //'fix'
			}

			if (m_Options.NrCorners % 2 == 1)
				m_pMesh->Indices.Add(iCorner - 1);

			m_pMesh->Indices.Add(first);
		}
		template<typename Vertex>
		inline void ClosedStripGenerator<Vertex, ModelTopology::TriangleStripIdx>::MakeCapCounterClockwise(unsigned first, unsigned center)
		{
			const unsigned beforeLast{ first + m_Options.NrCorners - 1 };
			unsigned iCorner{ first };

			//first
			m_pMesh->Indices.Add(iCorner, iCorner + 1, center);
			iCorner++;

			for (; iCorner + 1 < beforeLast; iCorner += 2)
			{
				m_pMesh->Indices.Add(iCorner + 1, iCorner + 2);
				m_pMesh->Indices.Add(iCorner + 2, center); //'fix'
			}

			if (m_Options.NrCorners % 2 == 1)
				m_pMesh->Indices.Add(iCorner + 1);

			m_pMesh->Indices.Add(first);
		}
	}
}