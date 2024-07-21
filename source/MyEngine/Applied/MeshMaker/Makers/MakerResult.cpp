#include "pch.h"
#include "MakerResult.h"

using namespace MeshMaker;

MakerResult::MakerResult(unsigned capacity)
	: m_Indices{ capacity }
{
}

void MakerResult::Add(unsigned index)
{
	m_Indices.Add(index);
}

void MakerResult::RemoveLast()
{
	m_Indices.ReduceSize(1);
}
