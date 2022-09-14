#pragma once
#include "ICamera.h"

namespace MyEngine
{
	namespace Game
	{
		namespace Camera
		{
			class Camera final
				: public ICamera
			{
			public:
				void Move(DirectX::XMFLOAT3 movement) override;

				const DirectX::XMFLOAT4X4& GetMatrix() const { return m_Matrix; }
			private:
				DirectX::XMFLOAT4X4 m_Matrix{1,0,0,0, 0,1,0,0, 0,0,1,0 ,0,0,0,1};
			};
		}
	}
}
