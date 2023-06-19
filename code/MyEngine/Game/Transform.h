#pragma once
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
			explicit Transform(const Math::Double4X4& matrix);
			explicit Transform(const Math::Float4X4& matrix);
			Transform& operator=(const DirectX::XMMATRIX& matrix);

			Math::Float3 Position{};
			Math::Quaternion Rotation{};

			Math::Float4X4 AsMatrix() const;
			Math::Float4X4 GetTransposeInverse() const;

			void LookAt(const Math::Float3& target);

			Math::Float3 WorldToLocal(const Math::Float3& worldPoint) const;
			Math::Float3 LocalToWorld(const Math::Float3& localPoint) const;

			void SetRelativeTo(const Transform& parent);
			Transform GetRelativeTo(const Transform& parent) const;

			static Transform WorldToLocal(const Transform& world, const Transform& parent);
			static Transform LocalToWorld(const Transform& local, const Transform& parent);
		};
	}
}
