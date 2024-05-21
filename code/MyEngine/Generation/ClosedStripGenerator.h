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
		class ClosedStripGenerator<Vertex, ModelTopology::TriangleList> : public ClosedStripGeneratorBase
		{
		public:
			ClosedStripGenerator(List<Vertex>& vertices, const Options& options);

			void AddVertex(const Vertex& vertex);
			void SetStartCap(const Vertex& vertex);
			void SetEndCap(const Vertex& vertex);

		private:
			List<Vertex>* m_pVertices;

			void MakeStartCap();
		};

		template<typename Vertex>
		class ClosedStripGenerator<Vertex, ModelTopology::TriangleListIdx> : public ClosedStripGeneratorBase
		{
		public:
			ClosedStripGenerator(List<Vertex>& vertices, List<int>& indices, unsigned firstIndex, const Options& options);

			void AddVertex(const Vertex& vertex);
			void SetStartCap(const Vertex& vertex);
			void SetEndCap(const Vertex& vertex);

		private:
			List<Vertex>* m_pVertices;
			List<int>* m_pIndices;

			void MakeStartCap();
		};

		template<typename Vertex>
		class ClosedStripGenerator<Vertex, ModelTopology::TriangleStripIdx> : public ClosedStripGeneratorBase
		{
		public:
			ClosedStripGenerator(List<Vertex>& vertices, List<int>& indices, unsigned firstIndex, const Options& options);

			void AddVertex(const Vertex& vertex);
			void SetStartCap(const Vertex& vertex);
			void SetEndCap(const Vertex& vertex);

		private:
			List<Vertex>* m_pVertices;
			List<int>* m_pIndices;

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

		//---| TRIANGLE LIST |---
		template<typename Vertex>
		inline ClosedStripGenerator<Vertex, ModelTopology::TriangleList>::ClosedStripGenerator(List<Vertex>& vertices, const Options& options)
			: ClosedStripGeneratorBase{ firstIndex, options }
			, m_pVertices{ &vertexAdder }
		{
		}

		template<typename Vertex>
		inline void ClosedStripGenerator<Vertex, ModelTopology::TriangleListIdx>::AddVertex(const Vertex& vertex)
		{
			m_Vertices.Add(vertex);

			if (m_HasStartCap)



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

				m_Indices.Add(botLeft, topLeft, botRight);
				m_Indices.Add(botRight, topLeft, topRight);
			}

			//last
			const unsigned botRight{ firstBot };
			const unsigned botLeft{ botRight + m_Options.NrCorners - 1 };
			const unsigned topRight{ firstTop };
			const unsigned topLeft{ topRight + m_Options.NrCorners - 1 };

			m_Indices.Add(botLeft, topLeft, botRight);
			m_Indices.Add(botRight, topLeft, topRight);
		}

		template<typename Vertex>
		inline void ClosedStripGenerator<Vertex, ModelTopology::TriangleList>::SetStartCap(const Vertex& vertex)
		{
			//m_Vertices.

			m_Vertices.Add(vertex);
			m_HasStartCap = true;
			m_FirstVertex++;
			m_CurrentVertex++;
		}

		//---| TRIANGLE LIST IDX |---
		template<typename Vertex>
		inline ClosedStripGenerator<Vertex, ModelTopology::TriangleListIdx>::ClosedStripGenerator(List<Vertex>& vertices, List<int>& indices, unsigned firstIndex, const Options& options)
			: ClosedStripGeneratorBase{ firstIndex, options }
			, m_pVertices{ &vertices }
			, m_pIndices{ &indices }
		{
		}

		template<typename Vertex>
		inline void ClosedStripGenerator<Vertex, ModelTopology::TriangleListIdx>::SetStartCap(const Vertex& vertex)
		{
			m_Vertices.Add(vertex);
			m_HasStartCap = true;
			m_FirstVertex++;
			m_CurrentVertex++;
		}

		template<typename Vertex>
		inline void ClosedStripGenerator<Vertex, ModelTopology::TriangleListIdx>::SetEndCap(const Vertex& vertex)
		{
			m_Vertices.Add(vertex);

			const unsigned center{ m_CurrentVertex };
			const unsigned beforeLast{ m_CurrentVertex - 1 };
			const unsigned first{ m_CurrentVertex - m_Options.NrCorners };

			m_Indices.Add(center, first, beforeLast);

			for (unsigned iCorner = beforeLast; iCorner > first; iCorner--)
				m_Indices.Add(center, iCorner, iCorner - 1);
		}

		template<typename VertexAdder, typename IndexContainer>
		inline void ClosedStripGenerator<VertexAdder, IndexContainer, ModelTopology::TriangleListIdx>::AddVertex(const Vertex& vertex)
		{
			m_Vertices.Add(vertex);

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

				m_Indices.Add(botLeft, topLeft, botRight);
				m_Indices.Add(botRight, topLeft, topRight);
			}

			//last
			const unsigned botRight{ firstBot };
			const unsigned botLeft{ botRight + m_Options.NrCorners - 1 };
			const unsigned topRight{ firstTop };
			const unsigned topLeft{ topRight + m_Options.NrCorners - 1 };

			m_Indices.Add(botLeft, topLeft, botRight);
			m_Indices.Add(botRight, topLeft, topRight);
		}

		template<typename VertexAdder, typename IndexContainer>
		inline void ClosedStripGenerator<VertexAdder, IndexContainer, ModelTopology::TriangleListIdx>::MakeStartCap()
		{
			const unsigned center{ m_FirstVertex - 1 };
			const unsigned last{ m_CurrentVertex - 1 };

			for (unsigned iCorner = m_FirstVertex; iCorner < last; iCorner++)
				m_Indices.Add(center, iCorner, iCorner + 1);

			m_Indices.Add(center, last, m_FirstVertex);
		}

		//---| TRIANGLE STRIP IDX |---
		template<typename VertexAdder, typename IndexContainer>
		inline ClosedStripGenerator<VertexAdder, IndexContainer, ModelTopology::TriangleStripIdx>::ClosedStripGenerator(VertexAdder&& vertexAdder, IndexContainer&& indexContainer, unsigned firstIndex, const Options& options)
			: ClosedStripGeneratorBase{ firstIndex, options }
			, m_Vertices{ std::move(vertexAdder) }
			, m_Indices{ std::move(indexContainer) }
		{
		}

		template<typename VertexAdder, typename IndexContainer>
		inline void ClosedStripGenerator<VertexAdder, IndexContainer, ModelTopology::TriangleStripIdx>::SetStartCap(const Vertex& vertex)
		{
			m_Vertices.Add(vertex);
			m_HasStartCap = true;
			m_FirstVertex++;
			m_CurrentVertex++;
		}

		template<typename VertexAdder, typename IndexContainer>
		inline void ClosedStripGenerator<VertexAdder, IndexContainer, ModelTopology::TriangleStripIdx>::SetEndCap(const Vertex& vertex)
		{
			m_Vertices.Add(vertex);

			const unsigned center{ m_CurrentVertex };
			const unsigned first{ center - m_Options.NrCorners };

			if (m_Indices.GetSize() % 2 == 0)
				MakeCapClockwise(first, center);
			else
				MakeCapCounterClockwise(first, center);
		}

		template<typename VertexAdder, typename IndexContainer>
		inline void ClosedStripGenerator<VertexAdder, IndexContainer, ModelTopology::TriangleStripIdx>::AddVertex(const Vertex& vertex)
		{
			m_Vertices.Add(vertex);

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

			if (m_Indices.GetSize() % 2 == 0)
			{
				for (unsigned iBot = firstBot; iBot < firstTop; iBot++)
				{
					m_Indices.Add(iBot);
					m_Indices.Add(iBot + m_Options.NrCorners);
				}
				m_Indices.Add(firstBot);
				m_Indices.Add(firstTop);
			}
			else
			{
				m_Indices.Add(firstBot);
				m_Indices.Add(firstTop);
				for (unsigned iBot = firstTop - 1; iBot >= firstBot; iBot--)
				{
					m_Indices.Add(iBot);
					m_Indices.Add(iBot + m_Options.NrCorners);
				}
			}
		}

		template<typename VertexAdder, typename IndexContainer>
		inline void ClosedStripGenerator<VertexAdder, IndexContainer, ModelTopology::TriangleStripIdx>::MakeStartCap()
		{
			const unsigned center{ m_FirstVertex - 1 };
			const unsigned first{ center + 1 };
			unsigned iCorner{ first };

			if (m_Indices.GetSize() % 2 == 0)
				MakeCapCounterClockwise(first, center);
			else
				MakeCapClockwise(first, center);
		}
		template<typename VertexAdder, typename IndexContainer>
		inline void ClosedStripGenerator<VertexAdder, IndexContainer, ModelTopology::TriangleStripIdx>::MakeCapClockwise(unsigned first, unsigned center)
		{
			const unsigned beforeLast{ first + m_Options.NrCorners - 1 };
			unsigned iCorner{ first };

			//first
			m_Indices.Add(iCorner, beforeLast, center);
			iCorner = beforeLast;

			for (; iCorner > first + 2; iCorner -= 2)
			{
				m_Indices.Add(iCorner - 1, iCorner - 2);
				m_Indices.Add(iCorner - 2, center); //'fix'
			}

			if (m_Options.NrCorners % 2 == 1)
				m_Indices.Add(iCorner - 1);

			m_Indices.Add(first);
		}
		template<typename VertexAdder, typename IndexContainer>
		inline void ClosedStripGenerator<VertexAdder, IndexContainer, ModelTopology::TriangleStripIdx>::MakeCapCounterClockwise(unsigned first, unsigned center)
		{
			const unsigned beforeLast{ first + m_Options.NrCorners - 1 };
			unsigned iCorner{ first };

			//first
			m_Indices.Add(iCorner, iCorner + 1, center);
			iCorner++;

			for (; iCorner + 1 < beforeLast; iCorner += 2)
			{
				m_Indices.Add(iCorner + 1, iCorner + 2);
				m_Indices.Add(iCorner + 2, center); //'fix'
			}

			if (m_Options.NrCorners % 2 == 1)
				m_Indices.Add(iCorner + 1);

			m_Indices.Add(first);
		}
	}
}