#pragma once
#include "Math/Double4x4.h"
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
			Transform(const Float3& position, const Quaternion& rotation);
			Transform(const DirectX::XMMATRIX& matrix);
			explicit Transform(const Double4X4& matrix);
			Transform& operator=(const DirectX::XMMATRIX& matrix);

			Float3 Position{};
			Quaternion Rotation{};

			Float4X4 AsMatrix() const;
			Float4X4 GetTransposeInverse() const;

			void LookAt(const Float3& target);

			Float3 WorldToLocal(const Float3& worldPoint) const;
			Float3 LocalToWorld(const Float3& localPoint) const;

			void SetRelativeTo(const Transform& parent);
			Transform GetRelativeTo(const Transform& parent) const;

			static Transform WorldToLocal(const Transform& world, const Transform& parent);
			static Transform LocalToWorld(const Transform& local, const Transform& parent);
		};
	}
}
