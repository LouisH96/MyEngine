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
				void Translate(DirectX::XMFLOAT3 translation) override;

			private:
				DirectX::XMFLOAT3 m_Position{ 0,0,0 };
			};
		}
	}
}
