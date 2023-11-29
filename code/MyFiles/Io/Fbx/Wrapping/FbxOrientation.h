#pragma once
#include "Math/Vectors.h"

namespace MyEngine
{
	namespace Io
	{
		namespace Fbx
		{
			namespace Wrapping
			{
				//not thoroughly tested
				class FbxOrientation
				{
				public:
					FbxOrientation() = default;
					explicit FbxOrientation(int upAxis);

					//int GetUpAxis() const { return m_UpAxis; }
					bool IsLeftHanded() const { return m_IsLeftHanded; } 
					bool HasClockwiseWinding() const { return IsLeftHanded(); }

					Float3 ConvertPoint(const double* pPoint) const;
					Float3 ConvertPoint(const Float3& point) const;

					Float3 ConvertRotation(const Float3& rotation) const;

					float GetXSign() const { return m_XSign; }
					float GetYSign() const { return m_YSign; }
					float GetZSign() const { return m_ZSign; }
					float GetSign(unsigned idx) const { return (&m_XSign)[idx]; }

				private:
					int m_UpAxis{ -10 };
					bool m_IsLeftHanded;

					unsigned m_XOffset{ 0 };
					unsigned m_YOffset{ 1 };
					unsigned m_ZOffset{ 2 };
					float m_XSign{ 1 };
					float m_YSign{ 1 };
					float m_ZSign{ 1 };
				};
			}
		}
	}
}
