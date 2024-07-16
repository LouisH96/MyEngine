#pragma once
#include <Math\Vectors.h>

namespace MyEngine
{
namespace MeshMaker
{
enum class PointType
{
	Point, Vertex, Index
};

template<typename Vertex>
class Point;

template<>
class Point<unsigned>
{
public:
	unsigned Index;

	static constexpr PointType PointType{ PointType::Index };
};

template<>
class Point<Float3>
{
public:
	Float3 Pos;

	static constexpr PointType PointType{ PointType::Point };
};

template<typename Vertex>
class Point
{
public:
	Vertex Vertex;

	static constexpr PointType PointType{ PointType::Vertex };
};

}
}