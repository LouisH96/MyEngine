#pragma once
#include <string>

namespace MyEngine
{
	enum class ModelTopology
	{
		LineList, LineListIdx,
		LineStrip, LineStripIdx,
		TriangleList, TriangleListIdx,
		TriangleStrip, TriangleStripIdx,

		Unknown
	};

	class ModelTopologyToString
	{
	public:
		template<ModelTopology Topology>
		static std::string ToString();
	};

	template<>
	inline std::string ModelTopologyToString::ToString<ModelTopology::LineList>()
	{
		return "LineList";
	}
	template<>
	inline std::string ModelTopologyToString::ToString<ModelTopology::LineListIdx>()
	{
		return "LineListIdx";
	}
	template<>
	inline std::string ModelTopologyToString::ToString<ModelTopology::LineStrip>()
	{
		return "LineStrip";
	}
	template<>
	inline std::string ModelTopologyToString::ToString<ModelTopology::LineStripIdx>()
	{
		return "LineStripIdx";
	}
	template<>
	inline std::string ModelTopologyToString::ToString<ModelTopology::TriangleList>()
	{
		return "TriangleList";
	}
	template<>
	inline std::string ModelTopologyToString::ToString<ModelTopology::TriangleListIdx>()
	{
		return "TriangleListIdx";
	}
	template<>
	inline std::string ModelTopologyToString::ToString<ModelTopology::TriangleStrip>()
	{
		return "TriangleStrip";
	}
	template<>
	inline std::string ModelTopologyToString::ToString<ModelTopology::TriangleStripIdx>()
	{
		return "TriangleStripIdx";
	}

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