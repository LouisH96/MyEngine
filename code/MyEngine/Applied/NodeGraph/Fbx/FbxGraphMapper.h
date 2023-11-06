#pragma once

namespace MyEngine
{
	namespace Io
	{
		namespace Fbx
		{
			namespace Wrapping
			{
				class FbxData;
			}
		}
	}
}

namespace MyEngine
{
	namespace Applied
	{
		class NodeGraph;

		class FbxGraphMapper
		{
		public:
			FbxGraphMapper(const Io::Fbx::Wrapping::FbxData& fbx, NodeGraph& graph);

		private:
			const Io::Fbx::Wrapping::FbxData& m_FbxData;
			NodeGraph& m_Graph;

			void AddModels() const;
		};
	}
}
