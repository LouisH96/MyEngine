#include "pch.h"
#include "PrismGenerator.h"

#include "PlaneGeneration.h"
#include "Geometry/Shapes/Quad.h"

using namespace MyEngine;

void Generation::PrismGenerator::Create(const Quad& quadBot, const Quad& quadTop, const Float3& color,
                                        List<Rendering::V_PosColNorm>& vertices, List<int>& indices, const Float3& offset)
{
	vertices.EnsureIncrease(4 * 6);
	indices.EnsureIncrease(6 * 6);
	ConnectSides(quadBot, quadTop, color, vertices, indices, offset);
	ConnectTop(quadBot, quadTop, color, vertices, indices, offset);
	ConnectBot(quadBot, quadTop, color, vertices, indices, offset);
}

void Generation::PrismGenerator::ConnectBack(const Quad& quadBot, const Quad& quadTop, const Float3& color,
	List<Rendering::V_PosColNorm>& vertices, List<int>& indices, const Float3& offset)
{
	vertices.EnsureIncrease(4);
	indices.EnsureIncrease(6);
	PlaneGeneration::Create(
		quadBot.GetLeftBot() + offset, quadTop.GetLeftBot() + offset,
		quadTop.GetRightBot() + offset, quadBot.GetRightBot() + offset, color,
		vertices, indices);
}

void Generation::PrismGenerator::ConnectFront(const Quad& quadBot, const Quad& quadTop, const Float3& color,
	List<Rendering::V_PosColNorm>& vertices, List<int>& indices, const Float3& offset)
{
	vertices.EnsureIncrease(4);
	indices.EnsureIncrease(6);
	PlaneGeneration::Create(
		quadBot.GetRightTop() + offset, quadTop.GetRightTop() + offset,
		quadTop.GetLeftTop() + offset, quadBot.GetLeftTop() + offset, color,
		vertices, indices);
}

void Generation::PrismGenerator::ConnectLeft(const Quad& quadBot, const Quad& quadTop, const Float3& color,
	List<Rendering::V_PosColNorm>& vertices, List<int>& indices, const Float3& offset)
{
	vertices.EnsureIncrease(4);
	indices.EnsureIncrease(6);
	PlaneGeneration::Create(
		quadBot.GetLeftTop() + offset, quadTop.GetLeftTop() + offset,
		quadTop.GetLeftBot() + offset, quadBot.GetLeftBot() + offset, color,
		vertices, indices);
}

void Generation::PrismGenerator::ConnectRight(const Quad& quadBot, const Quad& quadTop, const Float3& color,
	List<Rendering::V_PosColNorm>& vertices, List<int>& indices, const Float3& offset)
{
	vertices.EnsureIncrease(4);
	indices.EnsureIncrease(6);
	PlaneGeneration::Create(
		quadBot.GetRightBot() + offset, quadTop.GetRightBot() + offset,
		quadTop.GetRightTop() + offset, quadBot.GetRightTop() + offset, color,
		vertices, indices);
}

void Generation::PrismGenerator::ConnectTop(const Quad&, const Quad& quadTop, const Float3& color,
	List<Rendering::V_PosColNorm>& vertices, List<int>& indices, const Float3& offset)
{
	vertices.EnsureIncrease(4);
	indices.EnsureIncrease(6);
	PlaneGeneration::Create(
		quadTop.GetLeftBot() + offset, quadTop.GetLeftTop() + offset,
		quadTop.GetRightTop() + offset, quadTop.GetRightBot() + offset, color,
		vertices, indices);
}

void Generation::PrismGenerator::ConnectBot(const Quad& quadBot, const Quad&, const Float3& color,
	List<Rendering::V_PosColNorm>& vertices, List<int>& indices, const Float3& offset)
{
	vertices.EnsureIncrease(4);
	indices.EnsureIncrease(6);
	PlaneGeneration::Create(
		quadBot.GetLeftTop() + offset, quadBot.GetLeftBot() + offset,
		quadBot.GetRightBot() + offset, quadBot.GetRightTop() + offset, color,
		vertices, indices);
}

void Generation::PrismGenerator::ConnectSides(
	const Quad& quadBot, const Quad& quadTop, const Float3& color,
	List<Rendering::V_PosColNorm>& vertices, List<int>& indices, const Float3& offset)
{
	constexpr unsigned nrSides = 4;
	vertices.EnsureIncrease(4 * nrSides);
	indices.EnsureIncrease(3 * 2 * nrSides);
	ConnectBack(quadBot, quadTop, color, vertices, indices, offset);
	ConnectFront(quadBot, quadTop, color, vertices, indices, offset);
	ConnectLeft(quadBot, quadTop, color, vertices, indices, offset);
	ConnectRight(quadBot, quadTop, color, vertices, indices, offset);
}

void Generation::PrismGenerator::Connect(const RectFloat* pRects, const float* pHeightSteps, unsigned nrRects, const Float3& color,
	List<Rendering::V_PosColNorm>& vertices, List<int>& indices,
	const Float3& offset, bool capBegin, bool capEnd)
{
	const unsigned nrSides = 4 * (nrRects - 1) + capBegin ? 1 : 0 + capEnd ? 1 : 0;
	vertices.EnsureIncrease(4 * nrSides);
	indices.EnsureIncrease(6 * nrSides);

	float height{ pHeightSteps[0] };
	if (capBegin)
	{
		const RectFloat& rect{ pRects[0] };
		PlaneGeneration::TowardYMax(
			Float3{ rect.GetLeft(), height, rect.GetTop()} + offset, rect.GetSize(),
			color, vertices, indices);
	}
	for (unsigned i = 1; i < nrRects; i++)
	{
		const RectFloat& bottomRect{ pRects[i - 1] };
		const RectFloat& topRect{ pRects[i] };
		const Quad quadBottom{
			Float3{bottomRect.GetLeft(), height, bottomRect.GetBottom()},
			{1,0,0},{0,0,1},bottomRect.GetSize() };
		height += pHeightSteps[i];
		const Quad quadTop{
			Float3{topRect.GetLeft(), height, topRect.GetBottom()},
			{1,0,0},{0,0,1}, topRect.GetSize() };
		ConnectSides(quadBottom, quadTop, color, vertices, indices, offset);
	}
	if(capEnd)
	{
		const RectFloat& rect{ pRects[nrRects - 1] };
		PlaneGeneration::TowardYMin(
			Float3{ rect.GetLeft(), height, rect.GetBottom() } + offset, rect.GetSize(),
			color, vertices, indices);
	}
}
