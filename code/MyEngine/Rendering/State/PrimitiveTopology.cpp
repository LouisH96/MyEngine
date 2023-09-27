#include "pch.h"
#include "PrimitiveTopology.h"

#include "Rendering/Gpu.h"

using namespace Rendering;

D3D11_PRIMITIVE_TOPOLOGY PrimitiveTopology::ToDx(ModelTopology modelTopology)
{
	switch (modelTopology)
	{
	case ModelTopology::LineList:
	case ModelTopology::LineListIdx:
		return D3D11_PRIMITIVE_TOPOLOGY_LINELIST;

	case ModelTopology::LineStrip:
	case ModelTopology::LineStripIdx:
		return D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;

	case ModelTopology::TriangleList:
	case ModelTopology::TriangleListIdx:
		return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	case ModelTopology::TriangleStrip:
	case ModelTopology::TriangleStripIdx:
		return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

	default:
	case ModelTopology::Unknown:
		return D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;;
	}
}

void PrimitiveTopology::Activate(D3D11_PRIMITIVE_TOPOLOGY topology)
{
	Globals::pGpu->GetContext().IASetPrimitiveTopology(topology);
}

void PrimitiveTopology::Activate(ModelTopology modelTopology)
{
	Activate(ToDx(modelTopology));
}

PrimitiveTopology::PrimitiveTopology(ModelTopology modelTopology)
	: m_DxTopology{ ToDx(modelTopology) }
	, m_ModelTopology{ modelTopology }
{
}

void PrimitiveTopology::Activate() const
{
	Activate(m_DxTopology);
}
