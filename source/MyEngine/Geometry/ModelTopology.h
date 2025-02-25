#pragma once
#include <string>
#include <Logger\Logger.h>

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
	enum class DrawType
	{
		Line, Triangle
	};
	enum class FormatType
	{
		List, Strip
	};
	enum class UnitType
	{
		Vertex, Index
	};
	enum class BaseType
	{
		LineList, LineStrip,
		TriangleList, TriangleStrip
	};

	static constexpr DrawType GetDrawType(ModelTopology topology)
	{
		switch (topology)
		{
		case ModelTopology::LineList:
		case ModelTopology::LineListIdx:
		case ModelTopology::LineStrip:
		case ModelTopology::LineStripIdx:
			return DrawType::Line;
		case ModelTopology::TriangleList:
		case ModelTopology::TriangleListIdx:
		case ModelTopology::TriangleStrip:
		case ModelTopology::TriangleStripIdx:
			return DrawType::Triangle;
		default:
			Logger::Error("[TopologyInfo::GetUnityType] unknown type");
			return {};
		}
	}

	static constexpr FormatType GetFormatType(ModelTopology topology)
	{
		switch (topology)
		{
		case ModelTopology::LineList:
		case ModelTopology::LineListIdx:
		case ModelTopology::TriangleList:
		case ModelTopology::TriangleListIdx:
			return FormatType::List;
		case ModelTopology::LineStrip:
		case ModelTopology::LineStripIdx:
		case ModelTopology::TriangleStrip:
		case ModelTopology::TriangleStripIdx:
			return FormatType::Strip;
		default:
			Logger::Error("[TopologyInfo::GetFormatType] unknown type");
			return {};
		}
	}

	static constexpr UnitType GetUnitType(ModelTopology topology)
	{
		switch (topology)
		{
		case ModelTopology::LineList:
		case ModelTopology::LineStrip:
		case ModelTopology::TriangleList:
		case ModelTopology::TriangleStrip:
			return UnitType::Vertex;
		case ModelTopology::LineListIdx:
		case ModelTopology::LineStripIdx:
		case ModelTopology::TriangleListIdx:
		case ModelTopology::TriangleStripIdx:
			return UnitType::Index;
		default:
			Logger::Error("[TopologyInfo::GetUnityType] unknown type");
			return {};
		}
	}

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

		default:
			Logger::Error("[ModelTopology::GetBaseType] unknown type");
			return {};
		}
	}

	static constexpr bool IsLineType(ModelTopology topology)
	{
		return GetDrawType(topology) == DrawType::Line;
	}

	static constexpr bool IsTriangleType(ModelTopology topology)
	{
		return GetDrawType(topology) == DrawType::Triangle;
	}

	static constexpr bool HasIndices(ModelTopology topology)
	{
		return GetUnitType(topology) == UnitType::Index;
	}

	static constexpr bool IsListFormat(ModelTopology topology)
	{
		return GetFormatType(topology) == FormatType::List;
	}

	static constexpr bool IsStripFormat(ModelTopology topology)
	{
		return GetFormatType(topology) == FormatType::Strip;
	}
};

class TopologyBaseType
{
public:
	class LineList {};
	class LineStrip {};
	class TriangleList {};
	class TriangleStrip {};

	template<TopologyInfo::BaseType TBaseType>
	struct ToBaseType;

	template<ModelTopology TTopology>
	using FromModelTopology = typename ToBaseType<TopologyInfo::GetBaseType(TTopology)>::Type;

	template<>
	struct ToBaseType<TopologyInfo::BaseType::LineList>
	{
		using Type = LineList;
	};
	template<>
	struct ToBaseType<TopologyInfo::BaseType::LineStrip>
	{
		using Type = LineStrip;
	};
	template<>
	struct ToBaseType<TopologyInfo::BaseType::TriangleList>
	{
		using Type = TriangleList;
	};
	template<>
	struct ToBaseType<TopologyInfo::BaseType::TriangleStrip>
	{
		using Type = TriangleStrip;
	};
};
}