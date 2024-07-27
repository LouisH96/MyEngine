#include "pch.h"
#include "MakerResult.h"

using namespace MeshMaker;

MakerResult::MakerResult(unsigned capacity)
	: m_Indices{ capacity }
{
}

void MakerResult::EnsureIncrease(unsigned increase)
{
	m_Indices.EnsureIncrease(increase);
}

void MakerResult::Add(unsigned index)
{
	m_Indices.Add(index);
}

void MakerResult::Add(const MakerResult& other)
{
	m_Indices.Add(other.m_Indices);
}

void MakerResult::RemoveLast()
{
	m_Indices.ReduceSize(1);
}
