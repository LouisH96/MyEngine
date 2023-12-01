#pragma once
#include "Io/Fbx/FbxValueCurve.h"
#include "Math/Vectors.h"
#include "Transform/Transform.h"

namespace MyEngine
{
	namespace Io
	{
		namespace Fbx
		{
			namespace Wrapping
			{
				class Model;

				class FbxOrientation
				{
				public:
					FbxOrientation() = default;
					explicit FbxOrientation(float scale);

					float GetScale() const { return m_Scale; }

					static bool IsLeftHanded() { return false; } 
					static bool HasClockwiseWinding() { return IsLeftHanded(); }

					Float3 ConvertPoint(const double* pPoint) const;
					Float3 ConvertPoint(const Float3& point) const;
					template<typename T> void ConvertPoints(FbxValueCurve<T>* pFirst) const;

					static Float3 ConvertRotation(const Float3& rotation);
					template<typename T> static void ConvertRotations(FbxValueCurve<T>* pFirst);

					Game::Transform MakeLocalTransform(const Wrapping::Model& model) const;

				private:
					float m_Scale{ 1 };
				};

				template <typename T>
				void FbxOrientation::ConvertPoints(FbxValueCurve<T>* pFirst) const
				{
					pFirst->ScaleValues(-m_Scale);
					(++pFirst)->ScaleValues(m_Scale);
					(++pFirst)->ScaleValues(m_Scale);
				}

				template <typename T>
				void FbxOrientation::ConvertRotations(FbxValueCurve<T>* pFirst) 
				{
					(++pFirst)->ScaleValues(-1); //y
					(++pFirst)->ScaleValues(-1); //z
				}
			}
		}
	}
}
