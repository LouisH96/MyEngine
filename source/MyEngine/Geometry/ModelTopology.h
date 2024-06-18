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
}