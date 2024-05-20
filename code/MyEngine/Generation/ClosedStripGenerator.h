#pragma once
#include <Geometry\ModelTopology.h>

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

		template<typename VertexAdder, typename IndexAdder, ModelTopology Topology>
		class ClosedStripGenerator
		{ };

		//---| Specialized Classes |---
		template<typename VertexAdder, typename IndexAdder>
		class ClosedStripGenerator<VertexAdder, IndexAdder, ModelTopology::TriangleListIdx> : public ClosedStripGeneratorBase
		{
		public:
			using Vertex = typename VertexAdder::DataType;

			ClosedStripGenerator(VertexAdder&& vertexAdder, IndexAdder&& indexAdder, unsigned firstIndex, const Options& options);

			void AddVertex(const Vertex& vertex);
			void SetStartCap(const Vertex& vertex);
			void SetEndCap(const Vertex& vertex);

		private:
			VertexAdder m_Vertices;
			IndexAdder m_Indices;

			void MakeStartCap();
		};

		//---| Main Implementation |---
		inline ClosedStripGeneratorBase::ClosedStripGeneratorBase(unsigned firstVertex, const Options& options)
			: m_Options{ options }
			, m_FirstVertex{ firstVertex }
			, m_CurrentVertex{ firstVertex }
		{
		}

		//---| Specialized Implemenations |---
		template<typename VertexAdder, typename IndexAdder>
		inline ClosedStripGenerator<VertexAdder, IndexAdder, ModelTopology::TriangleListIdx>::ClosedStripGenerator(VertexAdder&& vertexAdder, IndexAdder&& indexAdder, unsigned firstIndex, const Options& options)
			: ClosedStripGeneratorBase{ firstIndex, options }
			, m_Vertices{ std::move(vertexAdder) }
			, m_Indices{ std::move(indexAdder) }
		{
		}

		template<typename VertexAdder, typename IndexAdder>
		inline void ClosedStripGenerator<VertexAdder, IndexAdder, ModelTopology::TriangleListIdx>::SetStartCap(const Vertex& vertex)
		{
			m_Vertices.Add(vertex);
			m_HasStartCap = true;
			m_FirstVertex++;
			m_CurrentVertex++;
		}

		template<typename VertexAdder, typename IndexAdder>
		inline void ClosedStripGenerator<VertexAdder, IndexAdder, ModelTopology::TriangleListIdx>::SetEndCap(const Vertex& vertex)
		{
			m_Vertices.Add(vertex);

			const unsigned center{ m_CurrentVertex };
			const unsigned beforeLast{ m_CurrentVertex - 1 };
			const unsigned first{ m_CurrentVertex - m_Options.NrCorners };

			m_Indices.Add(center, first, beforeLast);

			for (unsigned iCorner = beforeLast; iCorner > first; iCorner--)
				m_Indices.Add(center, iCorner, iCorner - 1);
		}

		template<typename VertexAdder, typename IndexAdder>
		inline void ClosedStripGenerator<VertexAdder, IndexAdder, ModelTopology::TriangleListIdx>::AddVertex(const Vertex& vertex)
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

		template<typename VertexAdder, typename IndexAdder>
		inline void ClosedStripGenerator<VertexAdder, IndexAdder, ModelTopology::TriangleListIdx>::MakeStartCap()
		{
			const unsigned center{ m_FirstVertex - 1 };
			const unsigned last{ m_CurrentVertex - 1 };

			for (unsigned iCorner = m_FirstVertex; iCorner < last; iCorner++)
				m_Indices.Add(center, iCorner, iCorner + 1);

			m_Indices.Add(center, last, m_FirstVertex);
		}
	}
}