#include "pch.h"
#include "PrimitiveTopology.h"

#include "Rendering/Gpu.h"

D3D11_PRIMITIVE_TOPOLOGY Rendering::PrimitiveTopologyUtils::ToDx(PrimitiveTopology topology)
{
	switch (topology)
	{
	case PrimitiveTopology::TriangleList: return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	case PrimitiveTopology::TriangleStrip: return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
	case PrimitiveTopology::LineStrip: return D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
	case PrimitiveTopology::LineList: return D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
	default:
	case PrimitiveTopology::Unknown: return D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
	}
}

void Rendering::PrimitiveTopologyUtils::Activate(D3D11_PRIMITIVE_TOPOLOGY topology)
{
	Globals::pGpu->GetContext().IASetPrimitiveTopology(topology);
}

void Rendering::PrimitiveTopologyUtils::Activate(PrimitiveTopology topology)
{
	Activate(ToDx(topology));
}
