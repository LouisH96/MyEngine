#pragma once

namespace MyEngine
{
	namespace Game
	{
		class WorldMatrix
		{
		public:
			static Float3 GetPosition(const Float4X4& world);
			static float GetXPosition(const Float4X4& world);
			static float GetYPosition(const Float4X4& world);
			static float GetZPosition(const Float4X4& world);

			static void SetRotation(Float4X4& world, const Float3& forward);
			static Float3 GetRight(const Float4X4& world);
			static Float3 GetUp(const Float4X4& world);
			static Float3 GetForward(const Float4X4& world);

			static void SetPosition(Float4X4& world, const Float3& position);
			static Float4X4 Translation(const Float3& translation);
		};
	}
}
