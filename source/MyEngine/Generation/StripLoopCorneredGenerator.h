#pragma once
#include <Geometry\ModelTopology.h>
#include <Rendering\Mesh\MeshData.h>

namespace MyEngine
{
	namespace Generation
	{
		//---| Main Class |---
		class StripLoopCorneredGeneratorBase
		{
		public:
			struct Options {
				unsigned NrCorners{};
			};

			StripLoopCorneredGeneratorBase(unsigned firstVertex, const Options& options);

		protected:
			const Options m_Options;
			unsigned m_FirstVertex;
			bool m_HasStartCap{ false };

			unsigned m_CurrentLayer{ 0 };
			unsigned m_CurrentCorner{ 0 };
			bool m_FirstOfCorner{ true };
		};

		//---| Main Implemenations |---
		inline StripLoopCorneredGeneratorBase::StripLoopCorneredGeneratorBase(unsigned firstVertex, const Options& options)
			: m_Options{ options }
			, m_FirstVertex{ firstVertex }
		{
		}

		template<typename Vertex, ModelTopology Topology>
		class StripLoopCorneredGenerator
		{};

		//---| Specialized Classes |---
		template<typename Vertex>
		class StripLoopCorneredGenerator<Vertex, ModelTopology::TriangleStripIdx>
			: public StripLoopCorneredGeneratorBase
		{
		public:
			using MeshData = MeshData<Vertex, ModelTopology::TriangleStripIdx>;

			StripLoopCorneredGenerator(MeshData& meshData, const Options& options);

			void AddVertex(const Vertex& vertex);

			void SetFlatStartCap(const Vertex& baseVertex);
			void SetFlatEndCap(Vertex baseVertex);

		private:
			MeshData* m_pMesh;
		};

		//---| Specialized Implementations |---
		template<typename Vertex>
		inline StripLoopCorneredGenerator<Vertex, ModelTopology::TriangleStripIdx>::StripLoopCorneredGenerator(MeshData& meshData, const Options& options)
			: StripLoopCorneredGeneratorBase{ meshData.Vertices.GetSize(), options }
			, m_pMesh{ &meshData }
		{
		}

		template<typename Vertex>
		inline void StripLoopCorneredGenerator<Vertex, ModelTopology::TriangleStripIdx>::AddVertex(const Vertex& vertex)
		{
			//Increase buffer sizes
			if (m_CurrentCorner == 0 && m_FirstOfCorner)
			{
				unsigned vertexIncrease{ m_Options.NrCorners * 2 };
				unsigned indexIncrease{ m_Options.NrCorners * 4 };

				//start cap?
				if (m_HasStartCap && m_CurrentLayer == 0)
				{
					vertexIncrease += m_Options.NrCorners;
					indexIncrease += m_Options.NrCorners;
				}
				else
				{
					//new layer
					m_pMesh->Vertices.EnsureIncrease(vertexIncrease);
					m_pMesh->Indices.EnsureIncrease(indexIncrease);
				}
			}

			//Add vertex to Vertices
			m_pMesh->Vertices.Add(vertex);

			//Update CurrentCorner
			if (!m_FirstOfCorner)
				m_CurrentCorner++;
			m_FirstOfCorner = !m_FirstOfCorner;

			//Assmeble walls if layer is finished
			if (m_CurrentCorner == m_Options.NrCorners)
			{
				m_CurrentCorner = 0;
				m_CurrentLayer++;

				if (m_CurrentLayer == 1)
				{
					if (m_HasStartCap)
					{
						//Make Start cap
						unsigned wallIdx{ m_pMesh->Vertices.GetSize() - m_Options.NrCorners * 2 };
						unsigned capIdx{ wallIdx - m_Options.NrCorners };

						for (unsigned iCorner = 0; iCorner < m_Options.NrCorners; iCorner++)
						{
							m_pMesh->Vertices[capIdx].Pos = m_pMesh->Vertices[wallIdx].Pos;
							capIdx++;
							wallIdx += 2;
						}
					}
				}
				else
				{
					//Assemble walls
					if (m_pMesh->Indices.GetSize() % 2 == 0)
					{
						unsigned top{ m_pMesh->Vertices.GetSize() - m_Options.NrCorners * 2 };
						unsigned bot{ top - m_Options.NrCorners * 2 };
						for (unsigned iCorner = 0; iCorner < m_Options.NrCorners * 2; iCorner++)
						{
							m_pMesh->Indices.Add(bot++);
							m_pMesh->Indices.Add(top++);
						}
					}
					else
					{
						unsigned top{ m_pMesh->Vertices.GetSize() - 1 };
						unsigned bot{ top - m_Options.NrCorners * 2 };
						for (unsigned iCorner = 0; iCorner < m_Options.NrCorners * 2; iCorner++)
						{
							m_pMesh->Indices.Add(bot--);
							m_pMesh->Indices.Add(top--);
						}
					}
				}
			}
		}
		template<typename Vertex>
		inline void StripLoopCorneredGenerator<Vertex, ModelTopology::TriangleStripIdx>::SetFlatStartCap(const Vertex& baseVertex)
		{
			m_HasStartCap = true;

			m_pMesh->Vertices.EnsureIncrease(m_Options.NrCorners);
			m_pMesh->Indices.EnsureIncrease(m_Options.NrCorners);

			//Add Indices
			unsigned idx{ m_pMesh->Vertices.GetSize() };
			m_pMesh->Indices.Add(idx);
			int step{ static_cast<int>(m_Options.NrCorners) - 1 };
			int sign = 1;

			if (m_pMesh->Indices.GetSize() % 2 != 0)
			{
				idx++;
				step--;
				m_pMesh->Indices.Add(idx);
			}

			while (step != 0)
			{
				idx += step * sign;
				step--;
				sign *= -1;
				m_pMesh->Indices.Add(idx);
			}

			//Add Vertices
			for (unsigned i = 0; i < m_Options.NrCorners; i++)
				m_pMesh->Vertices.Add(baseVertex);
		}

		template<typename Vertex>
		inline void StripLoopCorneredGenerator<Vertex, ModelTopology::TriangleStripIdx>::SetFlatEndCap(Vertex baseVertex)
		{
			m_pMesh->Vertices.EnsureIncrease(m_Options.NrCorners);
			m_pMesh->Indices.EnsureIncrease(m_Options.NrCorners);

			//create vertices
			unsigned idx{ m_pMesh->Vertices.GetSize() - m_Options.NrCorners * 2 };
			for (unsigned iCorner = 0; iCorner < m_Options.NrCorners; iCorner++, idx += 2)
			{
				baseVertex.Pos = m_pMesh->Vertices[idx].Pos;
				m_pMesh->Vertices.Add(baseVertex);
			}

			//create indices
			m_pMesh->Indices.Add(idx);
			int step{ static_cast<int>(m_Options.NrCorners) - 1 };
			int sign = 1;

			if (m_pMesh->Indices.GetSize() % 2 == 0)
			{
				idx++;
				step--;
				m_pMesh->Indices.Add(idx);
			}

			while (step != 0)
			{
				idx += step * sign;
				step--;
				sign *= -1;
				m_pMesh->Indices.Add(idx);
			}
		}
	}
}