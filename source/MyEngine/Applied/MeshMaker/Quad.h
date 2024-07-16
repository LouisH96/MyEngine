#pragma once

#include <Rendering\Mesh\MeshData.h>

#include "Point.h"
#include "TopologyInfo.h"

namespace MyEngine
{
namespace MeshMaker
{

template<typename T0, typename T1, typename T2, typename T3>
class Quad
{
public:
	Quad(T0 p0, T1 p1, T2 p2, T3 p3);

	Point<T0> P0;
	Point<T1> P1;
	Point<T2> P2;
	Point<T3> P3;
};

template<typename T0, typename T1, typename T2, typename T3>
inline Quad<T0, T1, T2, T3>::Quad(T0 p0, T1 p1, T2 p2, T3 p3)
	: P0{ p0 }, P1{ p1 }, P2{ p2 }, P3{ p3 }
{


}
}
}