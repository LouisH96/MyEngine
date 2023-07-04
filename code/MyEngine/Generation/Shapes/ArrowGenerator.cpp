#include "pch.h"
#include "ArrowGenerator.h"

#include "Generation/MeshUtils.h"
#include "Math/Constants.h"


void Generation::ArrowGenerator::CreatePivotArrows(Array<Rendering::V_PosColNorm>& vertices, Array<int>& indices, int nrSides, const Float3& origin, float scale)
{
	using namespace Rendering;
	constexpr float lineRadius = .05f;
	ArrowGenerator arrowGenerator{ nrSides, false, .8f, lineRadius, .2f, .1f };

	vertices = { arrowGenerator.GetNrVertices() * 2 };
	indices = { arrowGenerator.GetNrIndices() * 2 };

	//GET ARROW MODEL
	Array<Float3> positions{ };
	Array<Float3> normals{};
	Array<int> tempIndices{};

	arrowGenerator.Generate(positions, normals, tempIndices);

	const int lineBeginIdx = arrowGenerator.GetSectionBegin(VertexSection::LineBegin);
	const float angleStep = arrowGenerator.GetAngleStep();
	for(int i = 0; i < nrSides; i++)
	{
		const float c = cosf(i * angleStep);
		const float diff = c * lineRadius;
		positions[i + lineBeginIdx].x += diff;
	}

	for (int i = 0; i < positions.GetSize(); i++)
		vertices[i] = V_PosColNorm{ positions[i] * scale + origin, {.8f,.1f,.1f}, normals[i] };
	for (int i = 0; i < tempIndices.GetSize(); i++)
		indices[i] = tempIndices[i];

	int idx = positions.GetSize();

	//rotate to Z
	for(int i = 0; i < positions.GetSize(); i++)
	{
		const float oldPositionX = positions[i].x;
		positions[i].x = positions[i].z;
		positions[i].z = oldPositionX;
		positions[i].y = -positions[i].y;
		const float oldNormalX = normals[i].x;
		normals[i].x = normals[i].z;
		normals[i].z = oldNormalX;
		normals[i].y = -normals[i].y;
	}
	
	for (int i = 0; i < positions.GetSize(); i++)
		vertices[i + idx] = V_PosColNorm{ positions[i] * scale + origin, {.1f,.1f,.8f}, normals[i] };
	for (int i = 0; i < arrowGenerator.GetNrIndices(); i++)
		indices[i + arrowGenerator.GetNrIndices()] = tempIndices[i] + idx;

	//rotate to Y
	/*idx *= 2;
	for(int i = 0; i < positions.GetSize(); i++)
	{
		const float oldPositionZ = positions[i].z;
		positions[i].z = positions[i].y;
		positions[i].y = oldPositionZ;
		positions[i].x = -positions[i].x;
		const float oldNormalZ = normals[i].z;
		normals[i].z = normals[i].y;
		normals[i].y = oldNormalZ;
		normals[i].x = -normals[i].x;
	}

	for (int i = 0; i < positions.GetSize(); i++)
		vertices[i + idx] = V_PosColNorm{ positions[i], {.1f,.8f,.1f} , normals[i] };
	for (int i = 0; i < arrowGenerator.GetNrIndices(); i++)
		indices[i + arrowGenerator.GetNrIndices() * 2] = tempIndices[i] + idx;*/
}

Generation::ArrowGenerator::ArrowGenerator(int nrSides, bool capLineEnd, float lineLength, float lineRadius,
	float arrowLength, float arrowRadius)
	: m_NrSides{ nrSides }
	, m_CapLineEnd{ capLineEnd }
	, m_LineRadius{ lineRadius }
	, m_LineLength{ lineLength }
	, m_ArrowRadius{ arrowRadius }
	, m_ArrowLength{ arrowLength }
{
}

int Generation::ArrowGenerator::GetNrVertices() const
{
	return GetSectionBegin(VertexSection::Last);
}

int Generation::ArrowGenerator::GetNrTriangles() const
{
	return GetSectionBegin(TriangleSection::Last);
}

float Generation::ArrowGenerator::GetAngleStep() const
{
	return Constants::PI / static_cast<float>(m_NrSides) * 2.f;
}

int Generation::ArrowGenerator::GetSectionLength(VertexSection section) const
{
	switch (section)
	{
	case VertexSection::LineCap: return m_CapLineEnd ? m_NrSides : 0;
	case VertexSection::LineBegin:return  m_NrSides;
	case VertexSection::LineEnd: return m_NrSides;
	case VertexSection::Head: return m_NrSides;
	case VertexSection::HeadCap: return m_NrSides;
	case VertexSection::Tip: return 1;
	case VertexSection::Last:
	default:
		Logger::PrintError("Unknown ArrowGenerator-VertexSection");
		return -1;;
	}
}

int Generation::ArrowGenerator::GetSectionBegin(VertexSection section) const
{
	int begin = 0;
	for (int i = 0; i < static_cast<int>(section); i++)
		begin += GetSectionLength(static_cast<VertexSection>(i));
	return begin;
}

int Generation::ArrowGenerator::GetSectionLength(TriangleSection section) const
{
	switch (section)
	{
	case TriangleSection::LineCap: return m_CapLineEnd ? MeshUtils::GetNrTrianglesToCapCircle(m_NrSides) : 0;
	case TriangleSection::Line: return m_NrSides * 2;
	case TriangleSection::HeadCap: return MeshUtils::GetNrTrianglesToCapCircle(m_NrSides);
	case TriangleSection::Head: return m_NrSides;
	case TriangleSection::Last:
	default:
		Logger::PrintError("Unknown ArrowGenerator-TriangleSection");
		return -1;
	}
}

