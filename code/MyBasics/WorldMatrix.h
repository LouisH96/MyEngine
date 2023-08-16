#pragma once
#include "Math/Matrices.h"
#include "Math/Vectors.h"

namespace MyEngine
{
	namespace Game
	{
		//should have no scale
		class WorldMatrix
		{
		public:
			static Float3 GetPosition(const Float4X4& world);
			static float GetXPosition(const Float4X4& world);
			static float GetYPosition(const Float4X4& world);
			static float GetZPosition(const Float4X4& world);

			static Float2 GetPositionXz(const Float4X4& world);
			static void SetPositionXz(Float4X4& world, const Float2& xz);

			static void SetRotation(Float4X4& world, const Float3& forward);
			static Float3 GetRight(const Float4X4& world);
			static Float3 GetUp(const Float4X4& world);
			static Float3 GetForward(const Float4X4& world);
			static Float2 GetForwardXz(const Float4X4& world);

			static float GetYaw(const Float4X4& world);
			static void SetYaw(Float4X4& world, float yaw);

			static void SetPosition(Float4X4& world, const Float3& position);
			static Float4X4 Translation(const Float3& translation);

			static void Translate(Float4X4& m, const Float3& translation);
			static void TranslateRelativeX(Float4X4& m, float translation);
			static void TranslateRelativeY(Float4X4& m, float translation);

			static void SetPitchRotation(Float4X4& m ,float pitch);
		};
	}
}
