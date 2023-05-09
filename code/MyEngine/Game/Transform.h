#pragma once
#include "Math/Float4X4.h"
#include "Math/Quaternion.h"

namespace MyEngine
{
	namespace Game
	{
		class Transform
		{
		public:
			Transform() = default;
			Transform(const Math::Float3& position, const Math::Quaternion& rotation);
			Transform(const DirectX::XMMATRIX& matrix);
			Transform& operator=(const DirectX::XMMATRIX& matrix);

			Math::Float3 Position{};
			Math::Quaternion Rotation{};

			Math::Float4X4 AsMatrix() const;
			Math::Float4X4 GetTransposeInverse() const;

			void LookAt(const Math::Float3& target);

			Math::Float3 WorldToLocal(const Math::Float3& worldPoint) const;

			void SetRelativeTo(const Transform& parent);
			Transform GetRelativeTo(const Transform& parent) const;

			static Transform WorldToLocal(const Transform& world, const Transform& parent);
			static Transform LocalToWorld(const Transform& local, const Transform& parent);
		};
	}
}
