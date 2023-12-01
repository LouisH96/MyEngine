#pragma once
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
					static Float3 ConvertRotation(const Float3& rotation);

					Game::Transform MakeLocalTransform(const Wrapping::Model& model) const;

				private:
					float m_Scale{ 1 };
				};
			}
		}
	}
}
