#pragma once
#include <Geometry\ModelTopology.h>

namespace MyEngine
{
namespace MeshMaker
{
class TopologyInfo
{
public:
	enum class BaseType
	{
		LineList, LineStrip,
		TriangleList, TriangleStrip
	};

	static constexpr BaseType GetBaseType(ModelTopology topology)
	{
		switch (topology)
		{
		case ModelTopology::LineList:
		case ModelTopology::LineListIdx:
			return BaseType::LineList;

		case ModelTopology::LineStrip:
		case ModelTopology::LineStripIdx:
			return BaseType::LineStrip;

		case ModelTopology::TriangleList:
		case ModelTopology::TriangleListIdx:
			return BaseType::TriangleList;

		case ModelTopology::TriangleStrip:
		case ModelTopology::TriangleStripIdx:
			return BaseType::TriangleStrip;
		}
	}

	static constexpr bool IsLineType(ModelTopology topology)
	{
		switch (GetBaseType(topology))
		{
		case BaseType::LineList:
		case BaseType::LineStrip:
			return true;
		default:
			return false;
		}
	}

	static constexpr bool HasIndices(ModelTopology topology)
	{
		switch (topology)
		{
		case ModelTopology::LineListIdx:
		case ModelTopology::LineStripIdx:
		case ModelTopology::TriangleListIdx:
		case ModelTopology::TriangleStripIdx:
			return true;
		default: 
			return false;
		}
	}
};
}
}