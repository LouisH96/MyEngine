#pragma once

#include "MakerResult.h"

namespace MyEngine
{
namespace MeshMaker
{
template<typename TVertex, ModelTopology TTopology>
class HoleArrayResult
	: public MakerResult<TVertex, TTopology>
{
public:
	static constexpr unsigned NrCorners{ 4 };

	HoleArrayResult() = default;
	HoleArrayResult(MakerResult<TVertex, TTopology>&& base);

	void SetLeftBot(unsigned index) { m_Corners[0] = index; }
	void SetLeftTop(unsigned index) { m_Corners[1] = index; }
	void SetRightBot(unsigned index) { m_Corners[2] = index; }
	void SetRightTop(unsigned index) { m_Corners[3] = index; }

	unsigned GetLeftBot() const { return m_Corners[0]; }
	unsigned GetLeftTop() const { return m_Corners[1]; }
	unsigned GetRightBot() const { return m_Corners[2]; }
	unsigned GetRightTop() const { return m_Corners[3]; }

	HoleArrayResult Duplicate(MeshData<TVertex, TTopology>& meshData);

private:
	using BaseClass = MakerResult<TVertex, TTopology>;
	using BaseClass::Duplicator;

	//Might not exist (when NrHoleCorners is 3 or 4)
	unsigned m_Corners[NrCorners]{ Uint::MAX, Uint::MAX, Uint::MAX, Uint::MAX };
};

template<typename TVertex, ModelTopology TTopology>
inline HoleArrayResult<TVertex, TTopology>::HoleArrayResult(MakerResult<TVertex, TTopology>&& base)
	: MakerResult<TVertex, TTopology>{ std::move(base) }
{

}

template<typename TVertex, ModelTopology TTopology>
class DefaultHoleArrayResult
	: public MakerResult<TVertex, TTopology>
{
public:
	void SetLeftBot(unsigned index) {};
	void SetLeftTop(unsigned index) {};
	void SetRightBot(unsigned index) {};
	void SetRightTop(unsigned index) {};
};

template<typename TVertex, ModelTopology TTopology>
inline HoleArrayResult<TVertex, TTopology> HoleArrayResult<TVertex, TTopology>::Duplicate(MeshData<TVertex, TTopology>& meshData)
{
	Duplicator duplicator{ BaseClass::GetDuplicator(meshData) };
	HoleArrayResult result{ std::move(duplicator.GetMakerResult())};

	result.SetLeftBot(duplicator.GetUpdated(GetLeftBot()));
	result.SetLeftTop(duplicator.GetUpdated(GetLeftTop()));
	result.SetRightBot(duplicator.GetUpdated(GetRightBot()));
	result.SetRightTop(duplicator.GetUpdated(GetRightTop()));

	return result;
}
}
}