int Generation::ArrowGenerator::GetSectionBegin(TriangleSection section) const
{
	int begin = 0;
	for (int i = 0; i < static_cast<int>(section); i++)
		begin += GetSectionLength(static_cast<TriangleSection>(i));
	return begin;
}

void Generation::ArrowGenerator::Generate(Array<Float3>& vertexPoints, Array<Float3>& vertexNormals,
	Array<int>& indices) const
{
	vertexPoints = { GetNrVertices() };
	vertexNormals = { GetNrVertices() };
	indices = { GetNrTriangles() * 3 };

	//FOR EACH ANGLE
	const float angleStep = Constants::PI / static_cast<float>(m_NrSides) * 2.f;
	float currentAngle = 0;
	for (int i = 0; i < m_NrSides; i++)
	{
		const float cos = cosf(i * angleStep);
		const float sin = sinf(i * angleStep);
		const float arrowZ = cos * m_ArrowRadius;
		const float arrowY = -sin * m_ArrowRadius;
		const Float3 basicNormal{ Float3{0,-sin,cos}.Normalized() };

		//LINE CAP
		Float3 point{ 0, sin * m_LineRadius, cos * m_LineRadius };
		int vertexIdx;
		if (m_CapLineEnd)
		{
			vertexIdx = i + GetSectionBegin(VertexSection::LineCap);
			vertexPoints[vertexIdx] = point;
			vertexNormals[vertexIdx] = { -1,0,0 };
		}

		//LINE BEGIN
		vertexIdx = i + GetSectionBegin(VertexSection::LineBegin);
		point.y = -point.y;
		vertexPoints[vertexIdx] = point;
		vertexNormals[vertexIdx] = basicNormal;

		//LINE END
		vertexIdx = i + GetSectionBegin(VertexSection::LineEnd);
		point.x = m_LineLength;
		vertexPoints[vertexIdx] = point;
		vertexNormals[vertexIdx] = basicNormal;

		//HEAD CAP
		vertexIdx = i + GetSectionBegin(VertexSection::HeadCap);
		point.y = sin * m_ArrowRadius;
		point.z = cos * m_ArrowRadius;
		vertexPoints[vertexIdx] = point;
		vertexNormals[vertexIdx] = { -1,0,0 };

		//HEAD
		vertexIdx = i + GetSectionBegin(VertexSection::Head);
		point.y = -point.y;
		vertexPoints[vertexIdx] = point;
		vertexNormals[vertexIdx] = basicNormal;

		currentAngle += angleStep;
	}

	//TIP
	int vertexIdx = GetSectionBegin(VertexSection::Tip);
	vertexPoints[vertexIdx] = { m_LineLength + m_ArrowLength , 0, 0 };
	vertexNormals[vertexIdx] = { .1f,0,0 };

	//INDICES
	int indexIdx;

	//LINE CAP
	if (m_CapLineEnd)
	{
		vertexIdx = GetSectionBegin(VertexSection::LineCap);
		indexIdx = 3 * GetSectionBegin(TriangleSection::LineCap);
		MeshUtils::CapCircle(m_NrSides, indexIdx, vertexIdx, indices);
	}

	//LINE
	vertexIdx = GetSectionBegin(VertexSection::LineBegin);
	indexIdx = 3 * GetSectionBegin(TriangleSection::Line);
	for (int i = 0; i < m_NrSides; i++)
	{
		const int beginVertexIdx = i + GetSectionBegin(VertexSection::LineBegin);
		const int endVertexIdx = i + GetSectionBegin(VertexSection::LineEnd);

		const int rightTopIdx = beginVertexIdx;
		const int leftTopIdx = endVertexIdx;
		const int rightBotIdx = i == m_NrSides - 1 ? GetSectionBegin(VertexSection::LineBegin) : rightTopIdx + 1;
		const int leftBotIdx = i == m_NrSides - 1 ? GetSectionBegin(VertexSection::LineEnd) : leftTopIdx + 1;

		indexIdx = i * 3 * 2 + 3 * GetSectionBegin(TriangleSection::Line);
		indices[indexIdx + 0] = rightTopIdx;
		indices[indexIdx + 1] = rightBotIdx;
		indices[indexIdx + 2] = leftBotIdx;
		indices[indexIdx + 3] = rightTopIdx;
		indices[indexIdx + 4] = leftBotIdx;
		indices[indexIdx + 5] = leftTopIdx;
	}

	//HEAD CAP
	vertexIdx = GetSectionBegin(VertexSection::HeadCap);
	indexIdx = 3 * GetSectionBegin(TriangleSection::HeadCap);
	MeshUtils::CapCircle(m_NrSides, indexIdx, vertexIdx, indices);

	//HEAD
	vertexIdx = GetSectionBegin(VertexSection::Head);
	indexIdx = 3 * GetSectionBegin(TriangleSection::Head);
	const int tipVertexIdx = GetSectionBegin(VertexSection::Tip);
	for (int i = 0; i < m_NrSides; i++)
	{
		const int rightTopVertexIdx = vertexIdx + i;
		const int leftBotVertexIdx = i == m_NrSides - 1 ? vertexIdx : vertexIdx + i + 1;
		indices[indexIdx + i * 3 + 0] = rightTopVertexIdx;
		indices[indexIdx + i * 3 + 1] = leftBotVertexIdx;
		indices[indexIdx + i * 3 + 2] = tipVertexIdx;
	}
}
