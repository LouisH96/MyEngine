#pragma once

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
}