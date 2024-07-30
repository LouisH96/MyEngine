#pragma once

#include <Rendering\Mesh\MeshData.h>
#include <Applied\MeshMaker\MakerResult.h>

namespace MyEngine
{
namespace MeshMaker
{
template<typename TResult>
class MakerAdder
	: public TResult
{
public:
	MakerAdder() = default;
};
}
}